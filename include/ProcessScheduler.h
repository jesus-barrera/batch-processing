#ifndef _PROCESS_SCHEDULER_INCLUDED_
#define _PROCESS_SCHEDULER_INCLUDED_

#include <string>

#include "ProcessList.h"
#include "Timer.h"
#include "MemoryManager.h"

class ProcessSchedulerView;

using namespace std;

class ProcessScheduler {
public:
    static const int MAX_BLOCKED_TIME = 8;
    static const int GETCH_TIMEOUT = 500;

    static const string HELP;

    ProcessScheduler();
    ~ProcessScheduler();

    /**
     * Sets the quantum value, this is the maximum cpu time given the running
     * process.
     */
    void setQuantum(int quantum);

    /**
     * Post all UI elements into the screen.
     */
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
    // list to store the process on each state.
    ProcessList new_processes;
    ProcessList ready_processes;
    ProcessList blocked_processes;
    ProcessList terminated_processes;
    ProcessList pcb_table;

    Process *running_process;
    Process *next_process;

    Timer timer;

    unsigned int global_time;
    unsigned int num_of_processes;
    unsigned int quantum;
    unsigned int cpu_time;

    ProcessSchedulerView *view;
    MemoryManager memory;

    void initSimulation();
    void endSimulation();
    void update();
    int load();
    void serve();
    int getTotalActiveProcesses();
    void updateReadyProcesses();
    void updateRunningProcess();
    void updateBlockedProcesses();
    void terminate(short reason);
    void handleKey(int key);
    void suspend();
    void interrupt();
    void pause();
    void enterPause();
    void leavePause();
    void showBCPTable();
    void showPageTable();
};

#endif
