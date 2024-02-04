#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

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
				printf("Kernel Node not exists\n");
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
				printf("Kernel Node not exists\n");
				return -1;
			}
			break;
		default:
			printf("no options\n");
			return 0;
	}
}

void getcurtime() 
{
	time_t i;
	struct tm *timeinfo;
	
	time(&i);
	timeinfo = localtime(&i);
	mydebug("=====NOW is %02d:%02d:%02d======\n",timeinfo->tm_hour, timeinfo->tm_min, timeinfo-> tm_sec);
}

int read_config(const char *filepath, struct configkeyval *entries) 
{
    FILE *file;
	if (access(filepath, F_OK) == 0)
	{
		mydebug("Import config file: \"%s\"\n", filepath);
		file = fopen(filepath, "r");
		if (file == NULL) {
			printf("Error opening file, will use defaule configure");
		}

		char line[MAX_LINE_LENGTH];
		int numEntries = 0;

		while (fgets(line, sizeof(line), file) != NULL) {
			if (sscanf(line, "%[^:]:%d", entries[numEntries].key, &entries[numEntries].value) == 2) 
			{
				mydebug("key %s, val: %d\n", entries[numEntries].key, entries[numEntries].value);
				(numEntries)++;
				if (numEntries >= MAX_ENTRIES) {
					printf("Too many entries. Increase MAX_ENTRIES.\n");
				}
			}
		}
		fclose(file);
		return numEntries;
	} else {
		printf("Config not exists\n");
	}
	return 0;
}
