#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "utils.h"
#include "batt_limit.h"

int batt_chaged = 0;
int limitcap = LIMIT_CAP;

static int ispluged() 
{
	if (file_wr(IS_CHARGING_USB, r, 0) == 1 || file_wr(IS_CHARGING_WIRELESS, r, 0) == 1 ) {
		mydebug("Charged enabled\n");
		batt_chaged = 1;
		return 1;
	} else {
		mydebug("Charged disabled\n");
		return 0;
	}
}

static int battst_reset() 
{
	if (ispluged() == 0 && BATTSTATS_RESET == 1 && batt_chaged == 1) {
		int a;
		a = system("dumpsys batterystats --reset > /dev/null 2>&1");
		if (a == -1){
			printf("Shell call failed");
			return 1;
		}
		batt_chaged = 0;
	}
	return 0;
}

static int chk_sdfile(const char *filepath) 
{
	FILE *file;
	if (access(filepath, F_OK) != 0) {
		mydebug("Sdcard file not exists, create it!!\n");
		file = fopen(filepath, "w");
		if (file != NULL) {
			fprintf(file, "%d", limitcap);
			fclose(file);
		} else {
			printf("File created failed : %s\n", filepath);
		}
	} else {
		limitcap = file_wr(filepath, r, 0);
		if (limitcap == -1 ) {
			printf("Sdcard file read failed\n");
		}
		mydebug("Read limitcap: %d\n", limitcap);
	}
	return 0;
}


static int chg_contol() 
{

	if (ispluged()) {
		int battcap = 0, batt_limit = limitcap, i = 0;
		
		battcap = file_wr(BATT_CAP, r, 0);
		mydebug("BATT CAP: %d\n", battcap);
		if (battcap != -1) {
			if (batt_limit > 100)
				batt_limit = 100;
			if (battcap >= batt_limit) {
				
				i = file_wr(BATT_CONTROL, w, 0);
				if (i == -1){
					printf("write failed\n");
					return -1;
				}
				mydebug("batt limit mode.\n");
			} else {
				
				i = file_wr(BATT_CONTROL, w, 1);
				if (i == -1){
					printf("write failed\n");
					return -1;
				}
				mydebug("batt nolimit mode.\n");
			}
			return 0;
		}
	} else {
		mydebug("No charging.\n");
	}
	return 0;
}


int do_batt_limit()
{
	int ret;
	chk_sdfile(BATT_CAPFILE);
	ret = battst_reset();
	if (ret) {
		printf("Battery status reset failed\n");
		return 1;
	}
	ret = chg_contol();
	if (ret) {
		printf("Charrge control failed\n");
		return 1;
	}
	return 0;
}
