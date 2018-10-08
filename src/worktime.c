#define WORKTIME_FILENAME "worktime.wt"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "worktime.h"
#include "save.h"

int main(int argc, char** argv){
	if(argc > 1){
		if(strcmp("add",argv[1])==0){
			add(argc, argv);
		}else if(strcmp("list",argv[1]) == 0){
			list(argc, argv);
		}else if(strcmp("awk",argv[1]) == 0){
			printf("增加工时");
		}else if(strcmp("cmp",argv[1]) == 0){
			printf("完成任务");
		}else if(strcmp("del",argv[1]) == 0){
			printf("删除任务");
		}
	}
}

void add(int argc, char** argv){
	if(argc != 3){
		fprintf(stderr,"the input is wrong\n");
		exit(0);
	}
	char* title = argv[2];

	time_t curr_time = time(NULL);

	// 创建任务详情
	s_task_detail* task_detail1 = (s_task_detail*)malloc(sizeof(s_task_detail));
	task_detail1->date = curr_time/24/60/60;
	task_detail1->cost = 7;

	s_task_detail* task_detail2 = (s_task_detail*)malloc(sizeof(s_task_detail));
	task_detail2->date = curr_time/24/60/60;
	task_detail2->cost = 8;

	s_task_detail* task_details[2];
	task_details[0] = task_detail1;
	task_details[1] = task_detail2;

	// 创建任务
	s_task* task1 = (s_task*)malloc(sizeof(s_task));
	task1->title=title;
	task1->seq = 1;
	task1->par_seq = 0;
	task1->state = 0;
	task1->begin_time=curr_time;
	task1->end_time=0;
	task1->task_details_len = 2;
	task1->task_details = task_details;

	s_task* tasks[1];
	tasks[0] = task1;

	// 创建工作空间
	s_worktime* worktime = (s_worktime*)malloc(sizeof(s_worktime));
	worktime->version = "V1.0";
	worktime->sequence = 1;
	worktime->tasks_len = 1;
	worktime->tasks = tasks;

	// 保存
	fwrite_worktime(WORKTIME_FILENAME,worktime);

	printf("success\n");

	free(task1);
	free(task_detail1);
	free(task_detail2);
}

void list(int argc, char** argv){

}
