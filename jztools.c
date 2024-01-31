#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "batt_limit.h"
#include "utils.h"
// Loop
int main() 
{
	int ret;
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
