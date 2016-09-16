#include "../include/BatchPanel.h"

BatchPanel::BatchPanel(
        WINDOW *parent,
        std::string title,
        int nlines, int ncols,
        int begin_x, int begin_y)

        : Panel(parent, title, nlines, ncols, begin_x, begin_y) {
}

void BatchPanel::setPrintRowFunc(PrintRowFunc func) {
    print_row_func = func;
}

void BatchPanel::setHeader(string &str) {
    header = str;
}

void BatchPanel::display(Batch *batch) {
    Batch::iterator it;
    Process *process;
    int row;

    row = 1; // skip title row

    wmove(subwin, row, 0);
    wclrtobot(subwin); // clear data

    for (it = batch->begin(); it != batch->end(); it++, row++) {
        wmove(subwin, row, 0);
        print_row_func(subwin, *it);
    }
}

void BatchPanel::post() {
    Panel::post();
    mvwprintw(subwin, 0, 0, header.c_str());
}
