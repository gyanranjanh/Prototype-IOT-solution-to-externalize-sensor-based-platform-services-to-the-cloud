#include <xinu.h>

//#define TIMER_DEBUG
static timerList_t timerListH = {0};

static void timer_process(timerid id) {
    while (1) {
#ifdef TIMER_DEBUG
        kprintf("timer process going to sleep>>\n");
#endif
        receiveMsg();
#ifdef TIMER_DEBUG
        kprintf("timer fired>\n");
#endif
        timerListH.timerNode[id - 1].cb(timerListH.timerNode[id - 1].data);
    }
}

void init_timer_module() {
    uint32 count;

    for(count = 0; count < MAX_NO_TIMERS; count++) {
        timerListH.timerNode[count].id = INVALID_TIMER;
    }
}

timerid create_timer(timerCb cb, void *data, time ms) {
    uint32 indx = 0;
    timerid id = -1;

#ifdef TIMER_DEBUG
    kprintf("create_timer>>%d %x\n", ms, data);
#endif
    if (cb != NULL && ms != INVALID_TIME) {
        while (indx < MAX_NO_TIMERS) {
            if (timerListH.timerNode[indx].id == INVALID_TIMER) {
                break;
            }
            indx++;
        }

        if (indx < MAX_NO_TIMERS) {
            timerListH.count++;
            timerListH.timerNode[indx].isValid = FALSE;
            timerListH.timerNode[indx].id      = indx + 1;
            timerListH.timerNode[indx].data    = data;
            timerListH.timerNode[indx].cb      = cb;
            timerListH.timerNode[indx].start   = INVALID_TIME;
            timerListH.timerNode[indx].expiry  = ms;
            timerListH.timerNode[indx].timerp  = create(timer_process, 512, 
                                                    22, "timer", 1, timerListH.timerNode[indx].id);
            resume(timerListH.timerNode[indx].timerp);
            id = timerListH.timerNode[indx].id;
        }
    }
#ifdef TIMER_DEBUG
        kprintf("create_timer: timerid: %d<<\n", id);
#endif
    return id;
}

void start_timer(timerid id) {
    int32 indx = id - 1;

#ifdef TIMER_DEBUG
    kprintf("start_timer>>%d\n", id);
#endif
    if (indx > -1) {
        timerListH.timerNode[indx].isValid = TRUE;
        timerListH.timerNode[indx].start   = clktime_ms;
    }
    else {
        kprintf("failed to start timer\n");
    }
}

void stop_timer(timerid id) {
    int32 indx = id - 1;

#ifdef TIMER_DEBUG
    kprintf("stop_timer>>\n");
#endif
    if (indx > -1) {
        timerListH.timerNode[indx].isValid = FALSE;
    }
    else {
        kprintf("failed to stop timer\n");
    }
}

void cancel_timer(timerid id) {
    int32 indx = id - 1;

    kprintf("cancel_timer>>\n");
    if (indx > -1) {
        timerListH.timerNode[indx].isValid = FALSE;
        timerListH.timerNode[indx].id      = INVALID_TIMER;
        kill(timerListH.timerNode[indx].timerp);
    }
    else {
        kprintf("failed to stop timer\n");
    }
}

extern void fire_timers() {
    int32 indx;
    time exprdTime;

    resched_cntl(DEFER_START);
    for (indx = 0; indx < MAX_NO_TIMERS; indx++) {
        if (timerListH.timerNode[indx].isValid) {
            exprdTime = clktime_ms - timerListH.timerNode[indx].start;
            if (exprdTime > timerListH.timerNode[indx].expiry) {
                sendMsg(timerListH.timerNode[indx].timerp, TRUE);
                timerListH.timerNode[indx].start = clktime_ms;
            }
        }
    }
    resched_cntl(DEFER_STOP);
}
