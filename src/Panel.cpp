#include "../include/Panel.h"
#include "../include/util.h"

const int Panel::BORDER_WIDTH = 1;

Panel::Panel(WINDOW *parent, std::string title, int nlines, int ncols, int begin_x, int begin_y) {
    win = derwin(parent, nlines, ncols, begin_x, begin_y);

    subwin = derwin(
        win,
        nlines - BORDER_WIDTH * 2,
        ncols - BORDER_WIDTH * 2,
        BORDER_WIDTH,
        BORDER_WIDTH
    );

    this->title = title;
}

Panel::~Panel() {
    delwin(subwin);
    delwin(win);
}

void Panel::post() {
    box(win, 0, 0);
    printCentered(win, title, 0, A_BOLD);
}
