#include "Timer.h"

Timer::Timer() {
    stop();
}

void Timer::start() {
    if (!is_running) {
        clock_gettime(CLOCK_MONOTONIC, &ts_start); // save start time
        is_running = true;
    }
}

void Timer::pause() {
    getTime(ts_elapsed); // save time until this point
    is_running = false;
}

void Timer::stop() {
    ts_elapsed = {0, 0}; // reset time
    is_running = false;
}

void Timer::restart() {
    stop();
    start();
}

void Timer::getTime(struct timespec &ts_time) {
    struct timespec ts_current;
    struct timespec ts_elapsed;

    if (is_running) {
        clock_gettime(CLOCK_MONOTONIC, &ts_current);

        // calculate elapsed time since start
        ts_elapsed.tv_sec = ts_current.tv_sec - ts_start.tv_sec;

        if (ts_current.tv_nsec >= ts_start.tv_nsec) {
            ts_elapsed.tv_nsec = ts_current.tv_nsec - ts_start.tv_nsec;
        } else {
            ts_elapsed.tv_sec -= 1;
            ts_elapsed.tv_nsec = NANOSECS - (ts_start.tv_nsec - ts_current.tv_nsec);
        }

        // add previous time
        ts_elapsed.tv_sec += this->ts_elapsed.tv_sec;
        ts_elapsed.tv_nsec += this->ts_elapsed.tv_nsec;

        if (ts_elapsed.tv_nsec >= NANOSECS) {
            ts_elapsed.tv_nsec -= NANOSECS;
            ts_elapsed.tv_sec += 1;
        }

        ts_time = ts_elapsed;
    } else {

        ts_time = this->ts_elapsed;
    }
}

double Timer::getSeconds() {
    static struct timespec elapsed;

    getTime(elapsed);

    return elapsed.tv_sec + (double)elapsed.tv_nsec / NANOSECS;
}
