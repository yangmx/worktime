#ifndef _WORKTIME_H
#define _WORKTIME_H

#include "bean.h"

void add(int argc, char** argv);
void list(int argc, char** argv, char* file_path);

void add_work_time(int argc, char** argv);
void delete_task(int argc, char** argv);
void finish_task(int argc, char** argv);
void restore_task(int argc, char** argv);
void backup_task(int argc, char** argv);
void check_task(int argc, char** argv);

#endif
