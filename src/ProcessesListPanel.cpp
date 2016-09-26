#include "../include/ProcessesListPanel.h"

ProcessesListPanel::ProcessesListPanel(
        WINDOW *parent,
        std::string title,
        int nlines, int ncols,
        int begin_x, int begin_y)

        : Panel(parent, title, nlines, ncols, begin_x, begin_y) {

        scrollok(inner_win, TRUE);
}

void ProcessesListPanel::post() {
    int max_x;

    Panel::post();

    // fill heading with spaces.
    max_x = getmaxx(inner_win);
    columns_heading.append(max_x - columns_heading.size(), ' ');

    wattron(inner_win, A_REVERSE);
    mvwaddstr(inner_win, 0, 0, columns_heading.c_str());
    wattroff(inner_win, A_REVERSE);
}

/**
 * Displays a list of processes in the table.
 */
void ProcessesListPanel::setProcesses(Batch *batch) {
    int row;
    int count;

    row = 1; // skip column header row

    // clear previous data
    wmove(inner_win, row, 0);
    wclrtobot(inner_win);

    count = 0;

    while (count < batch->size()) {
        wmove(inner_win, row, 0);
        printProcess(batch->at(count++));

        // add extra space at the end of the batch
        row += ((count % PROCESSES_PER_BATCH) == 0) ? 2 : 1;
    }
}
