#ifndef _WORKTIME_H
#define _WORKTIME_H

#include "bean.h"

void add(char** );
void list(char**);
void save_all(char*, s_worktime*);
void fwrite_s_task_ele(s_task_ele * , FILE *);

#endif
