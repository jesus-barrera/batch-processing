#include <iostream>
#include <deque>
#include <ctime>

#include <ncurses.h>

#include "../include/ProcessForm.h"
#include "../include/ProcessPanel.h"
#include "../include/BatchPanel.h"
#include "../include/util.h"

using namespace std;

void initPanels();
void captureProcesses(int num_of_processes);
void setBatchesLeftCounter(unsigned int batches_left);
void setTotalTimeCounter(unsigned int batches_left);
void execute();


WINDOW *content_win;
deque<Batch *> pending_batches;
Batch finished_processes;

// info panels
BatchPanel   *batch_panel, *finished_panel;
ProcessPanel *process_panel;

void printProcess(WINDOW *win, Process *process);
void printFinishedProcess(WINDOW *win, Process *process);

int main(void) {
    int num_of_processes;

    // Initialize ncurses
    initscr();
    cbreak();
    keypad(stdscr, TRUE);

    content_win = subwin(stdscr, LINES - 4, COLS - 4, 3, 2);

    // set panels
    process_panel  = new ProcessPanel(content_win, 10, 25, 4, 15);
    batch_panel    = new BatchPanel(content_win, "Lote actual", 10, 15, 4, 0);
    finished_panel = new BatchPanel(content_win, "Terminados", 10, 36, 4, 40);

    batch_panel->setPrintRowFunc(&printProcess);
    finished_panel->setPrintRowFunc(&printFinishedProcess);

    // set main window
    box(stdscr, 0, 0);
    printCentered(stdscr, "PROCESAMIENTO POR LOTES", 1, A_BOLD);
    wnoutrefresh(stdscr);

    mvwprintw(content_win, 0, 0, "Numero de procesos: ");
    wscanw(content_win, "%d", &num_of_processes);

    noecho();
    captureProcesses(num_of_processes);

    curs_set(0);

    batch_panel->post();
    process_panel->post();
    finished_panel->post();

    setTotalTimeCounter(0);
    setBatchesLeftCounter(pending_batches.size());

    execute();

    getch();

    delete(process_panel);
    delete(batch_panel);
    delete(finished_panel);
    delwin(content_win);

    endwin();

    return 0;
}

/**
 * shows the process
 */
void captureProcesses(int num_of_processes) {
    WINDOW *form_win;
    int key;
    int captured_processes;
    Batch *batch;
    Process *process;
    ProcessForm *form;

    form_win = derwin(content_win, getmaxy(content_win) - 3, getmaxx(content_win), 3, 0);
    keypad(form_win, true);

    form = new ProcessForm(form_win);
    form->post();

    captured_processes = 0;

    while (captured_processes < num_of_processes) {
        mvwprintw(content_win, 0, 0, "Procesos capturados: %d/%d", captured_processes, num_of_processes);
        wnoutrefresh(content_win);

        form->setCursor();

        do {
            while ((key = wgetch(form_win)) != '\n') {
                form->handleKey(key);
            }
        } while (!form->validate());

        // create process
        process = new Process();

        process->program_number = form->getProgramNumber();
        process->name           = form->getName();
        process->operation      = form->getOperation();
        process->left_operand   = form->getLeftOperand();
        process->right_operand  = form->getRightOperand();
        process->estimated_time = form->getEstimatedTime();

        if (++captured_processes % PROCESSES_PER_BATCH == 1) {
            batch = new Batch();
            pending_batches.push_back(batch);
        }

        batch->push_back(process);

        form->clear();
    }

    clearLine(content_win, 0);
    form->unpost();

    wnoutrefresh(content_win);

    delwin(form_win);
    delete(form);
}

void execute() {
    Batch *batch;
    Process *process;
    unsigned int total_time;

    time_t last_time;
    time_t new_time;
    time_t time_step;

    total_time = 0;

    while (!pending_batches.empty()) {
        // get first batch
        batch = pending_batches.front();
        pending_batches.pop_front();

        // update batchers counter
        setBatchesLeftCounter(pending_batches.size());

        while (!batch->empty()) {
            // get first process
            process = batch->front();
            batch->pop_front();

            // display information
            batch_panel->display(batch);
            process_panel->display(process);

            time(&last_time);

            while (process->elapsed_time < process->estimated_time) {
                time(&new_time);

                time_step = new_time - last_time;

                process->elapsed_time += time_step;
                total_time += time_step;
                last_time += time_step;

                process_panel->setElapsedTime(process->elapsed_time);
                setTotalTimeCounter(total_time);

                doupdate();
            }

            process->run();
            finished_processes.push_back(process);

            finished_panel->display(&finished_processes);
            doupdate();
        }
    }
}

void setBatchesLeftCounter(unsigned int num_of_batches) {
    mvwprintw(content_win, 0, 0, "Lotes pendientes: %lu", num_of_batches);
    wnoutrefresh(content_win);
}

void setTotalTimeCounter(unsigned int total_time) {
    mvwprintw(content_win, 2, 0, "Tiempo transcurrido: %lu", total_time);
    wnoutrefresh(content_win);
}

void printProcess(WINDOW *win, Process *process) {
    wprintw(win, "%-4lu | %-4lu", process->program_number, process->estimated_time);
}

void printFinishedProcess(WINDOW *win, Process *process) {
    wprintw(
        win,
        "%-4d | %-4d %c %-4d | %-4d",
        process->program_number,
        process->left_operand,
        process->operation,
        process->right_operand,
        process->result
    );
}
