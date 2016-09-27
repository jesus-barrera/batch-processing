#include <cstdlib>
#include <ctime>

#include "../include/screen.h"
#include "../include/BatchProcessing.h"
#include "../include/util.h"

void initialize();
void finalize();

WINDOW *screen;
BatchProcessing *batch_processing;

int main(void) {
    int num_of_processes;

    initialize();

    // get number of processes
    wprintw(screen, "Numero de procesos: ");
    wscanw(screen, "%d", &num_of_processes);

    batch_processing->generateProcesses(num_of_processes);
    batch_processing->post();

    batch_processing->setMessage("Presiona una tecla para comenzar...");
    wgetch(screen);

    batch_processing->runSimulation();

    batch_processing->setMessage("Presiona una tecla para salir...");
    wgetch(screen);

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
    screen = derwin(stdscr, SCREEN_LINES, SCREEN_COLS, 3, 1);
    keypad(screen, TRUE);

    batch_processing = new BatchProcessing();
}

void finalize() {
    delwin(screen);
    endwin();
}
