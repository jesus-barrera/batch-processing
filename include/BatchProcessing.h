#ifndef _BATCH_PROCESSING_INCLUDED_
#define _BATCH_PROCESSING_INCLUDED_

#include <deque>
#include <string>

#include "../include/ProcessPanel.h"
#include "../include/BatchProcessesPanel.h"
#include "../include/FinishedProcessesPanel.h"
#include "../include/Field.h"
#include "../include/Timer.h"

using namespace std;

class BatchProcessing {
public:
    static const char INTERRUPT_KEY = 'e';
    static const char ERROR_KEY     = 'w';
    static const char PAUSE_KEY     = 'p';
    static const char CONTINUE_KEY  = 'c';

    BatchProcessing();
    ~BatchProcessing();

    void post();
    void setMessage(string message);
    void generateProcesses(int num_of_processes);
    void runSimulation();

private:
    WINDOW *help_win;

    // counters
    Field<unsigned int> *batches_left_counter;
    Field<unsigned int> *total_time_counter;

    // info panels
    BatchProcessesPanel *batch_panel;
    FinishedProcessesPanel *finished_panel;
    ProcessPanel *process_panel;

    deque<Batch *> pending_batches;
    Batch finished_processes;

    Process *current_process;
    Batch *current_batch;

    Timer timer;

    int run(Process *process);
    void nextProcess();
    void nextBatch();

    void handleKey(int key, Process *process);
    void pause();

    void updateTimers();
    void updatePanels();
    void printHelp();
};

#endif
