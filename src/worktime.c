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
		if(strcmp("t",argv[1])==0 || strcmp("task",argv[1])==0){
			add(argc, argv);
		}else if(strcmp("l",argv[1]) == 0 || strcmp("list",argv[1])==0){
			list(argc, argv,WORKTIME_FILENAME);
		}else if(strcmp("h",argv[1]) == 0 || strcmp("hour",argv[1])==0){
			add_work_time(argc, argv);
		}else if(strcmp("c",argv[1]) == 0 || strcmp("complete",argv[1])==0){
			finish_task(argc, argv);
		}else if(strcmp("d",argv[1]) == 0 || strcmp("delete",argv[1])==0){
			delete_task(argc, argv);
		}else if(strcmp("r",argv[1]) == 0 || strcmp("restore",argv[1])==0){
			restore_task(argc, argv);
		}else if(strcmp("ac",argv[1]) == 0 || strcmp("achieve",argv[1])==0){
			backup_task(argc, argv);
		}else if(strcmp("ck",argv[1]) == 0 || strcmp("check",argv[1])==0){
			check_task(argc, argv);
		}else{
			fprintf(stderr,"command is wrong\n");
			exit(0);
		}
	}
}

void check_task(int argc, char** argv){
	if(argc < 3){
		fprintf(stderr, "the input is wrong\n");
		exit(0);
	}
	char* date = argv[2];

	char* ary[5];
	ary[0] = BACKUP_DIR;
	ary[1] = "/";
	ary[2] = WORKTIME_FILENAME;
	ary[3] = ".";
	ary[4] = date;
	char * src_file_name = concat_string(ary,5);

	if(argc > 3){
		argv[2] = argv[3];
	}
	list(argc, argv, src_file_name);
}

void backup_task(int argc, char** argv){
	if(argc < 3){
		fprintf(stderr, "the input is wrong\n");
		exit(0);
	}
	char* date = argv[2];

	char* ary[3];
	ary[0] = WORKTIME_FILENAME;
	ary[1] = ".";
	ary[2] = date;
	char * src_file_name = concat_string(ary,3);
	backup_file_special(WORKTIME_FILENAME, src_file_name);

	printf("backup success! file name:[%s]\n" , src_file_name);
}

void restore_task(int argc, char** argv){
	restore_file(WORKTIME_FILENAME);
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

	printf("delete success! number is #%d\n" , sequence);
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

	printf("finish success! number is #%d\n" , sequence);
}

void add_work_time(int argc, char** argv){
	if(argc < 4){
		fprintf(stderr,"the input is wrong\n");
		exit(0);
	}
	char* seq_no = argv[2];
	char* work_time = argv[3];
	char* title = NULL;
	if(argc > 4){
		title = argv[4];
	}

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
	detail->title = title;
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

	printf("work time add success! number is #%d\n" , sequence);
}

void add(int argc, char** argv){
	int pid = 0;
	char* title = argv[2];
	char* pid_str;
	if(argc == 3){
		title = argv[2];
	} else if(argc == 4){
		title = argv[3];
		pid_str = argv[2];
		int temp_pid;
		int pid_len = strlen(pid_str);
		if(pid_str[0] == 'p' && strlen(pid_str) > 1){
			for(int i=1;i<pid_len;i++){
				temp_pid = pid_str[i] - '0';
				if(temp_pid >= 0 && temp_pid <= 9){
					pid = pid * 10 + temp_pid;
				}else{
					fprintf(stderr, "parent sequence is wrong\n");
					exit(0);
				}
			}
		}
	} else{
		fprintf(stderr,"the input is wrong\n");
		exit(0);
	}

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
	task->par_seq = pid;
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

	printf("task add success! number is #%d\n" , worktime->sequence);

	free(task);
}

