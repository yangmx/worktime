#include "util.h"

#define BUFFER_SIZE 4096

unsigned long get_file_size(char* file_path){
	FILE* fp;
	if((fp = fopen(file_path,"rb+")) == NULL){
		fprintf(stderr,"open %s file fail", file_path);
		exit(0);
	}
	unsigned char buffer[BUFFER_SIZE];
	unsigned long size = 0,temp_size = 0;
	while(!feof(fp)){
		temp_size = fread(buffer,1,BUFFER_SIZE,fp);
		size += temp_size;
	}

	fclose(fp);
	return size;
}

unsigned char* simply_int(int src_int,  int* simply_len){
	unsigned char* bytes;
	if(src_int < 0x0100){
		bytes = (unsigned char*)malloc(sizeof(unsigned char));
		bytes[0] = (unsigned char)src_int;
		*simply_len=1;
	}else if(src_int < 0x010000){
		bytes = (unsigned char*)malloc(sizeof(unsigned char) * 2);
		bytes[0] = (unsigned char)(src_int >> 8);
		bytes[1] = (unsigned char)(src_int);
		*simply_len=2;
	}else if(src_int < 0x01000000){
		bytes = (unsigned char*)malloc(sizeof(unsigned char) * 3);
		bytes[0] = (unsigned char)(src_int >> 16);
		bytes[1] = (unsigned char)(src_int >> 8);
		bytes[2] = (unsigned char)(src_int);
		*simply_len=3;
	}else{
		bytes = (unsigned char*)malloc(sizeof(unsigned char) * 4);
		bytes[0] = (unsigned char)(src_int >> 24);
		bytes[1] = (unsigned char)(src_int >> 16);
		bytes[2] = (unsigned char)(src_int >> 8);
		bytes[3] = (unsigned char)(src_int);
		*simply_len=4;
	}
	return bytes;
}

char* parse_buffer_string(unsigned char* buffer,unsigned long * begin_pos,unsigned long total_size){
	return "123";
}

int parse_buffer_int_simply(unsigned char* buffer,unsigned long * begin_pos,unsigned long total_size){
	return 0;
}

void fwrite_int_simply(int var, FILE * fp){
	int len;
	unsigned char* bytes = simply_int(var,&len);
	fwrite(bytes,1,len,fp);
	free(bytes);
}
