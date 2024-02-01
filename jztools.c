#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "batt_limit.h"
#include "utils.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
struct config params;

static void initialize_config(struct config *params)
{
	params->batt_limitcap = LIMIT_CAP;
}
static void parse_config(struct config *params, const char *filepath)
 {
    struct configkeyval entries[MAX_ENTRIES];
	int linecount;

    linecount = read_config(filepath, entries);

    for (int i = 0; i < linecount; i++) {
		if (strcmp(entries[i].key, "batt_limit") == 0) {
			mydebug("change batt_limitcap value to %d\n", entries[i].value);
			params->batt_limitcap = entries[i].value;
		}
    }
}

void *refesh_config(void* arg) {

    while (1) {
		pthread_mutex_lock(&mutex);
		parse_config(&params,CONFIG_FILE);
		pthread_mutex_unlock(&mutex);
        sleep(POLLINGTIME_REFESH_CONFIG);
    }

    return NULL;
}

void *main_task(void* arg) {
	int ret;
    while (1) {
        pthread_mutex_lock(&mutex);
		ret = do_batt_limit(&params);
		if (ret) {
			printf("Batt limit failed\n");
		}
		mydebug("Version: %s\n",VER);
		pthread_mutex_unlock(&mutex);
		getcurtime();
        sleep(POLLINGTIME);
    }

    return NULL;
}

// Loop
int main() 
{
	pthread_t thread_config, thread_main;

	initialize_config(&params);
	pthread_create(&thread_config, NULL, refesh_config, NULL);
	pthread_create(&thread_main, NULL, main_task, NULL);
	pthread_join(thread_config, NULL);
	pthread_join(thread_main, NULL);
	return 0;
}

