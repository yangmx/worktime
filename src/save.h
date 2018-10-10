#ifndef _SAVE_H
#define _SAVE_H

#include "bean.h"

void fwrite_worktime(char* file_path, s_worktime* worktime);
void fwrite_task(s_task * task, FILE * fp);
void backup_file(char* file_path);
void restore_file(char* file_path);

unsigned int find_curr_backup_seq();

#endif
