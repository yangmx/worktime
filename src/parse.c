#include <stdio.h>
#include <string.h>
#include <io.h>

#include "parse.h"
#include "util.h"

#define BUFFER_SIZE 4096

unsigned long get_file_size(char* file_path){
	/*FILE* fp;
	if((fp = fopen(file_path,"rb")) == NULL){
		return 0;
	}
	unsigned char buffer[BUFFER_SIZE];
	unsigned long size = 0,temp_size;
	while(!feof(fp)){
		temp_size = fread(buffer,1,BUFFER_SIZE,fp);
		size += temp_size;
	}

	fclose(fp);*/

	// 判断文件夹是否存在
	intptr_t handle;
	struct _finddata_t * findData = (struct _finddata_t *) malloc(
			sizeof(struct _finddata_t));
	handle = _findfirst("worktime.wt", findData);    // 查找目录中的第一个文件
	if (handle == -1) {
		return 0;
	}

	do {
		if (findData->attrib & _A_SUBDIR) {
			continue;
		}
		return findData->size;
	} while (_findnext(handle, findData) == 0);    // 查找目录中的下一个文件

	_findclose(handle);    // 关闭搜索句柄

	return 0;
}

s_worktime* parse_worktime(char* file_path){
	// 读取数据
	unsigned long total_size = get_file_size(file_path);
	if(total_size == 0){
		return NULL;
	}
	unsigned char* buffer = (unsigned char*)malloc(sizeof(unsigned char)*total_size);
	FILE* fp;
	if((fp=fopen(file_path,"rb"))==NULL){
		return NULL;
	}
	fread(buffer,1,total_size,fp);
	fclose(fp);
	// 解析内容
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
	unsigned int seq = parse_buffer_int_simply(buffer,begin_pos,total_size);
	task->seq = seq;
	// 解析父序号
	unsigned int par_seq = parse_buffer_int_simply(buffer,begin_pos,total_size);
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

unsigned int parse_input_sequence(char* seq_no){
	int seq_no_len = strlen(seq_no);
//	if(seq_no_len < 2){
//		fprintf(stderr,"number is wrong\n");
//		exit(0);
//	}

//	if(*seq_no != '#'){
//		fprintf(stderr,"number is not start with #\n");
//		exit(0);
//	}
	unsigned int value = 0,temp_val = 0;
	for(int i=0;i<seq_no_len;i++){
		temp_val = seq_no[i] - '0';
		if(temp_val >= 0 && temp_val <= 9){
			value = value * 10 + temp_val;
		}
	}
	if(value == 0){
		fprintf(stderr,"number is wrong\n");
		exit(0);
	}
	return value;
}

unsigned int parse_input_duration(char* work_time){
	int len = strlen(work_time);

	unsigned int value = 0,temp_val = 0;
	char temp_char;
	int next_is_decimal = 0;
	int is_odd = 0;
	for(int i=0;i<len;i++){
		temp_char = work_time[i];
		if(temp_char == '.'){
			next_is_decimal = 1;
			continue;
		}
		if(next_is_decimal == 1){
			if(temp_char == '5'){
				is_odd = 1;
			}
			break;
		}
		temp_val = temp_char - '0';
		if(temp_val >= 0 && temp_val <= 9){
			value = value * 10 + temp_val;
		}else{
			break;
		}
	}
	value = value << 1;
	if(is_odd){
		value ++;
	}
	if(value == 0){
		fprintf(stderr,"number is wrong\n");
		exit(0);
	}
	return value;
}

