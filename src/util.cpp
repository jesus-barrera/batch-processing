#include "../include/util.h"

void printCentered(WINDOW *win, std::string text, int row, int attrs) {
    attron(attrs);
    mvwprintw(win, row, (getmaxx(win) - text.size()) / 2, text.c_str());
    attroff(attrs);
}

void clearLine(WINDOW *win, int line, bool has_border) {
    if (line < 0) line = getcury(win);

    if (has_border) {
        std::string blank_line(getmaxx(win) - 2, ' ');

        mvwprintw(win, line, 1, blank_line.c_str());
    } else {
        wmove(win, line, 0);
        wclrtoeol(win);
    }
}

std::string& trim(std::string &str) {
    std::size_t index;

    index = str.find_last_not_of(" ");

    str.erase(index + 1);

    return str;
}
