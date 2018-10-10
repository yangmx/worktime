#include <string.h>
#include <io.h>

#include "util.h"

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
	int len = 0;
	for(int i=*begin_pos;i<total_size;i++){
		len ++;
		if(buffer[i] == '\0'){
			break;
		}
	}
	char* str = (char*)malloc(sizeof(char) * len);
	memcpy(str,(unsigned char*)(buffer + *begin_pos),len);
	*begin_pos = *begin_pos + len;
	if(*begin_pos >= total_size){
		*begin_pos = -1;
	}
	return str;
}

unsigned int parse_buffer_int_simply(unsigned char* buffer,unsigned long * begin_pos,unsigned long total_size){
	int len = 0;
	int rtn = 0;
	for(int i=*begin_pos;i<total_size;i++){
		len ++;
		if(buffer[i] == '\0'){
			break;
		}
		rtn = (rtn << 8) + buffer[i];
	}
	*begin_pos = *begin_pos + len;
	if(*begin_pos >= total_size){
		*begin_pos = -1;
	}
	return rtn;
}

void fwrite_int_simply(int var, FILE * fp){
	int len;
	unsigned char* bytes = simply_int(var,&len);
	fwrite(bytes,1,len,fp);
	free(bytes);
}

char* int2str(unsigned int prev_seq,int min_len){
	int max_len = strlen("4294967295");// int最大值
	if(min_len > max_len){
		min_len = max_len;
	}
	int len = 0;
	int temp_value = prev_seq;
	do{
		temp_value = temp_value/10;
		len++;
	}while(temp_value > 0);

	if(len < min_len){
		len = min_len;
	}

	char* result = (char*)malloc(len + 1); // int最大值
	int mod,pos=len-1;
	temp_value = prev_seq;
	do{
		if(temp_value > 0){
			mod = temp_value % 10;
			temp_value = temp_value / 10;
			result[pos--] = mod + '0';
		}else{
			result[pos--] = '0';
		}
	}while(pos >= 0);
	result[len] = '\0';

	return result;
}

char* concat_string(char** str_ary, int len){
	int max_len = 0;
	for(int i=0;i<len;i++){
		max_len += strlen(str_ary[i]);
	}
	char* new_str = (char*)malloc(max_len + 1);
	memcpy(new_str,str_ary[0],strlen(str_ary[0]) + 1);
	for(int i=1;i<len;i++){
		strcat(new_str,str_ary[i]);
	}
	return new_str;
}

void print_row_ary(s_print_row** two_level_char_ary,int size){
	s_print_row* temp_row;
	// 计算总行数
	int max_col_num = 0, temp_col_num, temp_col_size;
	int max_len_per_col[] = {0,0,0,0,0,0,0,0,0,0};
	char** rows;
	for(int i=0;i<size;i++){
		temp_row = two_level_char_ary[i];
		temp_col_num = temp_row->len;
		// 列数超过10行，则不再显示。
		if(temp_col_num >= 10){
			temp_col_num = 10;
		}
		// 设置最大列数
		if(temp_col_num > max_col_num){
			max_col_num = temp_col_num;
		}
		rows = temp_row->char_ary;
		// 计算每列最大值
		for(int j=0;j<temp_col_num;j++){
			if(j >= 10){
				break;
			}
			temp_col_size = strlen(rows[j]);
			if(max_len_per_col[j] < temp_col_size){
				max_len_per_col[j] = temp_col_size;
			}
		}
	}
	int space_num;
	// 格式化输出
	for(int i=0;i<size;i++){
		temp_row = two_level_char_ary[i];
		temp_col_num = temp_row->len;
		rows = temp_row->char_ary;
		if(temp_col_num >= 10){
			temp_col_num = 10;
		}
		// 计算每列最大值
		for(int j=0;j<temp_col_num-1;j++){
			temp_col_size = strlen(rows[j]);
			printf("%s",rows[j]);
			space_num = max_len_per_col[j] - temp_col_size;
			// 多输几个空白
			print_space(space_num + 2);
		}
		printf("%s\n",rows[temp_col_num-1]);
		fflush(stdout);
	}
}

char* creat_space_string(int len){
	char* new_string = (char*)malloc(sizeof(char)*(len + 1));
	for(int i=0;i<len;i++){
		new_string[i] = ' ';
	}
	new_string[len] = '\0';
	return new_string;
}

void print_space(int len){
	for(int i=0;i<len;i++){
		printf(" ");
	}
}
