#define WORKTIME_FILENAME "worktime.wt"

#include <stdio.h>

int main(int argc, char** argv){
	for(int i=0;i<argc;i++){
		printf("argc[%d]=%s\n",i,argv[i]);
	}
}
