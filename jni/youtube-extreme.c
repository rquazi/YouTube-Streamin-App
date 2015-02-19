/*
 ============================================================================
 Name        : youtube-extreme.c
 Author      : mhoque
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================

 Name        : youtube-extreme JNI adaption for android
 Author      : quazi
 Date        : 14.11.2011
 */
#include "youtube-extreme.h"
#include "http_response.h"
#include <stdio.h>
#include <stdlib.h>

/*android specific headers*/
#include <jni.h>
#include <android/log.h>

/*for android logs*/
#define LOG_TAG "YoutubeTestAndroid"
#define LOG_LEVEL 10
#define LOGI(level, ...) if (level <= LOG_LEVEL) {__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__);}
#define LOGE(level, ...) if (level <= LOG_LEVEL) {__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__);}


int file_status;

int file_count=0;
int access_file;

pthread_mutex_t m_file_access,m_count_access;




int open_squid_port()
{
    struct sockaddr_in rem_addr;
    int len, s, x;
    struct hostent *H;
    int on = 1;

    H = gethostbyname("localhost");
    if (!H)
	return (-2);

    len = sizeof(rem_addr);

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0)
	return s;

    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, 4);
    int window_size=256*1024;
    setsockopt(s,SOL_SOCKET,SO_SNDBUF,&window_size,sizeof(window_size));
    setsockopt(s,SOL_SOCKET,SO_RCVBUF,&window_size,sizeof(window_size));
    len = sizeof(rem_addr);
    memset(&rem_addr, '\0', len);
    rem_addr.sin_family = AF_INET;
    memcpy(&rem_addr.sin_addr, H->h_addr, H->h_length);
    rem_addr.sin_port = htons(SQUID_PORT);
    x = connect(s, (struct sockaddr *) &rem_addr, len);
    if (x < 0)
    {
    	close(s);
    	return x;
    }

    return s;
}
/*

int get_last_keyframe_pos(struct Metadata* meta)
{

	int i=0,j=0;

	AVFormatContext *p_format_ctx;
	AVCodecContext  *p_codec_ctx;
	AVCodec         *p_codec;
	AVFrame         *p_frame;
	AVPacket        packet;

	int video_stream,frame_finished;
	//int max_bee_pee_fr=0;

	struct KF_data kf_data[50];
	struct BP_data bp_data[2000];
	//printf("%s:Filename ---------------%s\n",__FUNCTION__,meta->filename);
	av_register_all();
	if(av_open_input_file(&p_format_ctx, meta->filename, NULL, 0, NULL)!=0)
		return -1;

	if(av_find_stream_info(p_format_ctx)<0)
	    return -1;


	video_stream=-1;

	for(i=0; i<p_format_ctx->nb_streams; i++)
	if(p_format_ctx->streams[i]->codec->codec_type==CODEC_TYPE_VIDEO)
	{
	  video_stream=i;
	  break;
	}
	if(video_stream==-1)
		return -1;



	p_codec_ctx=p_format_ctx->streams[video_stream]->codec;

	p_codec=avcodec_find_decoder(p_codec_ctx->codec_id);

	if(p_codec==NULL)
	  return -1;


	if(avcodec_open(p_codec_ctx, p_codec)<0)
        return -1;



    p_frame=avcodec_alloc_frame();

    av_init_packet(&packet);



    while(av_read_frame(p_format_ctx, &packet)>=0)
    {

        if(packet.stream_index==video_stream)
        {

            avcodec_decode_video2(p_codec_ctx, p_frame, &frame_finished,&packet);
            if(frame_finished)
            {
            	if(p_frame->key_frame)
            	{
            		kf_data[i].kf_pos=packet.pos;
            		kf_data[i].kf_time=packet.pts;
            		kf_data[i].kf_size=packet.size;
            		++i;

            	}
            	else
            	{
            		bp_data[j].bee_pee_fr_pos=packet.pos;
            	    bp_data[j].bee_pee_fr_time=packet.pts;
            	    bp_data[j].bee_pee_fr_size=packet.size;

            	    ++j;

            	}
            }
        }
        av_free_packet(&packet);
    }
    	meta->first_kf_pos=kf_data[0].kf_pos;
    	meta->current_file_size=p_format_ctx->file_size;
    	meta->last_kf_index=kf_data[i-1].kf_pos;
    	meta->last_kf_time=kf_data[i-1].kf_time;
    	meta->last_kf_fs_dif=meta->current_file_size-meta->last_kf_index;
    	meta->last_kf_size=kf_data[i-1].kf_size;
    	meta->last_kf_pos_dif=kf_data[i-1].kf_pos-kf_data[i-2].kf_pos;
    	printf("The First Key Frame in forward direction..................%f......pos:\n",meta->first_kf_pos);
    	printf("The Last Key Frame in forward direction..................%f......pos:\n",meta->last_kf_index);
    //meta->frame_size=frame_size;


    av_free(p_frame);
    avcodec_close(p_codec_ctx);
    av_close_input_file(p_format_ctx);

    return 1;

}
*/

