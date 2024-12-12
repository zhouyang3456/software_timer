#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "timer.h"

// Global head of the soft timer linked list
static SoftTimer *soft_timer_list_head = NULL;

// Initialize the software timer system
void init_soft_timer_system(void) {
    soft_timer_list_head = NULL;
}

// Create a new software timer
SoftTimer* create_soft_timer(unsigned long timeout_ms, unsigned char mode, void (*callback)(void *param), void *param) {
    SoftTimer *new_timer = (SoftTimer*)malloc(sizeof(SoftTimer));
    if (!new_timer) return NULL;

    new_timer->period = timeout_ms / SOFT_TIMER_TICK_MS;
    new_timer->timeout = timeout_ms / SOFT_TIMER_TICK_MS;
    new_timer->mode = mode;
    new_timer->status = SOFT_TIMER_STATUS_STOPPED;
    new_timer->callback = callback;
    new_timer->param = param;
    new_timer->next = NULL;

    // Add to the end of the list
    if (!soft_timer_list_head) {
        soft_timer_list_head = new_timer;
    } else {
        SoftTimer *current = soft_timer_list_head;
        while (current->next != NULL) current = current->next;
        current->next = new_timer;
    }

    return new_timer;
}

// Start a software timer
void start_soft_timer(SoftTimer *timer) {
    if (timer && timer->status == SOFT_TIMER_STATUS_STOPPED) {
        timer->timeout = timer->period;
        timer->status = SOFT_TIMER_STATUS_RUNNING;
    }
}

// Stop a software timer
void stop_soft_timer(SoftTimer *timer) {
    if (timer && timer->status == SOFT_TIMER_STATUS_RUNNING) {
        timer->status = SOFT_TIMER_STATUS_STOPPED;
    }
}

// Delete a software timer from the list and return whether deletion was successful
int delete_soft_timer(SoftTimer *timer) {
    if (!timer || !soft_timer_list_head) return 0; // 如果定时器为空或者列表为空，则返回失败

    // 特殊情况：如果要删除的是头节点
    if (soft_timer_list_head == timer) {
        soft_timer_list_head = timer->next;
        free(timer);
        return 1;
    }

    // 遍历链表寻找待删除的定时器
    SoftTimer *current = soft_timer_list_head;
    while (current->next != NULL && current->next != timer) {
        current = current->next;
    }

    // 检查是否找到了匹配的定时器
    if (current->next == timer) {
        current->next = timer->next;
        free(timer);
        return 1; // 删除成功
    }

    // 如果没有找到对应的定时器，则返回失败
    return 0;
}

void process_timers(void) {
    SoftTimer *current = soft_timer_list_head;
    SoftTimer *prev = NULL;

    while (current != NULL) {
        if (current->status == SOFT_TIMER_STATUS_RUNNING) {
            if (--current->timeout == 0) {
                current->callback(current->param);

                if (current->mode == SOFT_TIMER_MODE_PERIODIC) {
                    // 重置周期性定时器的超时值
                    current->timeout = current->period;
                } else {
                    stop_soft_timer(current);
                }
            }
        }

        prev = current;
        current = current->next;
    }
}