#ifndef _BATCH_PROCESSING_INCLUDED_
#define _BATCH_PROCESSING_INCLUDED_

#include <string>

#include "../include/ProcessPanel.h"
#include "../include/ReadyProcessesPanel.h"
#include "../include/BlockedProcessesPanel.h"
#include "../include/FinishedProcessesPanel.h"
#include "../include/Field.h"
#include "../include/Timer.h"

using namespace std;

class ProcessScheduler {
public:
    static const int MAX_ACTIVE_PROCESSES = 5;
    static const int MAX_BLOCKED_TIME = 8;

    static const char INTERRUPT_KEY = 'e';
    static const char ERROR_KEY     = 'w';
    static const char PAUSE_KEY     = 'p';
    static const char CONTINUE_KEY  = 'c';

    static const int ANY_KEY = -1;

    ProcessScheduler();
    ~ProcessScheduler();

    void post();
    void setMessage(string message);
    void generateProcesses(int num_of_processes);
    void waitForKey(string message, int key = ANY_KEY);
    void runSimulation();
    void showResults();

private:
    WINDOW *help_win;

    // counters
    Field<unsigned int> *new_processes_counter;
    Field<unsigned int> *total_time_counter;

    // info panels
    ReadyProcessesPanel *ready_panel;
    ProcessPanel *process_panel;
    BlockedProcessesPanel *blocked_panel;
    FinishedProcessesPanel *finished_panel;

    ProcessList new_processes;
    ProcessList ready_processes;
    ProcessList blocked_processes;
    ProcessList finished_processes;

    Process *running_process;

    Timer timer;

    unsigned int time_step;

    int load(int num);
    void update();
    void updateRunningProcess();
    void updateBlockedProcesses();
    bool serve();
    void terminate(short reason);
    void handleKey(int key);
    void interrupt();
    void pause();

    void updateView();
    void printHelp();
};

#endif
