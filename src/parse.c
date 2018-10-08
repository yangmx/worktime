#include <stdio.h>
#include <string.h>

#include "parse.h"
#include "util.h"

s_worktime* parse_worktime(unsigned char* buffer,unsigned long total_size){
	unsigned long begin_pos = 0;
	s_worktime* worktime = (s_worktime*)malloc(sizeof(s_worktime));
	// 读取版本
	char* version = parse_buffer_string(buffer,&begin_pos,total_size);
	worktime->version = version;
	// 读取序列号
	int sequence = parse_buffer_int_simply(buffer,&begin_pos,total_size);
	worktime->sequence=sequence;

	int tasks_len = 0, tasks_ary_len = 10;
	s_task** tasks = (s_task**)malloc(sizeof(s_task*) * tasks_ary_len);
	s_task** ext_tasks;
	s_task* temp_task;
	// 读取任务
	while(begin_pos != -1){
		if(tasks_len == tasks_ary_len){
			tasks_ary_len += 10;
			ext_tasks = (s_task**)malloc(sizeof(s_task*) * tasks_ary_len);
			memcpy(ext_tasks,tasks,tasks_len);
			free(tasks);
			tasks = ext_tasks;
		}
		temp_task = parse_task(buffer,&begin_pos,total_size);
		tasks[tasks_len] = temp_task;
		tasks_len++;
	}
	worktime->tasks = tasks;
	worktime->tasks_len = tasks_len;
	return worktime;
}

s_task* parse_task(unsigned char* buffer,unsigned long * begin_pos,unsigned long total_size){
	s_task* task = (s_task*)malloc(sizeof(s_task));
	// 解析序号
	int seq = parse_buffer_int_simply(buffer,begin_pos,total_size);
	task->seq = seq;
	// 解析父序号
	int par_seq = parse_buffer_int_simply(buffer,begin_pos,total_size);
	task->par_seq = par_seq;
	// 解析标题
	char* title = parse_buffer_string(buffer,begin_pos,total_size);
	task->title = title;
	// 解析状态
	unsigned char state = buffer[*begin_pos];
	task->state = state;
	*begin_pos = *begin_pos + 1;
	// 解析开始时间
	unsigned long temp_time = (buffer[*begin_pos] << 24) + (buffer[*begin_pos + 1] << 16) + (buffer[*begin_pos+2] << 8) + buffer[*begin_pos+3];
	task->begin_time = temp_time;
	*begin_pos = *begin_pos + 4;
	// 解析结束时间
	temp_time = (buffer[*begin_pos] << 24) + (buffer[*begin_pos + 1] << 16) + (buffer[*begin_pos+2] << 8) + buffer[*begin_pos+3];
	task->end_time = temp_time;
	*begin_pos = *begin_pos + 4;
	// 解析工时数量
	int task_details_len = task->task_details_len = buffer[*begin_pos];
	task->task_details_len = task_details_len;
	*begin_pos = *begin_pos + 1;
	// 解析工时
	if(task_details_len >0){
		s_task_detail ** task_details = (s_task_detail**)malloc(sizeof(s_task_detail*)*task_details_len);
		short temp_date;
		s_task_detail * task_detail;
		for(int i=0;i<task_details_len;i++){
			task_detail = (s_task_detail*)malloc(sizeof(s_task_detail));
			temp_date = (buffer[*begin_pos] << 8) + buffer[*begin_pos+1];
			task_detail ->date = temp_date;
			task_detail->cost = buffer[*begin_pos + 2];
			task_details[i] = task_detail;
			*begin_pos = *begin_pos + 3;
		}
		task->task_details = task_details;
	}
	if(*begin_pos == total_size){
		*begin_pos = -1;
	}
	return task;
}
