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

    ProcessScheduler();
    ~ProcessScheduler();

    /**
     * Post simulation interface.
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

private:
    enum {
        READY_PANEL,
        BLOCKED_PANEL,
        PROCESS_PANEL,
        TERMINATED_PANEL,
        NUM_PANELS
    };

    WINDOW *panels_win;

    // counters
    Field<unsigned int> *new_processes_counter;
    Field<unsigned int> *total_time_counter;

    // info panels
    ReadyProcessesPanel *ready_panel;
    ProcessPanel *process_panel;
    BlockedProcessesPanel *blocked_panel;
    FinishedProcessesPanel *finished_panel;

    // lists of processes to store processes in each state
    ProcessList new_processes;
    ProcessList ready_processes;
    ProcessList blocked_processes;
    ProcessList finished_processes;

    Process *running_process;

    Timer timer;

    unsigned int time_step;

    /**
     *
     */
    void initPanels();

    /**
     * Loads as many processes as possible into memory.
     */
    int load();

    /**
     * Serves the next ready process.
     */
    bool serve();

    /**
     * Calculates how many processes are currently in memory.
     */
    int getTotalActiveProcesses();

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

    /**
     * Updates the screen to reflect the simulation state data.
     */
    void updateView();

    /**
     * Displays the list of controls.
     */
    void printHelp();
};

#endif