/*
void set_and_dump_metadata(AVFormatContext* p_format_ctx,struct Metadata* meta)
{
	AVMetadataTag *neta_tag;
	neta_tag=av_metadata_get(p_format_ctx->metadata, "videodatarate", NULL, 0);
	if(neta_tag!=NULL)
	{
		meta->video_rate=atoi(neta_tag->value)*1024;
		printf("%s....%s.....%ld\n",__FUNCTION__,neta_tag->key,meta->video_rate);

	}
	neta_tag=av_metadata_get(p_format_ctx->metadata, "audiodatarate", NULL, 0);
	if(neta_tag!=NULL)
	{
		meta->audio_rate=atoi(neta_tag->value)*1024;
		printf("%s....%s.....%ld\n",__FUNCTION__,neta_tag->key,meta->audio_rate);

	}
	neta_tag=av_metadata_get(p_format_ctx->metadata, "totaldatarate", NULL, 0);
	if(neta_tag!=NULL)
	{
		meta->total_data_rate=atoi(neta_tag->value)*1024;
		printf("%s....%s.....%ld\n",__FUNCTION__,neta_tag->key,meta->total_data_rate);
	}

	neta_tag=av_metadata_get(p_format_ctx->metadata, "framerate", NULL, 0);
	if(neta_tag!=NULL)
	{
		meta->frame_rate=atoi(neta_tag->value);
		printf("%s....%s.....%d\n",__FUNCTION__,neta_tag->key,meta->frame_rate);

	}
	neta_tag=av_metadata_get(p_format_ctx->metadata, "bytelength", NULL, 0);
	if(neta_tag!=NULL)
	{
		meta->byte_length=(long int)atof(neta_tag->value);
		printf("%s....%s.....%ld\n",__FUNCTION__,neta_tag->key,meta->byte_length);
	}

	neta_tag=av_metadata_get(p_format_ctx->metadata, "starttime", NULL, 0);
	if(neta_tag!=NULL){
		meta->start_time=(long int)atoi(neta_tag->value)*1000;
		printf("%s....%s.....%ld\n",__FUNCTION__,neta_tag->key,meta->start_time);
	}
	neta_tag=av_metadata_get(p_format_ctx->metadata, "duration", NULL, 0);
	if(neta_tag!=NULL)
	{	meta->duration=(long int)atoi(neta_tag->value)*1000;
		printf("%s....%s.....%ld\n",__FUNCTION__,neta_tag->key,meta->duration);
	}
	neta_tag=av_metadata_get(p_format_ctx->metadata, "totalduration", NULL, 0);
	if(neta_tag!=NULL)
	{
		meta->total_duration=(long int)atoi(neta_tag->value)*1000;
		printf("%s....%s.....%ld\n",__FUNCTION__,neta_tag->key,meta->total_duration);
	}

	neta_tag=av_metadata_get(p_format_ctx->metadata, "height", NULL, 0);
	if(neta_tag!=NULL)
	{
		meta->video_height=(int)atoi(neta_tag->value);
		printf("%s....%s.....%d\n",__FUNCTION__,neta_tag->key,meta->video_height);
	}
	neta_tag=av_metadata_get(p_format_ctx->metadata, "width", NULL, 0);
	if(neta_tag!=NULL)
	{
		meta->video_width=(int)atoi(neta_tag->value);
		printf("%s....%s.....%d\n",__FUNCTION__,neta_tag->key,meta->video_width);
	}




}



int get_stream_info(struct Metadata * meta)
{

	AVFormatContext *p_format_ctx;
	int video_stream=0,i=0;
	if(av_find_stream_info(p_format_ctx)<0)
		return -1;



	video_stream=-1;

	for(i=0; i<p_format_ctx->nb_streams; i++)
	{
		if(p_format_ctx->streams[i]->codec->codec_type==CODEC_TYPE_VIDEO)
		{
		  video_stream=i;
		  break;
		}
		if(video_stream==-1)
			return -1;
	}
	meta->stream_info=video_stream;
	return video_stream;
}*/


