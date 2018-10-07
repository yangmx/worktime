#ifndef _BEAN_H
#define _BEAN_H

#include <time.h>

typedef struct wt_day
{
	short date;
	unsigned char time_len;
} s_wt_day;

typedef struct task_ele
{
	int seq;
	int par_seq;
	time_t time;
	unsigned char state;//0-未完成，1-完成
	char* comment;
	s_wt_day** wt_days;
	unsigned char wt_days_len;

} s_task_ele;

typedef struct worktime
{
	char * version;
	int sequence;
	s_task_ele** task_eles;
	int task_eles_len;
} s_worktime;

#endif
