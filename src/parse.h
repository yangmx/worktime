#ifndef _PARSE_H
#define _PARSE_H

#include "bean.h"

s_worktime* parse_worktime(unsigned char* buffer,unsigned long total_size);
s_task* parse_task(unsigned char* buffer,unsigned long * begin_pos,unsigned long total_size);

#endif
