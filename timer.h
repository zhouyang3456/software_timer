#ifndef __TIMER__
#define __TIMER__

// Define the timer tick in milliseconds, adjust as needed
#define SOFT_TIMER_TICK_MS 1

// Timer modes and statuses
#define SOFT_TIMER_MODE_SINGLE 0
#define SOFT_TIMER_MODE_PERIODIC 1
#define SOFT_TIMER_STATUS_STOPPED 0
#define SOFT_TIMER_STATUS_RUNNING 1

// Structure representing a software timer
typedef struct SoftTimer {
    unsigned long timeout; // 当前剩余的滴答数
    unsigned long period;  // 周期性定时器的周期，以滴答数表示
    unsigned char mode;   // 定时器模式 (单次或周期)
    unsigned char status; // 定时器状态 (运行, 停止等)
    void (*callback)(void *param); // 回调函数指针
    void *param;          // 参数，供回调函数使用
    struct SoftTimer *next; // 指向下一个定时器的指针
} SoftTimer;

// Function prototypes
void init_soft_timer_system(void);
SoftTimer* create_soft_timer(unsigned long timeout_ms, unsigned char mode, void (*callback)(void *param), void *param);
void start_soft_timer(SoftTimer *timer);
void stop_soft_timer(SoftTimer *timer);
int delete_soft_timer(SoftTimer *timer);
void process_timers(void);

#endif // __TIMER__