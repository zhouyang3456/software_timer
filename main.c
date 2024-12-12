#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include "timer.h"

void print_timestamp();

// Example callback functions
void example_callback_1(void *param) {
    print_timestamp();
    printf("Callback 1 triggered[%d]\n", (int)param);
}

void example_callback_2(void *param) {
    print_timestamp();
    printf("Callback 2 triggered[%d]\n", (int)param);
}

// Main function to demonstrate usage
int main() {
    // Initialize the timer system
    init_soft_timer_system();

    // Create two timers with different timeouts and modes
    SoftTimer *timer1 = create_soft_timer(5000, SOFT_TIMER_MODE_SINGLE, example_callback_2, (void*)5000); // 5 seconds single-shot
    SoftTimer *timer2 = create_soft_timer(1000, SOFT_TIMER_MODE_PERIODIC, example_callback_1, (void*)1000); // 1 second periodic

    // Start both timers
    start_soft_timer(timer1);
    start_soft_timer(timer2);

    // Simulate the main loop where we periodically call process_timers()
    while (1) {
        // process_timers();
        // Simulate a delay that matches our SOFT_TIMER_TICK_MS
        struct timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = SOFT_TIMER_TICK_MS * 1000000L;
        nanosleep(&ts, NULL);
    }

    // Cleanup (not reached in this example)
    delete_soft_timer(timer1);
    delete_soft_timer(timer2);

    return 0;
}

void print_timestamp() {
    time_t now;
    time(&now); // 获取当前时间
    char timestamp[80];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    printf("Timestamp: %s\n", timestamp);
}