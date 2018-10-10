#ifndef _UTIL_H
#define _UTIL_H

#include <stdio.h>
#include <stdlib.h>

typedef struct print_row{
	int len;
	char** char_ary;
} s_print_row;

unsigned char* simply_int(int src_int,  int* simply_len);

char* parse_buffer_string(unsigned char* buffer,unsigned long * begin_pos,unsigned long total_size);
unsigned int parse_buffer_int_simply(unsigned char* buffer,unsigned long * begin_pos,unsigned long total_size);

void fwrite_int_simply(int var, FILE * fp);
char* int2str(unsigned int prev_seq,int min_len);

char* concat_string(char** str_ary, int len);
char* creat_space_string(int len);

void print_row_ary(s_print_row** two_level_char_ary,int size);
void print_space(int len);
#endif
