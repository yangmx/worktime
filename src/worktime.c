#define WORKTIME_FILENAME "worktime.wt"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

#include "worktime.h"
#include "save.h"
#include "parse.h"
#include "util.h"

int main(int argc, char** argv){
	if(argc > 1){
		if(strcmp("add",argv[1])==0){
			add(argc, argv);
		}else if(strcmp("list",argv[1]) == 0){
			list(argc, argv);
		}else if(strcmp("awk",argv[1]) == 0){
			add_work_time(argc, argv);
		}else if(strcmp("cmp",argv[1]) == 0){
			finish_task(argc, argv);
		}else if(strcmp("del",argv[1]) == 0){
			delete_task(argc, argv);
		}else{
			fprintf(stderr,"command is wrong\n");
			exit(0);
		}
	}
}

void delete_task(int argc, char** argv){
	if(argc != 3){
		fprintf(stderr,"the input is wrong\n");
		exit(0);
	}
	char* seq_no = argv[2];

	unsigned int sequence = parse_input_sequence(seq_no);
	// 解析原数据
	s_worktime* worktime = parse_worktime(WORKTIME_FILENAME);
	if(worktime == NULL){
		fprintf(stderr,"file is empty\n");
		exit(0);
	}
	int tasks_len = worktime->tasks_len;
	s_task** tasks = worktime->tasks;
	s_task* task = NULL;
	for(int i=0;i< tasks_len;i++){
		if(task != NULL){
			tasks[i-1] = tasks[i];
		} else if(tasks[i]->seq == sequence){
			task = tasks[i];
		}
	}
	if(task != NULL){
		tasks_len --;
		worktime->tasks_len = tasks_len;

		// 保存
		fwrite_worktime(WORKTIME_FILENAME,worktime);
	}

	printf("success\n");
}

void finish_task(int argc, char** argv){
	if(argc != 3){
		fprintf(stderr,"the input is wrong\n");
		exit(0);
	}
	char* seq_no = argv[2];

	unsigned int sequence = parse_input_sequence(seq_no);
	// 解析原数据
	s_worktime* worktime = parse_worktime(WORKTIME_FILENAME);
	if(worktime == NULL){
		fprintf(stderr,"file is empty\n");
		exit(0);
	}
	int tasks_len = worktime->tasks_len;
	s_task** tasks = worktime->tasks;
	s_task* task = NULL;
	for(int i=0;i< tasks_len;i++){
		if(tasks[i]->seq == sequence){
			task = tasks[i];
			break;
		}
	}
	if(task == NULL){
		fprintf(stderr,"not found\n");
		exit(0);
	}
	if(task->state == 0){
		task->state = 1;
		task->end_time = time(NULL);

		// 保存
		fwrite_worktime(WORKTIME_FILENAME,worktime);
	}

	printf("success\n");
}

void add_work_time(int argc, char** argv){
	if(argc != 4){
		fprintf(stderr,"the input is wrong\n");
		exit(0);
	}
	char* seq_no = argv[2];
	char* work_time = argv[3];

	unsigned int sequence = parse_input_sequence(seq_no);
	unsigned int duration = parse_input_duration(work_time);

	// 解析原数据
	s_worktime* worktime = parse_worktime(WORKTIME_FILENAME);
	if(worktime == NULL){
		fprintf(stderr,"file is empty\n");
		exit(0);
	}
	int tasks_len = worktime->tasks_len;
	s_task** tasks = worktime->tasks;
	s_task* task = NULL;
	for(int i=0;i< tasks_len;i++){
		if(tasks[i]->seq == sequence){
			task = tasks[i];
			break;
		}
	}
	if(task == NULL){
		fprintf(stderr,"not found\n");
		exit(0);
	}

	s_task_detail** details = task->task_details;
	int detail_len = task->task_details_len;

	time_t curr_time = time(NULL);
	// 创建任务详情
	s_task_detail* detail = (s_task_detail*)malloc(sizeof(s_task_detail));
	detail->date = curr_time/24/60/60;
	detail->cost = duration;

	// 加入对象
	if(detail_len == 0){
		details = (s_task_detail**)malloc(sizeof(s_task_detail*));
	}else{
		s_task_detail** temp_details = (s_task_detail**)malloc(sizeof(s_task_detail*) * (detail_len + 1));
		for(int i=0;i<detail_len;i++){
			temp_details[i] = details[i];
		}
		free(details);
		details = temp_details;
	}
	detail_len += 1;
	details[detail_len -1] = detail;

	task->task_details = details;
	task->task_details_len = detail_len;

	// 保存
	fwrite_worktime(WORKTIME_FILENAME,worktime);

	printf("success\n");
}

