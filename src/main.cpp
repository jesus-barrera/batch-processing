#include <cstdlib>
#include <ctime>

#include "ui/screen.h"
#include "ui/scheduler/MemoryView.h"
#include "ProcessScheduler.h"
#include "util.h"

void initialize();
void finalize();
void pause(std::string message);

ProcessScheduler *scheduler;

int main(void) {
    int num_of_processes;
    int quantum;

    initialize();

    wprintw(content, "Numero de procesos: ");
    wscanw(content, "%d", &num_of_processes);

    wprintw(content, "Quantum: ");
    wscanw(content, "%d", &quantum);

    curs_set(0); // hide cursor
    noecho();    // disable echoing

    scheduler->setQuantum(quantum);
    scheduler->generateProcesses(num_of_processes);
    scheduler->post();

    pause("Presiona una tecla para comenzar...");

    scheduler->runSimulation();

    pause("Presiona una tecla para continuar...");

    scheduler->showResults();

    pause("Presiona una tecla para salir...");

    finalize();

    return 0;
}

void pause(std::string message) {
    setFooter(message);
    getch();
}

void initialize() {
    srand(time(NULL));

    // initialize ncurses
    initscr();
    cbreak();
    start_color();

    MemoryView::init();
    startScreen();

    setHeader("PAGINACION SIMPLE");

    scheduler = new ProcessScheduler();
}

void finalize() {
    delete(scheduler);
    endScreen();
    endwin();
}
