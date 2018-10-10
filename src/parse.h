#ifndef _PARSE_H
#define _PARSE_H

#include "bean.h"

unsigned long get_file_size(char* file_path);

s_worktime* parse_worktime(char* file_path);
s_task* parse_task(unsigned char* buffer,unsigned long * begin_pos,unsigned long total_size);

unsigned int parse_input_sequence(char* seq_no);
unsigned int parse_input_duration(char* work_time);

#endif
