#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "utils.h"

int file_wr(const char *filepath, const char wr, const int val)
{
	FILE *file;
	switch (wr) {
		case 'r':
			file = fopen(filepath, "r");
			if (file != NULL) {
				int readval;
				fscanf(file, "%d", &readval);
				fclose(file);
				return readval;
			} else {
				printf("Kernel Node not exists");
				return -1;
			}
			break;
		case 'w':
			file = fopen(filepath, "w");
			if (file != NULL) {
				fprintf(file, "%d", val);
				fclose(file);
				return 0;
			} else {
				printf("Kernel Node not exists");
				return -1;
			}
			break;
		default:
			printf("no options\n");
			return 0;
	}
}

int getcurtime() 
{
	time_t i;
	struct tm *timeinfo;
	
	time(&i);
	timeinfo = localtime(&i);
	mydebug("=====NOW is %02d:%02d:%02d======\n",timeinfo->tm_hour, timeinfo->tm_min, timeinfo-> tm_sec);
	return 0;
}
