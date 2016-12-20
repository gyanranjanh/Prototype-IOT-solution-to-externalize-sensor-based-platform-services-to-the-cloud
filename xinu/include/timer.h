#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef __cplusplus
extern "C" {
#endif


#define MAX_NO_TIMERS (10)
#define INVALID_TIMER (-1)
#define INVALID_TIME  (0xffffffff)

typedef void (*timerCb) (void *data);

typedef struct {
    pid32 timerp;
    bool8 isValid;
    time start;
    time expiry;
    timerid id;
    timerCb cb;
    void *data;
} timerNode_t;

typedef struct _timerList_t {
    uint32 count;
    timerNode_t timerNode[MAX_NO_TIMERS];
} timerList_t;

void init_timer_module();
timerid create_timer(timerCb cb, void *data, time ms);
void start_timer(timerid id);
void stop_timer(timerid id);
void cancel_timer(timerid id);

#ifdef __cplusplus
} //end extern "C"
#endif

#endif
