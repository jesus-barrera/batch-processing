#include "ui/Panel.h"
#include "util.h"

const int Panel::BORDER_WIDTH = 1;

Panel::Panel(WINDOW *parent, std::string title, int nlines, int ncols, int begin_x, int begin_y) {
    // create outer window to draw borders and title
    outer_win = derwin(parent, nlines, ncols, begin_x, begin_y);

    // create inner window where actual content is written.
    inner_win = derwin(
        outer_win,
        nlines - BORDER_WIDTH * 2,
        ncols - BORDER_WIDTH * 2,
        BORDER_WIDTH,
        BORDER_WIDTH
    );

    this->title = title;

    syncok(inner_win, TRUE);
}

Panel::~Panel() {
    delwin(inner_win);
    delwin(outer_win);
}

/**
 * Writes borders and title to outer window.
 */
void Panel::post() {
    box(outer_win, 0, 0);
    printCentered(outer_win, title, 0, A_BOLD);
}

/**
 * Removes view's content.
 */
void Panel::unpost() {
    werase(outer_win);
    wsyncdown(inner_win);
}
