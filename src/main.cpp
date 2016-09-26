#include <iostream>
#include <deque>
#include <cstdlib>
#include <ctime>

#include <ncurses.h>

#include "../include/ProcessPanel.h"
#include "../include/BatchProcessesPanel.h"
#include "../include/FinishedProcessesPanel.h"
#include "../include/util.h"

#define INTERRUPT_KEY 'e'
#define ERROR_KEY 'w'
#define PAUSE_KEY 'p'
#define CONTINUE_KEY 'c'

using namespace std;

void initialize();
void finalize();

void generateProcesses(int num_of_processes);
void setBatchesLeftCounter(unsigned int batches_left);
void setTotalTimeCounter(unsigned int batches_left);
void startSimulation();
void handleKey(int key);
void interrupt();
void printControls();

WINDOW *inner_win;
WINDOW *help_win;
deque<Batch *> pending_batches;
Batch finished_processes;

Process *current_process;
Batch *current_batch;

// info panels
ProcessesListPanel *batch_panel, *finished_panel;
ProcessPanel *process_panel;

int main(void) {
    int num_of_processes;

    initialize();

    // get number of processes
    wprintw(inner_win, "Numero de procesos: ");
    wscanw(inner_win, "%d", &num_of_processes);

    generateProcesses(num_of_processes);

    batch_panel->post();
    process_panel->post();
    finished_panel->post();

    setTotalTimeCounter(0);
    setBatchesLeftCounter(pending_batches.size());
    printControls();

    curs_set(0);

    wgetch(inner_win);
    startSimulation();
    wgetch(inner_win);

    finalize();

    return 0;
}

void initialize() {
    srand(time(NULL));

    // initialize ncurses
    initscr();
    cbreak();

    // set up main window
    box(stdscr, 0, 0);
    printCentered(stdscr, "PROCESAMIENTO POR LOTES", 1, A_BOLD);
    wnoutrefresh(stdscr);

    // create subwindow for content
    inner_win = derwin(stdscr, LINES - 4, COLS - 2, 3, 1);
    keypad(inner_win, TRUE);

    // set up panels
    batch_panel = new BatchProcessesPanel(inner_win, 10, 15, 3, 0);
    process_panel = new ProcessPanel(inner_win, 10, 25, 3, 15);
    finished_panel = new FinishedProcessesPanel(inner_win, 17, 37, 3, 40);

    help_win = derwin(inner_win, 7, 40, 13, 0);
    syncok(help_win, TRUE);
}

void finalize() {
    // free memory
    delete(process_panel);
    delete(batch_panel);
    delete(finished_panel);

    delwin(inner_win);
    delwin(help_win);

    while (!finished_processes.empty()) {
        delete(finished_processes.back());
        finished_processes.pop_back();
    }

    endwin();
}

void generateProcesses(int num_of_processes) {
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

void startSimulation() {
    unsigned int total_time;

    time_t last_time;
    time_t new_time;
    time_t time_step;

    total_time = 0;

    nodelay(inner_win, TRUE);

    while (!pending_batches.empty()) {
        // get first batch
        current_batch = pending_batches.front();
        pending_batches.pop_front();

        // update batchers counter
        setBatchesLeftCounter(pending_batches.size());

        while (!current_batch->empty()) {
            // get first process
            current_process = current_batch->front();
            current_batch->pop_front();

            // display information
            batch_panel->setProcesses(current_batch);
            process_panel->display(current_process);

            time(&last_time);

            while (current_process->elapsed_time < current_process->estimated_time) {
                time(&new_time);

                time_step = new_time - last_time;

                current_process->elapsed_time += time_step;
                total_time += time_step;
                last_time += time_step;

                process_panel->setElapsedTime(current_process->elapsed_time);
                setTotalTimeCounter(total_time);

                handleKey(wgetch(inner_win));
            }

            current_process->run();
            finished_processes.push_back(current_process);

            finished_panel->setProcesses(&finished_processes);
        }
    }

    process_panel->clear();
    setBatchesLeftCounter(0);

    nodelay(inner_win, FALSE);
}

void handleKey(int key) {
    switch (key) {
        case INTERRUPT_KEY:
            interrupt();
            break;
        case ERROR_KEY:
            break;
        case PAUSE_KEY:
            break;
        default:
            break;
    }
}

void interrupt() {
    current_batch->push_back(current_process);
    current_process = current_batch->front();
    current_batch->pop_front();

    batch_panel->setProcesses(current_batch);
    process_panel->display(current_process);
}

void setBatchesLeftCounter(unsigned int num_of_batches) {
    wmove(inner_win, 0, 0);
    wclrtoeol(inner_win);
    wprintw(inner_win, "Lotes pendientes: %lu", num_of_batches);
}

void setTotalTimeCounter(unsigned int total_time) {
    wmove(inner_win, 1, 0);
    wclrtoeol(inner_win);
    wprintw(inner_win, "Tiempo transcurrido: %lu", total_time);
}

void printControls() {
    wmove(help_win, 0, 0);
    wprintw(help_win, "%c = interrupcion de E/S \n", INTERRUPT_KEY);
    wprintw(help_win, "%c = error \n", ERROR_KEY);
    wprintw(help_win, "%c = pausa \n", PAUSE_KEY);
    wprintw(help_win, "%c = continuar \n", CONTINUE_KEY);
}
