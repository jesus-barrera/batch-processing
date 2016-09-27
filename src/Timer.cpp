#include "../include/Timer.h"

clock_t total_ticks;
clock_t last_tick;

Timer::Timer() {
    stop();
}

void Timer::start() {
    if (is_paused) {
        last_tick = clock();
        is_paused = false;
    }
}

void Timer::pause() {
    total_ticks = getTicks();
    is_paused = true;
}

void Timer::stop() {
    total_ticks = 0;
    is_paused = true;
}

void Timer::restart() {
    stop();
    start();
}

clock_t Timer::getTicks() {
    if (is_paused) {
        return total_ticks;
    } else {
        return total_ticks + clock() - last_tick;
    }
}
double Timer::getTime() {
    return (double)getTicks() / CLOCKS_PER_SEC;
}
