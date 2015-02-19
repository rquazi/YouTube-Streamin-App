/*
 * youtube-extreme.h
 *
 *  Created on: Sep 5, 2011
 *      Author:quazi
 */


#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/times.h>
#include <poll.h>
//#include <linux/time.h>

//#include <linux/time.h>
#include <string.h>
#include "interface.h"
#include<libavutil/avutil.h>

#include<libavformat/avformat.h>
#include<libavcodec/avcodec.h>
#include<libavutil/avutil.h>
//#include<libavutil/mathematics.h>


#ifndef YOUTUBE_EXTREME_H_
#define YOUTUBE_EXTREME_H_


long int get_micro_sec()
{

	struct timeval tv;
	long int total;

	int rt=gettimeofday(&tv, NULL);
	if(rt==0)
	{
		total = tv.tv_sec*1000000+tv.tv_usec;
	}
	else
	{	 printf("gettimeofday() failed, errno = %d\n", errno);
		total=0;
	}
	return total;
}

long int get_cur_time()
{
	struct timespec begin;
	long int total=0;
	clock_gettime(CLOCK_REALTIME,&begin);
	total=begin.tv_sec*1000000+(begin.tv_nsec/1000);
	return total;
}

long int get_time_dif(long int old_time)
{
	struct timespec end;
	long int dif,total;

	clock_gettime(CLOCK_REALTIME,&end);
	total=end.tv_sec*1000000+(end.tv_nsec/1000);
	dif=total-old_time;
	return dif;

}

void convert_socket_block(int sock)
{
	int flags, f_flags;
	flags=fcntl(sock,F_GETFL,0);
	if(flags<0)
	{
		printf("%s Error %s \n",__FUNCTION__,strerror(errno));

	}

	flags&=~O_NONBLOCK;

	f_flags=fcntl(sock,F_SETFL,flags);
	if(flags<0)
	{
		printf("%s Error %s \n",__FUNCTION__,strerror(errno));

	}



}

void convert_socket_nonblock(int sock)
{
	int flags, f_flags;
	flags=fcntl(sock,F_GETFL,0);
	if(flags<0)
	{
		printf("%s Error %s \n",__FUNCTION__,strerror(errno));
	}

	flags|=O_NONBLOCK;

	f_flags=fcntl(sock,F_SETFL,flags);
	if(flags<0)
	{

		printf("%s Error %s \n",__FUNCTION__,strerror(errno));
	}
}

int my_socket_write(int fd, char *buf, int *len)
{
	int x = write(fd, buf, *len);
	if (x < 0)
		return x;
	if (x == 0)
		return x;
	if (x != *len)
		memmove(buf, buf+x, (*len)-x);
	*len -= x;
	return x;
}



int local_video_sanity_request(const char* request)
{
	//printf("%s.....\n%s",__FUNCTION__,request);
	size_t start_loc=strlen(strstr(request,"videoplayback?sparams="));
	printf("%s...............%d\n",__FUNCTION__,start_loc);
	if((int)start_loc>=0)
		return 1;
	else
		return 0;

}

int endof_http_header(const char* request)
{
	//printf("%s.....\n%s",__FUNCTION__,request);
	size_t start_loc=strlen(strstr(request,"\r\n\r\n"));
	if((int)start_loc>=0)
		return 1;
	else
		return 0;
}



int local_video_sanity_response(const char* response)
{

	size_t video_loc=strlen(strstr(response,"Content-Type: video/x-flv"));
	if ((int)video_loc>=0)
	{
		return 1;
	}
	video_loc=strlen(strstr(response,"content-type:video/x-flv"));
	if((int)video_loc>=0)
	{

		return 1;
	}
	video_loc=strlen(strstr(response,"Content-Type:Video/x-flv"));
	if((int)video_loc>=0)
	{
		return 1;
	}

	return 0;
}







#endif /* YOUTUBE_EXTREME_H_ */
