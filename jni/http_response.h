
#include "interface.h"
/*
 * http_response.h
 *
 *  Created on: Sep 3, 2011
 *      Author: mhoque
 */

#ifndef HTTP_RESPONSE_H_
#define HTTP_RESPONSE_H_

	struct KF_data
	{
		double kf_pos;
		double kf_size;
		double kf_time;
	};
	struct BP_data{
		int bee_pee_fr_size;
		double bee_pee_fr_pos;
		double bee_pee_fr_time;
	};

struct Metadata
{

	int mobile_sock;
	char file_id[255];
	char filename[255];
	long int audio_rate;
	long int video_rate;
	int frame_rate;

	long int total_data_rate;
	long int duration;
	long int total_duration;
	long int dub_start_time;
	long int byte_length;

	long int start_time;
	double start_length;

	long int frame_size;
	int video_height;
	int video_width;
	long int toal_download_time;

	//int response_code;
	//double content_length;
	long long int received_bytes;
	//long long int total_received_bytes;

	long long int current_file_size;
	long long int last_kf_index;
	long int last_kf_time;
	long int first_kf_time;
	long int last_kf_fs_dif; //current different between key frame position and file size
	long long int last_kf_pos_dif;
	long int last_kf_size;

	long int first_kf_pos;

	int stream_info;
	//double extra_bytes;
	//int discard_meta_bytes;
	//int bytes_per_msec;
	//int flv_num_files;
	//int flv_com_files;
	//int flv_file_merge_complete;
	//long int estm_rate;


};


void set_http_reponse(struct  Metadata* meta);
void parse_set_http_response(const char* response, struct Metadata* meta);
void set_flv_metadata(u_char* metadata,struct Metadata* meta,int flv_length);
int set_non_metadata_length(u_char* non_metadata,int length,struct Metadata* meta);
int parse_check_flv_metadata(u_char* metadata, const char* element,int flv_length);
int get_video_discard_length();

int get_http_response_code();
int get_http_content_type();
int get_audio_rate();
int get_video_rate();
int get_data_rate();
int get_frame_rate();
int get_video_duration();
void update_received_bytes(int bytes);
long int get_content_length();
long int get_start_time();
int get_http_param_status();
long int convert_received_bytes_msec();
long int convert_msec_bytes(long int msec);
long int convert_bytes_msec(long int bytes);
int get_extra_bytes_length();
double get_received_bytes();
void update_extra_bytes(int bytes);
void dump_metadata_info();


#endif /* HTTP_RESPONSE_H_ */