void list(int argc, char** argv, char* file_path){
	int show_all = 0;
	int show_detail = 1;
	int show_time = 0;
	int spec_mday = 0;
	if(argc >= 3){
		char* param = argv[2];
		int param_len = strlen(param);
		for (int i = 0; i < param_len; i++) {
			if ('a' == param[i]) {
				show_all = 1;
			} else if ('h' == param[i]) {
				show_detail = 0;
			} else if ('t' == param[i]) {
				show_time = 1;
			} else if ('d' == param[i] && spec_mday == 0) {
				if(i + 1 < param_len){
					int temp_day ,temp_spec_mday;
					for(;i< param_len - 1;i++){
						// 取i+1的值，因为外层循环还有个i++
						temp_day = param[i+1] - '0';
						if(temp_day >= 0 && temp_day <= 9){
							temp_spec_mday = spec_mday * 10 + temp_day;
							if(temp_spec_mday <= 31){
								spec_mday = temp_spec_mday;
							}else{
								break;
							}
						}else{
							break;
						}
					}
				}
				if(spec_mday == 0){
					struct tm * today_tm;
					time_t today_time = time(NULL);
					today_tm = localtime(&today_time);
					spec_mday = today_tm->tm_mday;
				}
			}
		}
	}

	s_worktime* worktime = parse_worktime(file_path);
	if(worktime == NULL){
		fprintf(stderr,"file is empty\n");
		exit(0);
	}
	int tasks_len = worktime->tasks_len;
	s_task * task;
	struct tm * temp_tm;
	struct tm * task_begin_tm;
	s_task_detail * task_detail;
	int task_details_len;
	time_t temp_time;
	int total_worktime;
	s_print_row** row_ary = (s_print_row**)malloc(sizeof(s_print_row*)*20);
	int row_ary_len = 0;
	int row_ary_size=20;
	s_print_row** temp_row_ary;
	s_print_row* temp_row;

	char** col_ary;
	int col_ary_len;

	char** concat_ary;
	int concat_ary_len;

	int row_len;
	for(int i=0;i<tasks_len;i++){
		task = (worktime->tasks)[i];
		if(task->state == 1 && show_all == 0){
			continue;
		}
		// 判断行数，并扩展数组
		row_len = 1;
		if(show_detail){
			row_len += task->task_details_len;;
		}
		if(row_len + row_ary_len > row_ary_size){
			row_ary_size += 20;
			temp_row_ary = (s_print_row**)malloc(sizeof(s_print_row*)*(row_ary_size));
			for(int j=0;j<row_ary_len;j++){
				temp_row_ary[j] = row_ary[j];
			}
			row_ary = temp_row_ary;
		}
		// 时间筛选
		task_begin_tm = localtime(&(task->begin_time));
		int display_task_row = 0;
		if(spec_mday == 0 || (spec_mday > 0 && task_begin_tm->tm_mday == spec_mday)){
			// 若创建日期是当前，则必显示该任务
			display_task_row = 1;
		}
		// 计算总工时消耗
		total_worktime = 0;
		task_details_len = task->task_details_len;
		for(int j=0;j<task_details_len;j++){
			task_detail = (task->task_details)[j];
			// 获取日期
			temp_time = task_detail->date * 24*60*60;
			temp_tm = localtime(&temp_time);

			if(spec_mday > 0 && temp_tm->tm_mday != spec_mday){
				continue;
			}
			total_worktime += (task->task_details)[j]->cost;
			// 若一致，则必显示该行。
			if(display_task_row == 0){
				display_task_row = 1;
			}
		}
		if(!display_task_row){
			continue;
		}

		col_ary_len = 0;
		col_ary = (char**)malloc(sizeof(char*) * 3);

		// 第一列
		concat_ary_len = 0;
		concat_ary = (char**)malloc(sizeof(char*) * 11);
//		printf("#%u",task->seq);
		concat_ary[concat_ary_len++] = "#";
		concat_ary[concat_ary_len++] = int2str(task->seq,0);

		if(task->par_seq > 0){
//			printf("[#%u]",task->par_seq);
			concat_ary[concat_ary_len++] = "[#";
			concat_ary[concat_ary_len++] = int2str(task->par_seq,0);
			concat_ary[concat_ary_len++] = "]";
		}
		int seq_prev_len = strlen(concat_string(concat_ary,concat_ary_len));

		if(total_worktime % 2 == 0){
//			printf("[%dh]", total_worktime / 2);
			concat_ary[concat_ary_len++] = "[";
			concat_ary[concat_ary_len++] = int2str(total_worktime / 2,0);
			concat_ary[concat_ary_len++] = "h]";
		}else{
//			printf("[%d.5h]", total_worktime / 2);
			concat_ary[concat_ary_len++] = "[";
			concat_ary[concat_ary_len++] = int2str(total_worktime / 2,0);
			concat_ary[concat_ary_len++] = ".5h]";
		}
		col_ary[col_ary_len++] = concat_string(concat_ary,concat_ary_len);
		// 不处理释放空间问题

		// 第二列
		concat_ary_len = 0;
		concat_ary = (char**)malloc(sizeof(char*) * 4);

//		printf("\t[%d]",task->state);
		concat_ary[concat_ary_len++] = "[";
		concat_ary[concat_ary_len++] = int2str(task->state,0);
		concat_ary[concat_ary_len++] = "]";

//		printf("%s", task->title);
		concat_ary[concat_ary_len++] = task->title;
		col_ary[col_ary_len++] = concat_string(concat_ary,concat_ary_len);

		if(show_time){
			// 第三列
			concat_ary_len = 0;
			concat_ary = (char**)malloc(sizeof(char*) * 22);

			temp_tm = localtime(&(task->begin_time));
//			printf("\t[%4d/%02d/%02d %02d:%02d]",temp_tm->tm_year + 1900, temp_tm->tm_mon+1,temp_tm->tm_mday,temp_tm->tm_hour,temp_tm->tm_min);
			concat_ary[concat_ary_len++] = "[";
			concat_ary[concat_ary_len++] = int2str(temp_tm->tm_year + 1900,4);
			concat_ary[concat_ary_len++] = "/";
			concat_ary[concat_ary_len++] = int2str(temp_tm->tm_mon + 1,2);
			concat_ary[concat_ary_len++] = "/";
			concat_ary[concat_ary_len++] = int2str(temp_tm->tm_mday,2);
			concat_ary[concat_ary_len++] = " ";
			concat_ary[concat_ary_len++] = int2str(temp_tm->tm_hour,2);
			concat_ary[concat_ary_len++] = ":";
			concat_ary[concat_ary_len++] = int2str(temp_tm->tm_min,2);
			concat_ary[concat_ary_len++] = "]";
			if(task->end_time != 0){
				temp_tm = localtime(&(task->end_time));
//				printf("[%4d/%02d/%02d %02d:%02d]",temp_tm->tm_year + 1900, temp_tm->tm_mon+1,temp_tm->tm_mday,temp_tm->tm_hour,temp_tm->tm_min);
				concat_ary[concat_ary_len++] = "[";
				concat_ary[concat_ary_len++] = int2str(temp_tm->tm_year + 1900,4);
				concat_ary[concat_ary_len++] = "/";
				concat_ary[concat_ary_len++] = int2str(temp_tm->tm_mon + 1,2);
				concat_ary[concat_ary_len++] = "/";
				concat_ary[concat_ary_len++] = int2str(temp_tm->tm_mday,2);
				concat_ary[concat_ary_len++] = " ";
				concat_ary[concat_ary_len++] = int2str(temp_tm->tm_hour,2);
				concat_ary[concat_ary_len++] = ":";
				concat_ary[concat_ary_len++] = int2str(temp_tm->tm_min,2);
				concat_ary[concat_ary_len++] = "]";
			}
			//		printf("\n");
			col_ary[col_ary_len++] = concat_string(concat_ary,concat_ary_len);
		}

		// 组装行对象
		temp_row = (s_print_row*)malloc(sizeof(s_print_row));
		temp_row->char_ary = col_ary;
		temp_row->len = col_ary_len;
		row_ary[row_ary_len++] = temp_row;

		// 显示工时详情
		if(show_detail){
			for(int j=0;j<task_details_len;j++){
				task_detail = (task->task_details)[j];
				// 获取日期
				temp_time = task_detail->date * 24*60*60;
				temp_tm = localtime(&temp_time);

				if(spec_mday > 0 && temp_tm->tm_mday != spec_mday){
					continue;
				}

				// 设置行数据
				col_ary_len = 0;
				col_ary = (char**)malloc(sizeof(char*) * 3);

				// 第一列
				concat_ary_len = 0;
				concat_ary = (char**)malloc(sizeof(char*) * 5);

				concat_ary[concat_ary_len++] = creat_space_string(seq_prev_len);
				concat_ary[concat_ary_len++] = "[";
				concat_ary[concat_ary_len++] = int2str(task_detail->cost /2,0);
				if(task_detail->cost % 2 != 0){
//					printf(".5");
					concat_ary[concat_ary_len++] = ".5";
				}
//				printf("h\n");
				concat_ary[concat_ary_len++] = "h]";
				col_ary[col_ary_len++] = concat_string(concat_ary,concat_ary_len);

				// 第二列
				if(task_detail->title == NULL || strlen(task_detail->title) == 0){
					col_ary[col_ary_len++] = "none";
				}else{
					col_ary[col_ary_len++] = task_detail->title;
				}

				// 第三列
				concat_ary_len = 0;
				concat_ary = (char**)malloc(sizeof(char*) * 10);

//				printf("\t--[%4d/%02d/%02d]%d", temp_tm->tm_year + 1900, temp_tm->tm_mon+1,temp_tm->tm_mday,task_detail->cost /2);
				concat_ary[concat_ary_len++] = "[";
				concat_ary[concat_ary_len++] = int2str(temp_tm->tm_year + 1900,4);
				concat_ary[concat_ary_len++] = "/";
				concat_ary[concat_ary_len++] = int2str(temp_tm->tm_mon + 1, 2);
				concat_ary[concat_ary_len++] = "/";
				concat_ary[concat_ary_len++] = int2str(temp_tm->tm_mday, 2);
				concat_ary[concat_ary_len++] = "]";
				col_ary[col_ary_len++] = concat_string(concat_ary,concat_ary_len);

				// 组装行对象
				temp_row = (s_print_row*)malloc(sizeof(s_print_row));
				temp_row->char_ary = col_ary;
				temp_row->len = col_ary_len;
				row_ary[row_ary_len++] = temp_row;
			}
		}
	}
	// 输出
	print_row_ary(row_ary,row_ary_len);
}
