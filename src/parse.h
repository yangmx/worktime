#ifndef _PARSE_H
#define _PARSE_H

#include <stdio.h>

#include "bean.h"

s_worktime* parse_all(char*);
s_task_ele* parse_task_eles(unsigned char* ,unsigned long *,unsigned long );
char* parse_string(unsigned char* ,unsigned long *,unsigned long );
int parse_sequence(unsigned char* ,unsigned long *,unsigned long );

#endif
