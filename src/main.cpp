#include <iostream>
#include <vector>
#include <ncurses.h>
#include <panel.h>
#include <ctime>

#include "../include/ProcessForm.h"
#include "../include/Process.h"

#define PROCESSES_PER_BATCH 5

using namespace std;

typedef vector<Process *> Batch;

enum {
    BATCH_SCREEN,
    PROCESS_SCREEN,
    FINISHED_SCREEN,
    NUM_OF_SCREENS
};

void printCentered(string text, int row = -1);
void clearLine(int row);
void initPanels();
void captureProcesses();
void showBatch(Batch *batch);
void showProcess(Process *process);
void updateBatchCounter();
void updateTimeCounter();
void execute();

int max_rows, max_cols;
vector<Batch *> pending_batches;
vector<Batch *> finished_batches;
WINDOW *windows[NUM_OF_SCREENS];
PANEL *panels[NUM_OF_SCREENS];
int num_of_processes;
int current_batch;
unsigned int total_time;

int main(void) {
    // Initialize ncurses
    initscr();
    cbreak();
    keypad(stdscr, TRUE);

    // Start colors
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    attron(COLOR_PAIR(1));

    // set up
    getmaxyx(stdscr, max_rows, max_cols);

    printw("Numero de procesos: ");
    scanw("%d", &num_of_processes);

    noecho();
    captureProcesses();
    curs_set(0);
    initPanels();
    updateBatchCounter();
    updateTimeCounter();
    update_panels();
    doupdate();

    execute();

    getch();
    endwin();

    return 0;
}

void captureProcesses() {
    int key;
    int captured_processes;
    Batch *batch;
    Process *process;
    ProcessForm form(2, 0);

    captured_processes = 0;

    while (captured_processes < num_of_processes) {
        mvprintw(0, 0, "Procesos capturados: %d/%d", captured_processes, num_of_processes);

        form.goToFirstField();

        do {
            while ((key = getch()) != '\n') {
                form.handleKey(key);
            }
        } while (!form.validate());

        // create process
        process = new Process();

        process->program_number = form.getProgramNumber();
        process->name           = form.getName();
        process->operation      = form.getOperation();
        process->left_operand   = form.getLeftOperand();
        process->right_operand  = form.getRightOperand();
        process->estimated_time = form.getEstimatedTime();

        if (++captured_processes % PROCESSES_PER_BATCH == 1) {
            batch = new Batch();
            pending_batches.push_back(batch);
        }

        batch->push_back(process);

        form.clear();
    }
}

void execute() {
    Batch *batch;
    Process *process;

    time_t last_time;
    time_t new_time;
    time_t time_step;

    while (!pending_batches.empty()) {
        batch = pending_batches.front();

        showBatch(batch);

        while (!batch->empty()) {
            process = batch->front();

            showProcess(process);
            doupdate();

            time(&last_time);

            while (process->elapsed_time < process->estimated_time) {
                time(&new_time);

                time_step = new_time - last_time;

                process->elapsed_time += time_step;
                total_time += time_step;
                last_time += time_step;

                showProcess(process);
                updateTimeCounter();
            }

            process->run();
            batch->erase(batch->begin());
        }

        pending_batches.erase(pending_batches.begin());
    }
}

void initPanels() {
    // set windows
    windows[BATCH_SCREEN]    = newwin(15, 25, 2, 0);
    windows[PROCESS_SCREEN]  = newwin(15, 25, 2, 25);
    windows[FINISHED_SCREEN] = newwin(max_rows, 30, 0, 50);

    // set panels
    panels[BATCH_SCREEN]    = new_panel(windows[BATCH_SCREEN]);
    panels[PROCESS_SCREEN]  = new_panel(windows[PROCESS_SCREEN]);
    panels[FINISHED_SCREEN] = new_panel(windows[FINISHED_SCREEN]);

    for (int i = 0; i < NUM_OF_SCREENS; i++) {
        box(windows[i], 0, 0);
    }
}

void printCentered(string text, int row) {
    int col;

    if (row == -1) {
        getyx(stdscr, row, col);
    }

    col = (max_cols - text.size()) / 2;

    mvprintw(row, col, text.c_str());
}

void updateBatchCounter() {
    mvprintw(0, 0, "Lotes pendientes: %d", pending_batches.size());
}

void updateTimeCounter() {
    mvprintw(max_rows - 1, 0, "Tiempo transcurrido: %lu", total_time);
    refresh();
}

void clearLine(int row) {
    string blank_line(max_cols - 2, ' ');

    mvprintw(row, 1, blank_line.c_str());
}

void showBatch(Batch *batch) {
    WINDOW *win;
    Batch::iterator it;
    Process *process;

    win = windows[BATCH_SCREEN];
    mvwprintw(win, 0, 2, "Lote actual");
    mvwprintw(win, 1, 2, "%4s %4s", "ID", "TME");

    int row = 2;

    for (it = batch->begin(); it != batch->end(); it++, row += 1) {
        process = *it;
        mvwprintw(win, row, 2, "%4d %4d", process->program_number, process->estimated_time);
    }

    wrefresh(win);
}

void showProcess(Process *process) {
    WINDOW *win;

    win = windows[PROCESS_SCREEN];

    mvwprintw(win, 0, 2, "Proceso en ejecucion");
    mvwprintw(win, 1, 1, "Nombre: %s", process->name.c_str());
    mvwprintw(win, 2, 1, "ID: %d", process->program_number);
    mvwprintw(win, 3, 1, "Op: %d %c %d", process->left_operand, process->operation, process->right_operand, process->result);
    mvwprintw(win, 4, 1, "TME: %lu", process->estimated_time);
    mvwprintw(win, 5, 1, "TR: %-5lu", process->estimated_time - process->elapsed_time);
    wrefresh(win);
}
