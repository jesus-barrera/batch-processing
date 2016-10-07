#include <cstdlib>
#include <ctime>

#include "../include/screen.h"
#include "../include/ProcessScheduler.h"
#include "../include/util.h"

void initialize();
void finalize();

WINDOW *screen;
ProcessScheduler *scheduler;

int main(void) {
    int num_of_processes;

    initialize();

    // get number of processes
    wprintw(screen, "Numero de procesos: ");
    wscanw(screen, "%d", &num_of_processes);

    scheduler->generateProcesses(num_of_processes);
    scheduler->post();

    scheduler->waitForKey("Presiona una tecla para comenzar...");

    scheduler->runSimulation();

    scheduler->waitForKey("Presiona una tecla para continuar...");

    scheduler->showResults();

    scheduler->waitForKey("Presiona una tecla para salir...");

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

    scheduler = new ProcessScheduler();
}

void finalize() {
    delete(scheduler);
    delwin(screen);
    endwin();
}
