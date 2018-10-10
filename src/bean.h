#ifndef _BEAN_H
#define _BEAN_H

#include <time.h>

#define WORKTIME_FILENAME "worktime.wt"
#define HISTORY_DIR ".wt_histories"
#define BACKUP_DIR ".wt_backups"

typedef struct task_detail
{
	short date;
	char* title;
	unsigned char cost;
} s_task_detail;

typedef struct task
{
	unsigned int seq;
	unsigned int par_seq;
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
	unsigned int sequence;
	s_task** tasks;
	unsigned int tasks_len;
} s_worktime;

#endif
