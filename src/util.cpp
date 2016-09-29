#include <cstdlib>
#include "../include/util.h"

void printCentered(WINDOW *win, std::string text, int row, int attrs) {
    wattron(win, attrs);
    mvwaddstr(win, row, (getmaxx(win) - text.size()) / 2, text.c_str());
    wattroff(win, attrs);
}

std::string& trim(std::string &str) {
    std::size_t index;

    index = str.find_last_not_of(" ");

    str.erase(index + 1);

    return str;
}

/**
 * Generates a random number between min and max inclusive.
 */
int random(unsigned int min, unsigned int max) {
    return (rand() % (max - min + 1) + min);
}
