#include <sstream>

#include "../include/BatchProcessing.h"
#include "../include/screen.h"

BatchProcessing::BatchProcessing() {
    // counters
    batches_left_counter = new Field<unsigned int>(screen, "Lotes pendientes: ", 0, 0);
    total_time_counter   = new Field<unsigned int>(screen, "Tiempo total: ", 1, 0);

    // place panels
    batch_panel = new BatchProcessesPanel(screen, 10, 17, 3, 0);
    process_panel = new ProcessPanel(screen, 10, 25, 3, 17);
    finished_panel = new FinishedProcessesPanel(screen, 17, 35, 3, 42);

    help_win = derwin(screen, 7, 40, 13, 0);
    syncok(help_win, TRUE);
}

BatchProcessing::~BatchProcessing() {
    delete(batches_left_counter);
    delete(total_time_counter);

    delete(process_panel);
    delete(batch_panel);
    delete(finished_panel);

    delwin(help_win);

    while (!finished_processes.empty()) {
        delete(finished_processes.back());
        finished_processes.pop_back();
    }
}

/**
 * Writes all elements to screen.
 */
void BatchProcessing::post() {
    curs_set(0);
    noecho();

    // show counters
    total_time_counter->post();
    total_time_counter->setValue(0);

    batches_left_counter->post();
    batches_left_counter->setValue(pending_batches.size());

    // show panels
    batch_panel->post();
    process_panel->post();
    finished_panel->post();
}

/**
 * Prints a message in the help window. Previous content is erased.
 */
void BatchProcessing::setMessage(std::string message) {
    werase(help_win);
    mvwaddstr(help_win, 0, 0, message.c_str());
}

/**
 * Generates a given number of processes randomly. All processes are stored and
 * grouped in batches.
 */
void BatchProcessing::generateProcesses(int num_of_processes) {
    Batch *batch;
    Process *process;
    int count;

    count = 0;

    while (count < num_of_processes) {
        process = Process::newRandom();

        if (++count % PROCESSES_PER_BATCH == 1) {
            batch = new Batch();
            pending_batches.push_back(batch);
        }

        batch->push_back(process);
    }
}

/**
 * Run all the processes.
 */
void BatchProcessing::runSimulation() {
    nodelay(screen, TRUE);

    printHelp();
    timer.start();

    while (!pending_batches.empty()) {
        nextBatch();
        batches_left_counter->setValue(pending_batches.size());

        while (!current_batch->empty()) {
            nextProcess();

            updatePanels();

            if (run(current_process) == Process::INTERRUPTED) {
                current_batch->push_back(current_process);
                current_process->state = Process::READY;

            } else {
                finished_processes.push_back(current_process);
            }

            current_batch->pop_front();
        }

        delete(current_batch);
    }

    timer.pause();
    current_batch = NULL;
    current_process = NULL;

    updatePanels();

    nodelay(screen, FALSE);
}

/**
 * Run a single process.
 */
int BatchProcessing::run(Process *process) {
    unsigned int last_time;
    unsigned int current_time;

    last_time = timer.getTime();

    process->state = Process::RUNNING;

    while ((process->elapsed_time < process->estimated_time) &&
           (process->state == Process::RUNNING)) {

        current_time = timer.getTime();

        process->elapsed_time += current_time - last_time;
        last_time = current_time;

        updateTimers();

        handleKey(wgetch(screen), process);
    }

    if (process->state == Process::RUNNING) {
        process->run();
        process->state = Process::TERMINATED;
    }

    return process->state;
}

/**
 * Obtains the next process of the current batch. The process is left in the batch.
 */
void BatchProcessing::nextProcess() {
    current_process = current_batch->front();
}

/**
 * Obtains the next batch and removes it from the queue.
 */
void BatchProcessing::nextBatch() {
    current_batch = pending_batches.front();
    pending_batches.pop_front();
}

/**
 * Handles a key press for a runnig process.
 */
void BatchProcessing::handleKey(int key, Process *process) {
    switch (key) {
        case INTERRUPT_KEY:
            process->state = Process::INTERRUPTED;
            break;

        case ERROR_KEY:
            process->state = Process::ERROR;
            break;

        case PAUSE_KEY:
            pause();
            break;

        default:
            break;
    }
}

/**
 * Pauses the simulation.
 */
void BatchProcessing::pause() {
    timer.pause();

    setMessage("Pausado, presiona 'c' para continuar...");
    while (wgetch(screen) != CONTINUE_KEY);

    printHelp();
    timer.start();
}

/**
 * Updates all timers showing in screen: the global timer, the current
 * process elapsed time and time left.
 */
void BatchProcessing::updateTimers() {
    total_time_counter->setValue(timer.getTime());
    process_panel->setTimeLeft(current_process->getTimeLeft());
    process_panel->setElapsedTime(current_process->elapsed_time);
    batch_panel->setProcesses(current_batch); // TODO: update only the current process row
}

/**
 * Updates panels with the current data.
 */
void BatchProcessing::updatePanels() {
    process_panel->display(current_process);
    batch_panel->setProcesses(current_batch);
    finished_panel->setProcesses(&finished_processes);
}

/**
 * Shows a list of options in the help window.
 */
void BatchProcessing::printHelp() {
    stringstream message;

    message << INTERRUPT_KEY << " = interrupcion de E/S" << endl;
    message << ERROR_KEY << " = error" << endl;
    message << PAUSE_KEY << " = pausar" << endl;

    setMessage(message.str());
}
