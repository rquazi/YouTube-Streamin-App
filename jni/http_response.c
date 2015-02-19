/*
 * httpreponse.cpp
 *
 *  Created on: Aug 21, 2011
 *      Author: quazi





      *Beginning of HTTP header


****  HTTP/1.1 200 OK
****  Last-Modified: Sun, 14 Aug 2011 00:26:44 GMT
****  Content-Type: video/x-flv
****  Date: Tue, 16 Aug 2011 13:02:59 GMT
****  Expires: Tue, 16 Aug 2011 13:02:59 GMT
****  Cache-Control: private, max-age=24721
****  Accept-Ranges: bytes
****  Content-Length: 25590510
****  Connection: close
****  X-Content-Type-Options: nosniff
****  Server: gvs 1.0

	  *End of HTTP header

	  *Beginning of data content, which  contains video information

****  FLVK
****  onMetaDataduration@o$starttimetotalduration@o$width@height@vvideodatarate@h&U]audiodatarate@`5Vtotaldatarate@P1>framerate@7|
****  bytelengthAxgcanseekontime

      *End of the the data content
*/


/*Actual response format: 325(http header)+292+23+9+128+9+128+19+256+y*/

#include "http_response.h"

void set_http_reponse(struct Metadata* mdata)
{
	// TODO Auto-generated constructor stub


	mdata->audio_rate=0;
	mdata->video_rate=0;
	mdata->frame_rate=0;

	mdata->total_data_rate=0;
	mdata->duration=0;
	mdata->total_duration=0;
	mdata->start_time=0;
	mdata->byte_length=0;
	mdata->video_height=0;
	mdata->video_width=0;
	mdata->last_kf_time=0;
	mdata->first_kf_time=0;
	//mdata->response_code=0;
	//mdata->content_length=0.0;
	mdata->received_bytes=0.0;
	mdata->toal_download_time=0;
	//mdata->bytes_per_msec=0.0;
	//mdata->flv_num_files=0;
	//mdata->flv_com_files=0;
	//mdata->flv_file_merge_complete=0;
}

/*
void parse_set_http_response(const char* response,struct Metadata* mdata)
{
	int begin=0;
	size_t loc=strlen(response)-strlen(strstr(response,"200 OK"))+strlen("200 OK");
	if((int)loc>=0)
		mdata->response_code=200;
	else
		mdata->response_code=0;



	char length[CONTENT_LENGTH];
	memset(length,'\0',CONTENT_LENGTH);

	loc=strlen(response)-strlen(strstr(response,"Content-Length: "))+strlen("Content-Length: ");
	while(response[loc]!='\r')
	{
		length[begin]=response[loc];
		++begin;
		++loc;
	}

	mdata->content_length=atof((const char*)length);
}
*/

int parse_check_flv_metadata(u_char *metadata,const char* element,int flv_length)
{
	int i=0, pos=-1,length=strlen(element);
	u_char temp[length];

	for(i=0;i<(flv_length-length);++i)
	{

		memset(temp,'\0',length);
		memcpy(temp,metadata+i,length);
		if(memcmp(temp,element,length)==0)
		{
			pos=i+length;
			break;
			//return pos;
		}
	}
	return pos;
}


double flv_int_double(uint64_t v)
{
     if(v+v > 0xFFEULL<<52)
         return 0.0/0.0;
     return ldexp(((v&((1LL<<52)-1)) + (1LL<<52)) * (v>>63|1), (v>>52&0x7FF)-1075);
 }





uint64_t parse_flv_metadata(u_char *metadata, const char* element,int *ele_type,int flv_length)
{
		int pos=-1, ele_length=0;
		uint64_t val;

		uint8_t metadata_value[8];

		pos=parse_check_flv_metadata(metadata,element,flv_length);
		*ele_type=(int)metadata[pos];

		if(*ele_type==FLV_DATA_TYPE_NUMBER)
		{
			ele_length=8;
			memcpy(metadata_value,metadata+pos+1,ele_length);
			val=(uint64_t)metadata_value[0]<<56;
			val|=(uint64_t)metadata_value[1]<<48;
			val|=(uint64_t)metadata_value[2]<<40;
			val|=(uint64_t)metadata_value[3]<<32;
			val|=(uint64_t)metadata_value[4]<<24;
			val|=(uint64_t)metadata_value[5]<<16;
			val|=(uint64_t)metadata_value[6]<<8;
			val|=(uint64_t)metadata_value[7];


		}
		else if(*ele_type==FLV_DATA_TYPE_BOOL)
		{
			ele_length=1;
			val=(int)metadata[pos+1];
		}

		//printf("%s: exiting................\n",__FUNCTION__);
	return val;
}



