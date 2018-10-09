#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bean.h"
#include "save.h"
#include "util.h"

void fwrite_worktime(char* file_path, s_worktime* worktime){
	FILE* fp;
	if((fp = fopen(file_path,"wb+")) == NULL){
		fprintf(stderr,"open %s file fail\n", file_path);
		exit(0);
	}
	// 写版本号
	fwrite(worktime->version,1,strlen(worktime->version),fp);
	fputc('\0',fp);

	// 写序列号
	fwrite_int_simply(worktime->sequence,fp);
	fputc('\0',fp);

	// 遍历写任务
	for(int i=0,j=worktime->tasks_len;i<j;i++){
		fwrite_task((worktime->tasks)[i],fp);
	}

	fclose(fp);
}

void fwrite_task(s_task * task, FILE * fp){
	// 写当前序号
	fwrite_int_simply(task->seq,fp);
	fputc('\0',fp);

	// 写父序号
	if(task->par_seq != 0){
		fwrite_int_simply(task->par_seq,fp);
	}
	fputc('\0',fp);

	// 写标题
	fwrite(task->title,1,strlen(task->title) + 1,fp);

	// 写状态
	fputc(task->state,fp);

	// 写日期
	long temp_time = task->begin_time;
	fputc(temp_time >> 24,fp);
	fputc(temp_time >> 16,fp);
	fputc(temp_time >> 8,fp);
	fputc(temp_time,fp);

	temp_time = task->end_time;
	fputc(temp_time >> 24,fp);
	fputc(temp_time >> 16,fp);
	fputc(temp_time >> 8,fp);
	fputc(temp_time,fp);

	// 写工时数量
	fputc(task->task_details_len,fp);

	int date;
	for(int i=0,j=task->task_details_len;i<j;i++){
		// 写工时日期
		date = (task->task_details)[i]->date;
		fputc(date >> 8,fp);
		fputc(date,fp);
		// 写工时时间
		fputc((task->task_details)[i]->cost,fp);
	}
}

