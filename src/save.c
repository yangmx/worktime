#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

#include "bean.h"
#include "save.h"
#include "util.h"
#include "parse.h"

void backup_file(char* file_path){
	unsigned int next_seq = find_curr_backup_seq();
	next_seq ++;

	FILE* src_fp;
	if((src_fp = fopen(file_path,"rb")) != NULL){
		// ����·���ַ���
		char* string_ary[5];
		string_ary[0] = BACKUP_DIR;
		string_ary[1] = "/";
		string_ary[2] = file_path;
		string_ary[3] = ".";
		string_ary[4] = int2str(next_seq, 0);
		char * bk_file_path = concat_string(string_ary, 5);

		FILE * dest_fp;
		if((dest_fp = fopen(bk_file_path,"wb+")) != NULL){
			unsigned char * buffer[4096];
			int buffer_pos = 0;
			while(!feof(src_fp)){
				buffer_pos = fread(buffer,1,4096,src_fp);
				fwrite(buffer,1,buffer_pos,dest_fp);
			}
			fclose(dest_fp);
		}
		fclose(src_fp);
	}
}

void restore_file(char* file_path){
	unsigned int curr_seq = find_curr_backup_seq();

	// ����·���ַ���
	char* string_ary[5];
	string_ary[0] = BACKUP_DIR;
	string_ary[1] = "/";
	string_ary[2] = file_path;
	string_ary[3] = ".";
	string_ary[4] = int2str(curr_seq, 0);
	char * bk_file_path = concat_string(string_ary,5);

	FILE* src_fp;
	if ((src_fp = fopen(bk_file_path, "rb")) != NULL) {
		FILE * dest_fp;
		if ((dest_fp = fopen(file_path, "wb+")) != NULL) {
			unsigned char * buffer[4096];
			int buffer_pos = 0;
			while (!feof(src_fp)) {
				buffer_pos = fread(buffer, 1, 4096, src_fp);
				fwrite(buffer, 1, buffer_pos, dest_fp);
			}
			fclose(dest_fp);
		}
		fclose(src_fp);
		printf("restore success! file name:[%s]\n", bk_file_path);
		// ɾ��ԭ�����ļ�
		remove(bk_file_path);
	}else{
		printf("restore fail! file not exists!");
	}
}

void fwrite_worktime(char* file_path, s_worktime* worktime){
	// �����ļ�
	backup_file(file_path);
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
	s_task_detail* detail;
	for(int i=0,j=task->task_details_len;i<j;i++){
		detail = (task->task_details)[i];
		if(detail->title != NULL){
			fwrite(detail->title,1,strlen(detail->title) + 1,fp);
		}else{
			fputc('\0',fp);
		}
		// д��ʱ����
		date = detail->date;
		fputc(date >> 8,fp);
		fputc(date,fp);
		// д��ʱʱ��
		fputc(detail->cost,fp);
	}
}

unsigned int find_curr_backup_seq(){
	// �ж��ļ����Ƿ����
	if(access(BACKUP_DIR,F_OK) == -1){
		system("mkdir .backups");
		system("attrib +h .backups");
	}
	intptr_t handle;
	struct _finddata_t * findData = (struct _finddata_t *) malloc(
			sizeof(struct _finddata_t));
	handle = _findfirst(".backups\\worktime.wt.*", findData);    // ����Ŀ¼�еĵ�һ���ļ�
	if (handle == -1) {
		return 1;
	}

	char* temp_file_name;
	int curr_max_value = 1,temp_value;
	int prev_len = strlen("worktime.wt.");
	do {
		if (findData->attrib & _A_SUBDIR) {
			continue;
		}
		temp_file_name = findData->name;
		temp_value = parse_input_sequence(temp_file_name + prev_len);
		if(temp_value > curr_max_value){
			curr_max_value = temp_value;
		}
	} while (_findnext(handle, findData) == 0);    // ����Ŀ¼�е���һ���ļ�

	_findclose(handle);    // �ر��������

	return curr_max_value;
}

