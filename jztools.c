#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "batt_limit.h"
#include "utils.h"


static void parse_config(struct config params, const char *filepath)
 {
    struct configkeyval entries[MAX_ENTRIES];
	int linecount;

    linecount = read_config(filepath, entries);

    for (int i = 0; i < linecount; i++) {
		if (strcmp(entries[i].key, "batt_limit") == 0) {
			params.batt_limitcap = entries[i].value;
		} else {
			params.batt_limitcap = LIMIT_CAP;
		} 
		mydebug("Key: %s, Value: %d\n", entries[i].key, entries[i].value);
    }
}

// Loop
int main() 
{
	struct config params;

	int ret;
	parse_config(params,CONFIG_FILE);
	while(1){
		ret = do_batt_limit();
		if (ret) {
			printf("Batt limit failed\n");
		}

		mydebug("Version: %s\n",VER);
		getcurtime();
		sleep(POLLINGTIME);
	}
	return 0;
}

