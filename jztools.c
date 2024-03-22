#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "batt_limit.h"
#include "utils.h"
#include "dc.h"

struct config params;
int pollingtime_config;
int pollingtime;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static void initialize_config(struct config *params)
{
	params->batt_limit = LIMIT_CAP;
	params->battstats_reset = BATTSTATS_RESET;
	params->poolingtime = POLLINGTIME;
	params->poolingtime_on = POLLINGTIME_ON;
	params->poolingtime_off = POLLINGTIME_OFF;
	params->dc_on_bri = 3900;
	params->dc_en = 1;
	params->dc_usb_force = 0;
}

static void parse_config(struct config *params, const char *filepath)
{
	struct configkeyval entries[MAX_ENTRIES];
	int linecount;

	linecount = read_config(filepath, entries);

	for (int i = 0; i < linecount; i++)
	{
		if (strcmp(entries[i].key, "batt_limit") == 0)
		{
			params->batt_limit = entries[i].value;
		}
		else if (strcmp(entries[i].key, "battstats_reset") == 0)
		{
			params->battstats_reset = entries[i].value;
		}
		else if (strcmp(entries[i].key, "poolingtime_on") == 0)
		{
			params->poolingtime_on = entries[i].value;
		}
		else if (strcmp(entries[i].key, "poolingtime_off") == 0)
		{
			params->poolingtime_off = entries[i].value;
		}
		else if (strcmp(entries[i].key, "dc_en") == 0)
		{
			params->dc_en = entries[i].value;
		}
		else if (strcmp(entries[i].key, "dc_on_bri") == 0)
		{
			params->dc_on_bri = entries[i].value;
		}
		else if (strcmp(entries[i].key, "dc_usb_force") == 0)
		{
			params->dc_usb_force = entries[i].value;
		}
	}
}

void *refesh_config(void *arg)
{

	while (1)
	{
		pthread_mutex_lock(&mutex);
		parse_config(&params, CONFIG_FILE);
		pthread_mutex_unlock(&mutex);
		sleep(pollingtime_config);
	}

	return NULL;
}

void *main_task(void *arg)
{
	int ret;
	while (1)
	{
		pthread_mutex_lock(&mutex);
		pollingtime = params.poolingtime_on;
		ret = do_batt_limit(&params);
		if (ret)
		{
			printf("Batt limit failed\n");
		}
		ret = dc_control(&params);
		if (ret)
		{
			printf("dc control failed\n");
		}
		mydebug("Version: %s\n", VER);
		pthread_mutex_unlock(&mutex);
		getcurtime();
		sleep(pollingtime);
	}

	return NULL;
}

// Loop
int main()
{
	pthread_t thread_config, thread_main;

	initialize_config(&params);
	pollingtime_config = POLLINGTIME_REFESH_CONFIG;

	pthread_create(&thread_config, NULL, refesh_config, NULL);
	pthread_create(&thread_main, NULL, main_task, NULL);
	pthread_join(thread_config, NULL);
	pthread_join(thread_main, NULL);
	return 0;
}