#include <iostream>
#include <ncurses.h>

using namespace std;

int maxRows, maxCols;

void printCentered(string text);

int main(void) {
    int key;
    int numOfProcesses;

    initscr();
    wborder(stdscr, '|', '|', '-', '-', '+', '+', '+', '+');
    getmaxyx(stdscr, maxRows, maxCols);
    move(1, 0);
    printCentered("Procesamiento por lotes");
    move(2, 0);
    printCentered("Numero de procesos: ");
    scanw("%d", &numOfProcesses);
    printw("%d", numOfProcesses);
    getch();
    endwin();

    return 0;
}

void printCentered(string text) {
    int x, y;

    getyx(stdscr, y, x);

    x = (maxCols - text.size()) / 2;

    mvprintw(y, x, text.c_str());
}
