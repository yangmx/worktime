#ifndef _UTIL_H
#define _UTIL_H

#include <stdio.h>
#include <stdlib.h>

unsigned long get_file_size(char*);
unsigned char* simply_int(int,  int*);
void fwrite_sequence(int , FILE *);
void fwrite_of_long(long , FILE *);
void fwrite_of_unsigned_char(unsigned char, FILE *);
void fwrite_of_short(short , FILE *);

#endif
