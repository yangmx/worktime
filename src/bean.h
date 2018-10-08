#ifndef _BEAN_H
#define _BEAN_H

#include <time.h>

typedef struct task_detail
{
	short date;
	unsigned char cost;
} s_task_detail;

typedef struct task
{
	int seq;
	int par_seq;
	char* title;
	s_task_detail** task_details;
	unsigned char task_details_len;
	unsigned char state;//0-未完成，1-完成
	time_t begin_time;
	time_t end_time;

} s_task;

typedef struct worktime
{
	char * version;
	int sequence;
	s_task** tasks;
	int tasks_len;
} s_worktime;

#endif
