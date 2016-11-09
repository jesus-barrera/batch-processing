#ifndef _PROCESS_SCHEDULER_INCLUDED_
#define _PROCESS_SCHEDULER_INCLUDED_

#include <string>

#include "ProcessList.h"
#include "Timer.h"

class ProcessSchedulerView;

using namespace std;

class ProcessScheduler {
public:
    static const int MAX_ACTIVE_PROCESSES = 5;
    static const int MAX_BLOCKED_TIME = 8;
    static const int GETCH_TIMEOUT = 500;

    static const string HELP;

    ProcessScheduler();
    ~ProcessScheduler();

    void setQuantum(int quantum);

    void post();

    /**
     * Randomly generates num_of_processes processes.
     */
    void generateProcesses(int num_of_processes);

    /**
     * Runs the simulation.
     */
    void runSimulation();

    /**
     * Shows terminated processes information.
     */
    void showResults();

    friend ProcessSchedulerView;

private:
    // lists of processes to store processes in each state
    ProcessList new_processes;
    ProcessList ready_processes;
    ProcessList blocked_processes;
    ProcessList terminated_processes;
    ProcessList pcb_table;

    Process *running_process;

    Timer timer;

    unsigned int global_time;
    unsigned int num_of_processes;
    unsigned int quantum;
    unsigned int cpu_time;

    ProcessSchedulerView *view;

    void initSimulation();

    void endSimulation();

    void update();

    /**
     * Loads as many processes as possible into memory.
     */
    int load();

    /**
     * Serves the next ready process.
     */
    void serve();

    /**
     * Calculates how many processes are currently in memory.
     */
    int getTotalActiveProcesses();

    void updateReadyProcesses();

    /**
     * Updates the elapsed time of the running process and checks if it's terminated.
     */
    void updateRunningProcess();

    /**
     * Checks each blocked process and moves it back to the ready list when needed.
     */
    void updateBlockedProcesses();

    /**
     * Terminates the running process with the given reason.
     */
    void terminate(short reason);

    void suspend();

    /**
     * Handles a key press during simulation.
     */
    void handleKey(int key);

    /**
     * Sends the running process to the blocked list.
     */
    void interrupt();

    /**
     * Pauses the simulation until the continue key is press.
     */
    void pause();

    void enterPause();

    void leavePause();

    void showBCPTable();
};

#endif
