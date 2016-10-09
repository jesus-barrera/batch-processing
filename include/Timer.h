#ifndef _TIMER_INCLUDED_
#define _TIMER_INCLUDED_

#include <ctime>

#define NANOSECS 1000000000

class Timer {
private:
    // Stores the elapsed time until the last pause. The actual elapsed time can
    // be retrieved by getTime.
    struct timespec ts_elapsed;

    // Stores the time the timer start running after being paused or stopped.
    struct timespec ts_start;

    // Tells if the timer is currently running. If false, it could be either stopped
    // or paused.
    bool is_running;

public:
    Timer();

    /**
     * Starts the timer, either to resume it after a pause or simply start it when
     * stopped. If the timer is already running, this function has no effect.
     */
    void start();

    /**
     * Pauses the timer. Elapsed time is not added to the timer's time count so
     * subsecuent calls to getTime should always return the same value.
     */
    void pause();

    /**
     * Stops the timer; this sets the elapsed time to 0.
     */
    void stop();

    /**
     * Restarts the timer. Equivalent to call stop and then start.
     */
    void restart();

    /**
     * Calculates the time passed since the timer was started, removing any paused
     * intervals. This time is stored in ts_time.
     */
    void getTime(struct timespec &ts_time);

    /**
     * Same as getTime but returns the elapsed time in seconds as a single double
     * value.
     */
    double getSeconds();
};

#endif
