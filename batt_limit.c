#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


#define BATT_CONTROL "/sys/devices/platform/soc/soc:oplus,chg_intf/oplus_chg/battery/mmi_charging_enable"
#define BATT_CAP "/sys/devices/platform/soc/soc:oplus,chg_intf/oplus_chg/battery/capacity"
#define IS_CHARGING_USB "/sys/class/power_supply/usb/online"
#define IS_CHARGING_WIRELESS "/sys/class/power_supply/wireless/online"
#define BATT_CAPFILE "/sdcard/battcap"

#define LIMIT_CAP 85
#define BATTSTATS_RESET 1
#define POLLINGTIME  10					// Secend

//#define POLLINGTIME_ON  30			// Secend
//#define POLLINGTIME_OFF	60			// Secend

#define r 'r'
#define w 'w'
#define VER "2401.1"

#define DEBUG 0

#if DEBUG
#define battdebug(fmt, ...) printf("BLT: "fmt, ##__VA_ARGS__)
#else
#define battdebug(fmt, ...) ((void)0)
#endif

int batt_chaged = 0;
int limitcap = LIMIT_CAP;

static int file_wr(const char *filepath, const char wr, const int val)
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

static int ispluged() 
{
	if (file_wr(IS_CHARGING_USB, r, 0) == 1 || file_wr(IS_CHARGING_WIRELESS, r, 0) == 1 ) {
		battdebug("Charged enabled\n");
		batt_chaged = 1;
		return 1;
	} else {
		battdebug("Charged disabled\n");
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
			return -1;
		}
		batt_chaged = 0;
	}
	return 0;
}

static int chk_sdfile(const char *filepath) 
{
	FILE *file;
	if (access(filepath, F_OK) != 0) {
		battdebug("Sdcard file not exists, create it!!\n");
		file = fopen(filepath, "w");
		if (file != NULL) {
			fprintf(file, "%d", limitcap);
			fclose(file);
		} else {
			printf("File created failed : %s", filepath);
			return -1;
		}
	} else {
		limitcap = file_wr(filepath, r, 0);
		if (limitcap == -1 ) {
			printf("Sdcard file read failed\n");
		}
		battdebug("Read limitcap: %d\n", limitcap);
	}
	return 0;
}


static int chg_contol() 
{

	if (ispluged()) {
		int battcap = 0, batt_limit = limitcap, i = 0;
		
		battcap = file_wr(BATT_CAP, r, 0);
		battdebug("BATT CAP: %d\n", battcap);
		if (battcap != -1) {
			if (batt_limit > 100)
				batt_limit = 100;
			if (battcap >= batt_limit) {
				
				i = file_wr(BATT_CONTROL, w, 0);
				if (i == -1){
					printf("write failed\n");
					return -1;
				}
				battdebug("batt limit mode.\n");
			} else {
				
				i = file_wr(BATT_CONTROL, w, 1);
				if (i == -1){
					printf("write failed\n");
					return -1;
				}
				battdebug("batt nolimit mode.\n");
			}
			return 0;
		}
	} else {
		battdebug("No charging.\n");
	}
	return 0;
}

static int getcurtime() {
	time_t i;
	struct tm *timeinfo;
	
	time(&i);
	timeinfo = localtime(&i);
	battdebug("=====NOW is %02d:%02d:%02d======\n",timeinfo->tm_hour, timeinfo->tm_min, timeinfo-> tm_sec);
	return 0;
}
int main() 
{
	// Loop

	int ret;
	while(1){
		ret = chk_sdfile(BATT_CAPFILE);
		if (ret) {
			printf("Check battcap failed\n");
			return -1;
		}
		ret = battst_reset();
		if (ret) {
			printf("Batty status reset failed\n");
			return -1;
		}
		ret = chg_contol();
		if (ret) {
			printf("Chaerge control failed\n");
			return -1;
		}
		battdebug("Version: %s\n",VER);
		getcurtime();
		
		sleep(POLLINGTIME);
	}

	return 0;
}