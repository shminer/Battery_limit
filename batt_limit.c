#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "utils.h"
#include "batt_limit.h"

static int battst_reset(struct config *params) 
{
	if (params->battstats_reset == 1 && params->batt_changed == 1) {
		int a;
		a = system("dumpsys batterystats --reset > /dev/null 2>&1");
		if (a == -1){
			printf("Shell operation failed");
			return 1;
		}
		params->batt_changed = 0;
	}
	return 0;
}

static int chg_contol(struct config *params)
{

	if (ispluged(params)) {
		int battcap = 0, batt_limit = 0, i = 0;
		mydebug("BATT CAP limit to : %d\n", params->batt_limit);

		batt_limit = params->batt_limit; 

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

int do_batt_limit(struct config *params)
{
	int ret;
	ret = battst_reset(params);
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