int short_cut_decode(struct Metadata* meta,double time,int throttle_count, int flag)
{

	AVFormatContext *p_format_ctx;
	AVPacket        packet;
	int i=0,video_stream=-1,pos,pts,size;

	long int start;
	long int end;
	start=get_micro_sec();
	//if(avformat_open_input(&p_format_ctx, meta->filename, NULL, NULL)!=0)
	if(av_open_input_file(&p_format_ctx, meta->filename, NULL, 0, NULL)!=0)
		return -1;

	if(av_find_stream_info(p_format_ctx)<0)
			return -1;
	if(meta->stream_info==-1)
	{
		video_stream=-1;

		for(i=0; i<p_format_ctx->nb_streams; i++)
		if(p_format_ctx->streams[i]->codec->codec_id==CODEC_ID_H264)
		{
		  video_stream=i;
		  meta->stream_info=video_stream;
		  break;
		}

	}
	else
		video_stream=meta->stream_info;

	if(video_stream==-1)
		return -1;

	long long int seek_target=0;
	double r_time=0;

	if(!flag)
	{
		if(meta->last_kf_time==0)
			r_time=FIXED_PLAY_PERIOD/2;
		else
			r_time=meta->last_kf_time/1000;
	}


	if(flag)
	{
		if(meta->first_kf_time==0)
			r_time=0;
		else
			r_time=meta->first_kf_time/1000;
	}
	int count=0;

	av_init_packet(&packet);
	while(1)

	{
		seek_target=(int64_t)(r_time*AV_TIME_BASE);
		seek_target=av_rescale_q(seek_target, AV_TIME_BASE_Q,p_format_ctx->streams[video_stream]->time_base);
		if(av_seek_frame(p_format_ctx,video_stream,seek_target, AVSEEK_FLAG_ANY) < 0)
		{
			//fprintf(stderr, "%s: error while seeking forward\n",p_format_ctx->filename);
			break;
		}
		else
		{

			if(av_read_frame(p_format_ctx, &packet)>=0)
			{
				if((i==0)&&(flag))
				{

					meta->first_kf_pos=packet.pos;
					meta->first_kf_time=meta->last_kf_time;
					end=get_time_dif(start);
					//printf("Finding the first key frame:%ld.....elapsed time:%ld usec and while count:%d and r_time:%lld\n",meta->first_kf_time,end,count,seek_target);
					break;

				}

				pos=packet.pos;
				pts=packet.pts;
				size=packet.size;
				++i;


			}
		}
		if(flag)
			r_time=r_time+0.01;
		else
			r_time=r_time+0.3;

		av_free_packet(&packet);
		++count;
	}

	meta->current_file_size=p_format_ctx->file_size;

	if(!flag)
	{
		end=get_time_dif(start);
		meta->last_kf_index=pos;
		meta->last_kf_time=pts;
		meta->last_kf_size=size;
		meta->last_kf_fs_dif=meta->current_file_size-meta->last_kf_index;
		//printf("Finding the last key frame:%ld.....elapsed time:%ld usec and while count:%d and r_time:%lld\n",meta->last_kf_time,end,count,seek_target);

	}
	av_close_input_file(p_format_ctx);
	return 1;
}

