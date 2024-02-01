
#define VER "2401.3"
#define CONFIG_FILE "/data/adb/modules/BATTLIMIT/config.txt"

#define r 'r'
#define w 'w'

#define MAX_LINE_LENGTH 255
#define MAX_ENTRIES 100

#define DEBUG 1

#if DEBUG
#define mydebug(fmt, ...) printf("JZTool: "fmt, ##__VA_ARGS__)
#define POLLINGTIME_REFESH_CONFIG 1 //Secend
#define POLLINGTIME  3					// Secend
#define POLLINGTIME_ON  3					// Secend
#define POLLINGTIME_OFF  3					// Secend
#else
#define mydebug(fmt, ...) ((void)0)
#define POLLINGTIME_REFESH_CONFIG 15 //Secend
#define POLLINGTIME  10					// Secend
#define POLLINGTIME_ON  10					// Secend
#define POLLINGTIME_OFF  20					// Secend
#endif

struct configkeyval {
    char key[MAX_LINE_LENGTH];
    int value;
};

struct config {
    int batt_limitcap;
};

int file_wr(const char *filepath, const char wr, const int val);
void getcurtime();
int read_config(const char *filename, struct configkeyval *entries);