void set_flv_metadata(u_char* metadata,struct Metadata* meta,int flv_length)
{


	char metadata_elements[]="- duration - totalduration height width audiodatarate videodatarate "
			"totaldatarate bytelength starttime framerate.";

	int element_type=0;
	double number_value=0.0;


	char *token=strtok(metadata_elements," ,.-");
	while(token!=NULL)
	{
		number_value=parse_flv_metadata(metadata,token,&element_type,flv_length);


		if(!strcmp(token,"audiodatarate"))
		{
				meta->audio_rate=(long int)flv_int_double(number_value)*1024;
				printf("%s..........................:%ld\n",token,meta->audio_rate);
		}
		else if(!strcmp(token,"videodatarate"))
		{
			meta->video_rate=(long int)flv_int_double(number_value)*1024;
			printf("%s..........................:%ld\n",token,meta->video_rate);

		}
		else if(!strcmp(token,"totaldatarate"))
		{
			meta->total_data_rate=(long int)flv_int_double(number_value)*1024;
			printf("%s..........................:%ld\n",token,meta->total_data_rate);

		}
		else if(!strcmp(token,"framerate"))
		{
			meta->frame_rate=(int )flv_int_double(number_value);
			printf("%s..........................:%d\n",token,meta->frame_rate);

		}
		else if(!strcmp(token,"bytelength"))
		{
			meta->byte_length=(long int)flv_int_double(number_value);
			printf("%s..........................:%ld\n",token,meta->byte_length);

		}
		else if(!strcmp(token,"starttime"))
		{
			meta->start_time=(long int)flv_int_double(number_value);
			printf("%s..........................:%ld\n",token,meta->start_time);
		}
		else if(!strcmp(token,"duration"))
		{
			meta->duration=(long int)flv_int_double(number_value)*1000;
			printf("%s..........................:%ld\n",token,meta->duration);

		}
		else if(!strcmp(token,"totalduration"))
		{
			meta->total_duration=(long int)flv_int_double(number_value)*1000;
			printf("%s..........................:%ld\n",token,meta->total_duration);

		}
		else if(!strcmp(token,"height"))
		{
			meta->video_height=(int)flv_int_double(number_value);
			printf("%s..........................:%d\n",token,meta->video_height);

		}
		else if(!strcmp(token,"width"))
		{
			meta->video_width=(int)flv_int_double(number_value);
			printf("%s..........................:%d\n",token,meta->video_width);

		}


		token=strtok(NULL, " ,.-");
	}

	//printf("%s: exiting................\n",__FUNCTION__);


	//meta->start_time=(long long int)meta->dub_start_time;

}


/*

int set_non_metadata_length(u_char* non_metadata,int length,struct Metadata* meta)
{
	u_char common_bytes[]="\xaf\x00\x12\x10\x00\x00\x00\x0f";
	int bytes_length=8,i=0,pos=0;
	u_char temp[bytes_length];
	for(i=0;i<length-bytes_length;++i)
	{
		memset(temp,'\0',bytes_length);
		memcpy(temp,non_metadata+i,bytes_length);
		if(memcmp(temp,common_bytes,bytes_length)==0)
		{
			pos=i+bytes_length;
			break;
			//return pos;
		}
	}

	//meta->discard_meta_bytes=FLV_METADATA_SIZE+OTHER_METADATA_SIZE+pos;

	//this->fixed_meta_nonmeta=this->reponse.length()+FLV_METADATA_SIZE+OTHER_METADATA_SIZE+pos;
	return FLV_METADATA_SIZE+OTHER_METADATA_SIZE+pos;
}








long int convert_received_bytes_msec(struct Metadata* meta)
{
	long int msec=meta->received_bytes/meta->bytes_per_msec;
	return msec;
}*/



void dump_metadata_info(struct Metadata* meta )
{

	//printf("Content Length......%f\n",meta->content_length);
	//printf("Video Duration......%f\n",meta->duration);
	//printf("Total Video Duration......%f\n",meta->total_duration);

	/*
	printf("Start Length ....%f\n",meta->start_length);
	printf("Byte Length ....%f\n",meta->byte_length);
	printf("Received Bytes ....%ld\n",(long int)meta->current_file_size);
	printf("Extra Received Bytes ....%ld\n",(long int)meta->extra_bytes);

	printf("Current kf and Fs difference ....%ld\n",(long int)meta->last_kf_fs_dif);
	printf("Requested Start Time ....%f\n",(meta->last_kf_time));
	printf("Start Time ....%f\n",(meta->dub_start_time*1000));
	printf("Bytes per milli second ....%d\n",meta->bytes_per_msec);
	printf("First key frame position....%f",meta->first_kf_pos);
	printf("dicarded bytes....%d",meta->discard_meta_bytes);*/
	//printf("Content Length......%f\n",meta->content_length);
	//printf("audio rate......%f\n",meta->audio_rate);
	//printf("video rate......%f\n",meta->video_rate);
	//printf("total rate......%f\n",meta->total_data_rate);
	//printf("total rate (audio+video)......%ld\n",(long int)(meta->video_rate+meta->audio_rate));
	//meta->estm_rate=(long int)meta->total_data_rate;
	//printf("Estimated total rate:%ld\n",meta->estm_rate);
	//printf("Video Duration......%f\n",meta->duration);

	printf("\n\n");


}