int get_first_kframe_index(struct Metadata* meta)
{

	AVFormatContext *p_format_ctx;
	AVPacket        packet;
	int i=0,video_stream=-1,pos,pts,size;

	//long int start;
	//if(avformat_open_input(&p_format_ctx, meta->filename, NULL, NULL)!=0)
	if(av_open_input_file(&p_format_ctx, meta->filename, NULL, 0, NULL)!=0)
		return -1;
	if(av_find_stream_info(p_format_ctx)<0)
		return -1;
	video_stream=-1;

	for(i=0; i<p_format_ctx->nb_streams; i++)
	if(p_format_ctx->streams[i]->codec->codec_id==CODEC_ID_H264)
	{
	  video_stream=i;
	  break;
	}
	if(video_stream==-1)
		return -1;

	av_init_packet(&packet);

	double r_time=0.0;
	int64_t seek_target=0;
	while(1)

	{
		seek_target=(int64_t)(r_time*AV_TIME_BASE);
		seek_target=av_rescale_q(seek_target, AV_TIME_BASE_Q,p_format_ctx->streams[video_stream]->time_base);
		if(av_seek_frame(p_format_ctx,video_stream,seek_target, AVSEEK_FLAG_ANY) < 0)
		{
			//fprintf(stderr, "%s: error while seeking forward\n",p_format_ctx->filename);
			break;
		}
		else
		{

			if(av_read_frame(p_format_ctx, &packet)>=0)
			{
				if(i==0)
				{

					meta->first_kf_pos=packet.pos;
					meta->first_kf_time=meta->last_kf_time;
					printf("Finding the first key frame:%ld.....and r_time:%lld\n",meta->first_kf_time,seek_target);
					break;

				}

				pos=packet.pos;
				pts=packet.pts;
				size=packet.size;
				++i;


			}
		}
		r_time=r_time+0.01;
		av_free_packet(&packet);

	}
	av_close_input_file(p_format_ctx);
	return 1;
}


int send_big_chunks_client(struct Metadata *t_data, long long int temp_kf_fs_dif)
{
	long long int new_read_pos=0;
	int write_bytes=0;
	if (temp_kf_fs_dif>0)
	{

		FILE *temp=fopen(t_data->filename,"rb");
		if(temp!=NULL)
		{

			new_read_pos=t_data->first_kf_pos+temp_kf_fs_dif;
			fseek (temp,new_read_pos,SEEK_SET);


			long int pos=ftell(temp);
			u_char read_buf[(long int)t_data->current_file_size];
			memset(read_buf,'\0',t_data->current_file_size);
			fread(read_buf,1,(long int)t_data->current_file_size-pos,temp);
			write_bytes=write(t_data->mobile_sock,read_buf,(long int)t_data->current_file_size-pos);

			if(write_bytes<0)
				return -1;
		}

		temp_kf_fs_dif=t_data->last_kf_fs_dif;
		fclose(temp);

		printf("%s:Extra Bytes Received.........:%lld and sent bytes..........%d\n",__FUNCTION__,new_read_pos,write_bytes);
		if(remove(t_data->filename)==-1)
			printf("The file %s is not deleted...............\n",t_data->filename);
		else
			printf("The file %s is deleted...............\n",t_data->filename);
	}
	return temp_kf_fs_dif;
}


int send_first_chunks_client(struct Metadata *t_data, long int new_read_pos)
{
	int write_bytes=0;
	if (new_read_pos>0)
	{

		FILE *temp=fopen(t_data->filename,"rb");
		if(temp!=NULL)
		{

			fseek (temp,new_read_pos,SEEK_SET);


			long int pos=ftell(temp);
			u_char read_buf[(long int)t_data->current_file_size];
			memset(read_buf,'\0',t_data->current_file_size);
			fread(read_buf,1,(long int)t_data->current_file_size-pos,temp);
			write_bytes=write(t_data->mobile_sock,read_buf,(long int)t_data->current_file_size-pos);

			if(write_bytes<0)
				return -1;
		}

		fclose(temp);
	}
	return write_bytes;
}


