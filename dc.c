#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "utils.h"
#include "dc.h"

static int dc_sys_status = 0;		  // 系统读出值
static int status = 0;				  // 系统是否修改过数值
static int dc_status = 0;			  // dc打开状态
static int dc_control_force_time = 5; // 如果经过5次超时，会强制设为程序本身的值
static int count = 0;				  // 初始化超时
static int scr_off = 0;

static int get_bri()
{
	FILE *fp;
	int brightness = -1;

	/* Open the command for reading. */
	fp = popen("settings get system screen_brightness", "r");
	if (fp == NULL)
	{
		perror("Failed to run command");
		return 1;
	}

	if (fscanf(fp, "%d", &brightness) == 1)
	{
		mydebug("Brightness is %d\n", brightness);
	}
	else
	{
		printf("Failed to read brightness value.\n");
		return 1;
	}

	/* close */
	pclose(fp);
	return brightness;
}

static int get_src_status()
{
	FILE *file;
	char line[100];
	char *substr;
	int request_power = -1;

	file = fopen(SCR_STATUS, "r");
	if (file == NULL)
	{
		printf("error to screen status file\n");
		return 1;
	}

	while (fgets(line, sizeof(line), file))
	{
		substr = strstr(line, "request power :");
		if (substr != NULL)
		{
			sscanf(substr, "request power :%d", &request_power);
			break;
		}
	}

	fclose(file);
	return request_power;
}

// dumpsys window | grep mShowingDream
static int get_lock_status()
{
	FILE *fp;
	char line[100];
	char *substr;
	char mDreamingLockscreen[6];

	fp = popen("dumpsys window | grep mShowingDream", "r");

	while (fgets(line, sizeof(line), fp))
	{
		substr = strstr(line, "mDreamingLockscreen=");
		if (substr != NULL)
		{
			sscanf(substr, "mDreamingLockscreen=%s", mDreamingLockscreen);
			break; // Exit the loop after finding
		}
	}

	pclose(fp);

	if (strcmp(mDreamingLockscreen, "true") == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int dc_control(struct config *params)
{
	if (get_lock_status())
	{
		mydebug("Screen lock, ignore\n");
		if (scr_off == 0)
		{
			file_wr(DC_PATH, w, 1);
			dc_status = 1;
			status = 0;
			scr_off = 1;
			mydebug("Reset status\n");
		}
		return 0;
	}
	else
	{
		scr_off = 0;
	}

	if (params->dc_en && get_src_status() == 2)
	{
		if (ispluged(params) && params->dc_usb_force)
		{
			mydebug("USB is active, will force enable DC\n");
			file_wr(DC_PATH, w, 1);
			dc_status = 1;
		}
		else
		{
			int bri;
			dc_sys_status = file_wr(DC_PATH, r, 0);
			if (dc_sys_status == dc_status || status == 0 || count >= dc_control_force_time)
			{
				bri = get_bri();
				if (bri >= params->dc_on_bri)
				{
					file_wr(DC_PATH, w, 0);
					mydebug("Disable DC\n");
					dc_status = 0;
				}
				else
				{
					file_wr(DC_PATH, w, 1);
					mydebug("Enable DC\n");
					dc_status = 1;
				}
				count = 0;
			}
			else
			{
				mydebug("DC status not match, ignore, will force DC status in %d/5\n", count);
				count++;
			}
			status = 1;
		}
	}
	else
	{
		status = 0;
		mydebug("DC not enable or screen off\n");
	}

	return 0;
}
