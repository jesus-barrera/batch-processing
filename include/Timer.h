#ifndef _TIMER_INCLUDED_
#define _TIMER_INCLUDED_

#include <ctime>

class Timer {
public:
    clock_t total_ticks;
    clock_t last_tick;

    bool is_paused;

    Timer();

    void start();
    void pause();
    void stop();
    void restart();

    clock_t getTicks();
    double getTime();
};

#endif