int connect_server(char *host)
{


	struct sockaddr_in serveraddr;
	struct hostent hostbuf,*server;
	size_t hstbuflen;
	char *tmphstbuf;
	int res,server_sock,new_con,herr;

	hstbuflen = 1024;
	tmphstbuf = (char *)alloca (hstbuflen);
	res = gethostbyname_r (host, &hostbuf, tmphstbuf, hstbuflen, &server, &herr);
	if (server == NULL)
	{
		printf( "%s Error %s; no such host %s is found \n", __FUNCTION__,strerror(errno),host);
		return -1;
	}

	bzero((char *) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,(char *)&serveraddr.sin_addr.s_addr, server->h_length);
	serveraddr.sin_port = htons(80);

	server_sock= socket(AF_INET, SOCK_STREAM, 0);
	new_con=connect(server_sock,(const struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (new_con<0)
	{
		printf( "%s: Error %s; connecting to the radio server %s \n",__FUNCTION__, strerror(errno),host);
		return -1;
	}

	return server_sock;

}

int get_snew_socket(int client_sock,char* cbuf, int cbo)
{
	int server_sock=-1;
	char *host;
	host=(char*)malloc(2048);
	memset(host,'\0',2048);


	int begin=0,index=0;
	//read_bytes=read(client_sock,cbuf,5120);
	if(cbo>0)
	{
		if((char*)strstr(cbuf,"Host: ")!=NULL)
		{
			begin=cbo-strlen((char*)strstr(cbuf,"Host: "))+strlen("Host: ");
			while(cbuf[begin]!='\r')
			{
				host[index]=cbuf[begin];
				++index;
				++begin;
			}

			index=0,begin=0;
		}
		//printf("Server address...............................%s\n",host);
		server_sock=connect_server(host);

		if(server_sock>0)
			printf("Client Server Connected...............................%s\n",host);
		else
		{
			printf("Could no Connect to Server ...............................%s\n",host);
			return -1;
		}

	}
	return server_sock;
}


void *client_and_service(void *client_object)
{

	struct sockets* new_sock=(struct sockets*)client_object;
    int flv_response_check=0,youtube_request=0, write_continue=0,first_read=4096;
    int x, n, cbo=0, sbo=0, rec_bytes=0, throttle_count=0,begin=0;;
    char sbuf[MAX_BUF],cbuf[BUF_SIZE],old_string_request[2*BUF_SIZE];
    memset(sbuf,'\0',MAX_BUF);
    memset(cbuf,'\0',BUF_SIZE);
    memset(old_string_request,'\0',2*BUF_SIZE);

    struct pollfd fd1,fd2;


    fd_set R;
    int mobile_fd=new_sock->mobile_tcpproxy;
    int squid_fd=new_sock->tcpproxy_squid;


    int maxfd=mobile_fd > squid_fd ? mobile_fd : squid_fd;
    ++maxfd;
    long int temp_start=0,start_time=0,end_time=0;

    long long int temp_kf_fs_dif=0;
    double last_kf_time=0;
    FILE *g_fp;

    struct Metadata meta;
    set_http_reponse(&meta);
    convert_socket_nonblock(mobile_fd);

    while (1)
    {
		struct timeval to;
    	//struct timespec to;
		usleep(10000);
		if (cbo)
		{
			if ((strstr(cbuf,"videoplayback?")!=NULL))
			{

				memset(old_string_request,'\0',2*BUF_SIZE);
				memcpy(old_string_request,cbuf,cbo);
				youtube_request=1;
				new_sock->youtube_request=youtube_request;
				first_read=BUF_SIZE;
				meta.mobile_sock=mobile_fd;
				convert_socket_block(squid_fd);
			}

			if (write(squid_fd, cbuf,cbo) < 0 && errno != EWOULDBLOCK)
				break;
			else
			{
				memset(cbuf,'\0',BUF_SIZE);
				cbo=0;
			}
		}//writing back to the server socket


		if ((!youtube_request)&&(sbo))
		{
			rec_bytes=my_socket_write(mobile_fd, sbuf, &sbo);
			if ((rec_bytes < 0) && (errno != EWOULDBLOCK))
				break;

		}
		else if ((sbo>=first_read)&&(youtube_request))
		{

			if(flv_response_check)
			{
				if(g_fp!=NULL)
				{
					fwrite(sbuf,1,sbo,g_fp);
				}

				rec_bytes=my_socket_write(mobile_fd, sbuf, &sbo);
				if ((rec_bytes < 0) && (errno != EWOULDBLOCK))
					break;
				meta.received_bytes+=rec_bytes;

				begin=(long long int)(meta.received_bytes*8)/(long long int)(meta.audio_rate+meta.video_rate);


				if(begin>=FIXED_PLAY_PERIOD)
				{
					fclose(g_fp);

					if(meta.duration>MAX_PLAY_PERIOD)
						short_cut_decode(&meta,(double)begin,throttle_count,0);
					if((meta.last_kf_fs_dif<=8*1024)&&(meta.total_duration>meta.last_kf_time)&&(meta.duration>47000))
						write_continue=1;
					else
					{	g_fp=fopen(meta.filename,"a+");
						first_read=8192;
						goto sock_read;
					}
				}

				if((begin>=FIXED_PLAY_PERIOD)&&(write_continue))
				{

					end_time=get_time_dif(temp_start)/1000;
					meta.toal_download_time+=end_time;
					printf("Burst duration...............:%ldms\n",end_time);
					close(squid_fd);
					//if(meta.last_kf_time-end_time>=)


					if(remove(meta.filename)==-1)
						printf("The file %s is not deleted...............\n",meta.filename);
					else
						printf("The file %s is deleted...............\n",meta.filename);


					temp_kf_fs_dif=meta.last_kf_fs_dif;


					char *begin_part=(char*)malloc(30);
					char *part_one=(char*)malloc(BUF_SIZE);
					char *part_two=(char*)malloc(BUF_SIZE);
					memset(begin_part,'\0',30);
					last_kf_time=meta.last_kf_time;
					sprintf(begin_part,"&begin=%ld",meta.last_kf_time);

					if((throttle_count==0)&&(strstr(old_string_request,"Cookie:")!=NULL))
					{

						int read_bytes=strlen(old_string_request);
						int part_loc=read_bytes-strlen(strstr(old_string_request,"Cookie:"));
						memset(part_one,'\0',BUF_SIZE);
						memcpy(part_one,old_string_request,part_loc);
						strcpy(part_one+strlen(part_one),"\r\n\r\n");
						memset(old_string_request,'\0',2*BUF_SIZE);
						strcpy(old_string_request,part_one);
					}


					if(strstr(old_string_request,"&begin=")!=NULL)
					{

						int read_bytes=strlen(old_string_request);
						int part_loc=read_bytes-strlen(strstr(old_string_request,"&begin="));
						memset(part_one,'\0',BUF_SIZE);
						memcpy(part_one,old_string_request,part_loc);
						part_loc+=strlen("&begin=");
						while(1)
						{

							if(!isdigit((int)old_string_request[part_loc]))
								break;

							++part_loc;
							if(part_loc>read_bytes)
								break;
						}
						memset(part_two,'\0',BUF_SIZE);
						memset(part_two,'\0',read_bytes-part_loc+1);
						memcpy(part_two,old_string_request+part_loc,read_bytes-part_loc);
						memset(old_string_request,'\0',2*BUF_SIZE);

						strcpy(old_string_request,part_one);
						strcpy(old_string_request+strlen(old_string_request),begin_part);
						strcpy(old_string_request+strlen(old_string_request),part_two);


					}
					else
					{
						size_t part_loc=strlen(old_string_request)-strlen(strstr(old_string_request,"id="))
							+strlen("id=")+16;

						memset(part_one,'\0',BUF_SIZE);
						memcpy(part_one,old_string_request,part_loc);
						memcpy(part_one+strlen(part_one),begin_part,strlen(begin_part));

						memset(part_two,'\0',BUF_SIZE);
						memcpy(part_two,old_string_request+part_loc,strlen(old_string_request)-part_loc);
						memset(old_string_request,'\0',2*BUF_SIZE);
						strcpy(old_string_request,part_one);
						strcpy(old_string_request+strlen(old_string_request),part_two);
					}

					flv_response_check=0;
					first_read=BUF_SIZE;
					sbo=0;
					throttle_count+=1;

					long int player_buffer_time=meta.last_kf_time-meta.start_time;
					if((new_sock->begin_burst>0)&&(throttle_count%new_sock->begin_burst==0))
					{
						printf("Going to deep sleep and sent bytes...............................%lld\n",meta.received_bytes);
						fd1.fd=mobile_fd;
						fd1.events=POLL_OUT;
						//apply pool to frequently check the status of the socket.
						while(1)
						{
							//end_time=start_time+(get_micro_sec()/1000);
							end_time=get_time_dif(start_time)/1000;
							if((player_buffer_time-end_time)<=15000)
							{

								break;
							}
							else
							{
								sleep(1);


							}
						}
					}


					squid_fd=get_snew_socket(mobile_fd,old_string_request,strlen(old_string_request));

					if (write(squid_fd, old_string_request, strlen(old_string_request)) < 0 && errno != EWOULDBLOCK)
					{
						printf("Sent Directly...................%s:\n",old_string_request);
						break;
					}

					convert_socket_nonblock(squid_fd);
					meta.received_bytes=0;

					free(begin_part);
					free(part_one);
					free(part_two);
					goto sock_read;
				}
			}//end of else flv_check

			if(!flv_response_check)
			{
				int http_header_size=strlen(sbuf)-strlen(strstr(sbuf,"\r\n\r\n"))+strlen("\r\n\r\n");
				int flv_length=sbo-http_header_size;
				if ((parse_check_flv_metadata((u_char*)sbuf+http_header_size,"FLV",flv_length))
				&&(parse_check_flv_metadata((u_char*)sbuf+http_header_size,"onMetadata",flv_length)))
				{

					set_flv_metadata((u_char*)sbuf+http_header_size,&meta,flv_length);
					flv_response_check=1;
					first_read=8192;
					convert_socket_block(mobile_fd);
					av_register_all();
					char th_filename[35];
					memset(th_filename,'\0',35);
					sprintf(th_filename,"%d_throttle_%d.flv",mobile_fd,throttle_count);
					memset(meta.filename,'\0',255);
					memcpy(meta.filename,th_filename,strlen(th_filename));
					meta.stream_info=-1;
					meta.received_bytes+=sbo-http_header_size;
					printf("Newly created file................%s\n",th_filename);


					FILE *fp=fopen(meta.filename,"w");
					if(fp!=NULL)
					{
						fwrite(sbuf+http_header_size,1,sbo-http_header_size,fp);
						g_fp=fp;

					}

					if(throttle_count==0)
					{
						//start_time=meta.start_time*1000;

						start_time=get_cur_time();
						temp_start=start_time;
						//printf("Starting time.........................%ld\n",start_time);
						meta.received_bytes+=sbo-http_header_size;
						rec_bytes=my_socket_write(mobile_fd, sbuf, &sbo);
						if ((rec_bytes < 0) && (errno != EWOULDBLOCK))
							break;
					}
					else
					{
						temp_start=get_cur_time();
						fclose(g_fp);
						get_first_kframe_index(&meta);
						long int new_pos=meta.first_kf_pos+temp_kf_fs_dif;
						int bytes=write(mobile_fd,sbuf+http_header_size+new_pos,sbo-http_header_size-new_pos);
						if(bytes>0)
						{
							printf("%s:Extra Bytes Received:%lld....sbo:%d \n",meta.filename,meta.first_kf_pos+temp_kf_fs_dif,sbo);

						}
						printf("%s, Sent the beginning bytes..............\n",meta.filename);
						meta.received_bytes+=bytes;
						g_fp=fopen(meta.filename,"a+");
						memset(sbuf,'\0',MAX_BUF);
						sbo=0;
					}

				}//end of setting flv metadata
			}//end of flv_check_
		}

sock_read:
		/* Reading from the sockets*/
		FD_ZERO(&R);
		if (cbo < BUF_SIZE)
			FD_SET(mobile_fd, &R);
		if (sbo < first_read)
			FD_SET(squid_fd, &R);
		to.tv_sec = 0;
		to.tv_usec = 100;
		x = select(maxfd+1, &R, 0, 0, &to);
		if (x > 0)
		{
			if (FD_ISSET(mobile_fd, &R))  //reading form the client socket
			{
				//if(BUF_SIZE-)
				n = read(mobile_fd, cbuf+cbo, BUF_SIZE-cbo);
				if (n > 0)
					cbo += n;



				if((n==0)||((n<0)&& errno!=EWOULDBLOCK))
				{
					if(cbo>0)
						rec_bytes=my_socket_write(squid_fd,cbuf,&cbo);

					if(youtube_request)
					{
						if(remove(meta.filename)==-1)
							printf("The file %s is not deleted...............\n",meta.filename);
						else
							printf("The file %s is deleted...............\n",meta.filename);
					}

					break;
				}
			}


			if (FD_ISSET(squid_fd, &R)) // reading from the server socket
			{
				n = read(squid_fd,sbuf+sbo,first_read);

				if (n > 0)
				{

					sbo += n;
				}
				if((n==0)||((n<0)&& errno!=EWOULDBLOCK))
				{
					if(sbo>=0)
					{

						if(youtube_request)
						{

							meta.received_bytes+=sbo;
							if(sbo>0)
								rec_bytes=my_socket_write(mobile_fd,sbuf,&sbo);
							if(g_fp!=NULL)
							{
								fwrite(sbuf,1,sbo,g_fp);
								fclose(g_fp);
							}
							if(remove(meta.filename)==-1)
								printf("The file %s is not deleted...............\n",meta.filename);
							else
								printf("The file %s is deleted...............\n",meta.filename);

							printf("Total elapsed time ...................%ld\n",meta.toal_download_time);
							//short_cut_decode(&meta,0.0,throttle_count);

							//short_cut_decode(&meta,0.0,throttle_count,1);
							//send_big_chunks_client(&meta,temp_kf_fs_dif);

						}
						else
						{
							rec_bytes=my_socket_write(mobile_fd,sbuf, &sbo);
							if ((rec_bytes < 0) && (errno != EWOULDBLOCK))
								break;

						}
					}
					break;

				}
			}
		}
		else if (x < 0 && errno != EINTR)
			break;

    }//end of while
sock_close:
	close(mobile_fd);
	close(squid_fd);
	return NULL;
}




JNIEXPORT void JNICALL Java_com_rumana_youtubeextreme_OpenYouTubePlayerActivity_startAccelerator(JNIEnv *pEnv, jobject pObj, jint port, jint burst, jint slp)
{
	int server_port,on=1;

	server_port=(int)port;
	int listen_fd,client_sock,server_sock;
	struct sockaddr_in serv_addr,cli_addr;
	socklen_t cli_len;
	int optval = 1;
	socklen_t optlen = sizeof(optval);


	listen_fd=socket(AF_INET,SOCK_STREAM,0);

	/* Check the status for the keepalive option */
	if(getsockopt(listen_fd, SOL_SOCKET, SO_KEEPALIVE, &optval, &optlen) < 0) {
	  perror("getsockopt()");
	  close(listen_fd);
	  exit(EXIT_FAILURE);
	}
	LOGI(10, "%s:SO_KEEPALIVE is %s\n",__FUNCTION__, (optval ? "ON" : "OFF"));

	/* Set the option active */
	if(setsockopt(listen_fd, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0)
	{
	  LOGE(1, "setsockopt()");
	  close(listen_fd);
	  return;
	}
	LOGI(10,"%s:SO_KEEPALIVE set on socket\n",__FUNCTION__);

	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, 1024);
	bzero(&serv_addr,sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(server_port);

	int rt=bind(listen_fd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	if (rt < 0)
	   LOGE(1, "bind %s:%d", inet_ntoa(serv_addr.sin_addr), server_port);

	rt = listen(listen_fd, 1024);
	if (rt < 0)
	   LOGE(1, "listen %s:%d", inet_ntoa(serv_addr.sin_addr), server_port);

	struct sockets client_server;
	/*--- begin waiting for connections ---*/

	pthread_t parent_con;



	client_sock=-1;
	server_sock=-1;
	/*
	client_sock=open_squid_port();

	printf("Server is ready to accept the connections...........................\n");

	if(client_sock<0)
	{
		printf("SQuid is not ready.............................\n");
		exit(0);
	}*/
	while (1)                         /* process all incoming clients */
	{
		usleep(100000);
		cli_len=sizeof(cli_addr);
		bzero(&cli_addr,cli_len);
		client_sock = accept(listen_fd,(struct sockaddr *)&cli_addr,&cli_len);
		if(client_sock<0)
		{
			if(errno==EWOULDBLOCK)
				continue;
		}
		else
		{

			LOGI(10,"Proxy Server Has Detected One Connection at port: %d, "
									   "From Address: %s, From Port:%d \n",server_port,
										inet_ntoa(cli_addr.sin_addr),ntohs(cli_addr.sin_port));

			server_sock = open_squid_port();
			if(server_sock<0)
			{
				LOGE(1,"Ssquid is not ready to accept the connection..............................\n");
				close(client_sock);
				continue;
			}
		}

		if(server_sock>0)
		{
			bzero(&client_server,sizeof(client_server));
			client_server.mobile_tcpproxy=client_sock;
			client_server.tcpproxy_squid=server_sock;
			client_server.begin_burst=(int)burst;
			client_server.sleep_time=(int)slp;
			pthread_create(&parent_con, 0,client_and_service, (void*)&client_server);
			pthread_detach(parent_con);
		}
	}//end of while

	//return 0;
}

