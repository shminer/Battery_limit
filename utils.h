
#define VER "2401.2"
#define POLLINGTIME  10					// Secend
#define POLLINGTIME_ON  10					// Secend
#define POLLINGTIME_OFF  20					// Secend
#define r 'r'
#define w 'w'

#define DEBUG 1

#if DEBUG
#define mydebug(fmt, ...) printf("JzTOOL: "fmt, ##__VA_ARGS__)
#else
#define mydebug(fmt, ...) ((void)0)
#endif

int file_wr(const char *filepath, const char wr, const int val);
int getcurtime();
