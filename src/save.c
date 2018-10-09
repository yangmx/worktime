#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bean.h"
#include "save.h"
#include "util.h"

#define BACKUP_DIR ".backups"

void backup_file(char* file_path,unsigned int prev_seq){
	FILE* src_fp;
	if((src_fp = fopen(file_path,"r")) != NULL){
		char* prev_seq_str = int2str(prev_seq);
		int bk_file_path_len = strlen(BACKUP_DIR) + 1 + strlen(file_path) + 1 + strlen(prev_seq_str);
		printf("backup file path :%u\n", bk_file_path_len);
		fflush(stdout);
		char * bk_file_path = (char*)malloc(sizeof(char) * (bk_file_path_len + 1));
		strcat(bk_file_path,BACKUP_DIR);
		strcat(bk_file_path,"/");
		strcat(bk_file_path,file_path);
		strcat(bk_file_path,".");
		strcat(bk_file_path,prev_seq_str);
		printf("backup file path :%s\n", bk_file_path);
		fflush(stdout);
//		FILE * dest_fp;
//		if((dest_fp = fopen(bk_file_path,"wb+")) != NULL){
//
//		}
	}else{
		printf("src file not exists!");
		fflush(stdout);
	}
}

void fwrite_worktime(char* file_path, s_worktime* worktime){
	// �����ļ�
	worktime->prev_seq = worktime->prev_seq + 1;
	backup_file(file_path, worktime->prev_seq);
	// ��д�ļ�
	FILE* fp;
	if((fp = fopen(file_path,"wb+")) == NULL){
		fprintf(stderr,"open %s file fail\n", file_path);
		exit(0);
	}
	// д�汾��
	fwrite(worktime->version,1,strlen(worktime->version),fp);
	fputc('\0',fp);

	// д���к�
	fwrite_int_simply(worktime->sequence,fp);
	fputc('\0',fp);

	// ����д����
	for(int i=0,j=worktime->tasks_len;i<j;i++){
		fwrite_task((worktime->tasks)[i],fp);
	}

	fclose(fp);
}

void fwrite_task(s_task * task, FILE * fp){
	// д��ǰ���
	fwrite_int_simply(task->seq,fp);
	fputc('\0',fp);

	// д�����
	if(task->par_seq != 0){
		fwrite_int_simply(task->par_seq,fp);
	}
	fputc('\0',fp);

	// д����
	fwrite(task->title,1,strlen(task->title) + 1,fp);

	// д״̬
	fputc(task->state,fp);

	// д����
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

	// д��ʱ����
	fputc(task->task_details_len,fp);

	int date;
	for(int i=0,j=task->task_details_len;i<j;i++){
		// д��ʱ����
		date = (task->task_details)[i]->date;
		fputc(date >> 8,fp);
		fputc(date,fp);
		// д��ʱʱ��
		fputc((task->task_details)[i]->cost,fp);
	}
}

