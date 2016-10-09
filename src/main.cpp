#include <cstdlib>
#include <ctime>

#include "../include/screen.h"
#include "../include/ProcessScheduler.h"
#include "../include/util.h"

void initialize();
void finalize();
void pause(std::string message);

WINDOW *screen;
ProcessScheduler *scheduler;

int main(void) {
    int num_of_processes;

    initialize();

    wprintw(content, "Numero de procesos: ");
    wscanw(content, "%d", &num_of_processes);

    curs_set(0); // hide cursor
    noecho();    // disable echoing

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

    initscr();
    cbreak();

    startScreen();
    keypad(content, TRUE);

    setHeader("PROCESAMIENTO POR LOTES");

    scheduler = new ProcessScheduler();
}

void finalize() {
    delete(scheduler);
    endScreen();
    endwin();
}
