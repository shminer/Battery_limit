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
		mydebug("Power connected\n");
		batt_chaged = 1;
		return 1;
	} else {
		mydebug("No Power connected\n");
		return 0;
	}
}

static int battst_reset() 
{
	if (BATTSTATS_RESET == 1 && batt_chaged == 1) {
		int a;
		a = system("dumpsys batterystats --reset > /dev/null 2>&1");
		if (a == -1){
			printf("Shell operation failed");
			return 1;
		}
		batt_chaged = 0;
	}
	return 0;
}

static int chg_contol(struct config params)
{

	if (ispluged()) {
		int battcap = 0, batt_limit = params.batt_limitcap, i = 0;
		
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
	struct config params;
	int ret;
	ret = battst_reset();
	if (ret) {
		printf("Battery status reset failed\n");
		return 1;
	}
	ret = chg_contol(params);
	if (ret) {
		printf("Charrge control failed\n");
		return 1;
	}
	return 0;
}
