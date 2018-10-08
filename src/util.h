#ifndef _UTIL_H
#define _UTIL_H

#include <stdio.h>
#include <stdlib.h>

unsigned long get_file_size(char* file_path);
unsigned char* simply_int(int src_int,  int* simply_len);

char* parse_buffer_string(unsigned char* buffer,unsigned long * begin_pos,unsigned long total_size);
int parse_buffer_int_simply(unsigned char* buffer,unsigned long * begin_pos,unsigned long total_size);

void fwrite_byte_ary(unsigned char * bytes, FILE * fp);
void fwrite_int_simply(int var, FILE * fp);
void fwrite_long(long var, FILE * fp);
void fwrite_short(short var, FILE * fp);

#endif
