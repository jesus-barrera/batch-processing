#include "../include/ProcessesListPanel.h"

ProcessesListPanel::ProcessesListPanel(
        WINDOW *parent,
        std::string title,
        int nlines, int ncols,
        int begin_x, int begin_y)

        : Panel(parent, title, nlines, ncols, begin_x, begin_y) {

        scrollok(inner_win, TRUE);
}

ProcessesListPanel::~ProcessesListPanel() { }

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
    // clear previous data, skip column header row
    wmove(inner_win, 1, 0);
    wclrtobot(inner_win);

    if (batch != NULL) {
        unsigned int count = 0;

        while (count < batch->size()) {
            printProcess(batch->at(count++));

            if (count != batch->size()) {
                waddch(inner_win, '\n');

                // add extra space at the end of the batch
                if ((count % PROCESSES_PER_BATCH) == 0) waddch(inner_win, '\n');
            }
        }
    }
}