void add(int argc, char** argv){
	if(argc != 3){
		fprintf(stderr,"the input is wrong\n");
		exit(0);
	}
	char* title = argv[2];

	// 解析原数据
	s_worktime* worktime = parse_worktime(WORKTIME_FILENAME);
	if(worktime == NULL){
		// 创建工作空间
		worktime = (s_worktime*)malloc(sizeof(s_worktime));
		worktime->version = "V1.0";
		worktime->sequence = 0;
		worktime->tasks_len = 0;
		worktime->tasks = NULL;
	}

	time_t curr_time = time(NULL);
	// 获取下一序列值
	int next_seq = (worktime->sequence) + 1;
	worktime->sequence = next_seq;

	// 创建任务
	s_task* task = (s_task*)malloc(sizeof(s_task));
	task->title=title;
	task->seq = next_seq;
	task->par_seq = 0;
	task->state = 0;
	task->begin_time=curr_time;
	task->end_time=0;
	task->task_details_len = 0;

	int tasks_len = worktime->tasks_len;
	// 初始化任务集合
	s_task** tasks = worktime->tasks;
	if(tasks == NULL){
		tasks = (s_task**)malloc(sizeof(s_task*));
		worktime->tasks = tasks;
		worktime->tasks_len = 1;

		// 增加新元素
		tasks_len = 1;
		tasks[tasks_len - 1] = task;
	}else{
		// 扩展空间
		s_task** temp_tasks = (s_task**)malloc(sizeof(s_task*) * (tasks_len + 1));
		for(int i=0;i<tasks_len;i++){
			temp_tasks[i] = tasks[i];
		}
		free(tasks);
		tasks = temp_tasks;
		worktime->tasks = tasks;
		worktime->tasks_len = tasks_len + 1;

		// 增加新元素
		tasks_len ++;
		tasks[tasks_len - 1] = task;
	}

	// 保存
	fwrite_worktime(WORKTIME_FILENAME,worktime);

	printf("success\n");

	free(task);
}

void list(int argc, char** argv){
	int show_all = 0;
	int show_detail = 1;
	for(int i=2;i<argc;i++){
		if(strcmp("-a",argv[i]) == 0){
			show_all = 1;
		}else if(strcmp("-h",argv[i]) == 0){
			show_detail = 0;
		}
	}

	s_worktime* worktime = parse_worktime(WORKTIME_FILENAME);
	int tasks_len = worktime->tasks_len;
	s_task * task;
	struct tm * temp_tm;
	s_task_detail * task_detail;
	int task_details_len;
	time_t temp_time;
	int total_worktime;
	for(int i=0;i<tasks_len;i++){
		task = (worktime->tasks)[i];
		if(task->state == 1 && show_all == 0){
			continue;
		}
		printf("#%u",task->seq);
		if(task->par_seq == 1){
			printf("[#%u]",task->par_seq);
		}
		// 计算总工时消耗
		total_worktime = 0;
		task_details_len = task->task_details_len;
		for(int j=0;j<task_details_len;j++){
			total_worktime += (task->task_details)[j]->cost;
		}
		printf("\t[%d]",task->state);
		if(total_worktime % 2 == 0){
			printf("[%dh]", total_worktime / 2);
		}else{
			printf("[%d.5h]", total_worktime / 2);
		}
		printf("%s", task->title);
		temp_tm = localtime(&(task->begin_time));
		printf("[%4d/%02d/%02d %02d:%02d]",temp_tm->tm_year + 1900, temp_tm->tm_mon+1,temp_tm->tm_mday,temp_tm->tm_hour,temp_tm->tm_min);
		if(task->end_time != 0){
			temp_tm = localtime(&(task->end_time));
			printf("[%4d/%02d/%02d %02d:%02d]",temp_tm->tm_year + 1900, temp_tm->tm_mon+1,temp_tm->tm_mday,temp_tm->tm_hour,temp_tm->tm_min);
		}
		printf("\n");
		fflush(stdout);
		// 显示工时详情
		if(show_detail){
			for(int j=0;j<task_details_len;j++){
				task_detail = (task->task_details)[j];
				temp_time = task_detail->date * 24*60*60;
				temp_tm = localtime(&temp_time);
				if(task_detail->cost % 2 == 0){
					printf("\t--[%4d/%02d/%02d]%dh\n", temp_tm->tm_year + 1900, temp_tm->tm_mon+1,temp_tm->tm_mday,task_detail->cost /2);
				} else{
					printf("\t--[%4d/%02d/%02d]%d.5h\n", temp_tm->tm_year + 1900, temp_tm->tm_mon+1,temp_tm->tm_mday,task_detail->cost /2);
				}
				fflush(stdout);
			}
		}
	}
}
