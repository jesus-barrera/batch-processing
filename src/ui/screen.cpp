#include "ui/screen.h"
#include "util.h"

WINDOW *header;
WINDOW *content;
WINDOW *footer;

void startScreen() {
    header = derwin(stdscr, HEADER_LINES, COLS, 0, 0);
    content = derwin(stdscr, CONTENT_LINES, COLS, HEADER_LINES, 0);
    footer = derwin(stdscr, FOOTER_LINES, COLS, HEADER_LINES + CONTENT_LINES, 0);

    syncok(header, TRUE);
    syncok(content, TRUE);
    syncok(footer, TRUE);

    wattron(footer, A_REVERSE);
    setFooter("");
}

void endScreen() {
    delwin(header);
    delwin(content);
    delwin(footer);
}

void setHeader(std::string title) {
    printCentered(header, title, 1, A_BOLD);
    wnoutrefresh(header);
}

void setFooter(std::string message) {
    message.append(COLS - message.size(), ' ');
    mvwaddstr(footer, 0, 0, message.c_str());
    wnoutrefresh(footer);
}
