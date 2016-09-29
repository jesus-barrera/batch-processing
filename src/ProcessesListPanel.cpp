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

void ProcessesListPanel::setColumnsHeading(std::string heading) {
    int max_x;

    // fill heading with spaces.
    max_x = getmaxx(inner_win);
    heading.append(max_x - heading.size(), ' ');

    columns_heading = heading;
}

void ProcessesListPanel::post() {
    Panel::post();

    wattron(inner_win, A_REVERSE);
    mvwaddstr(inner_win, 0, 0, columns_heading.c_str());
    wattroff(inner_win, A_REVERSE);
}

/**
 * Displays a list of processes in the table.
 */
void ProcessesListPanel::setProcesses(Batch *batch) {
    unsigned int count;

    // clear previous data, skip column heading row
    wmove(inner_win, 1, 0);
    wclrtobot(inner_win);

    if (batch == NULL) return;

    count = 0;

    while (count < batch->size()) {
        printProcess(batch->at(count++));

        if (count != batch->size()) {
            /* use a line feed instead of moving the cursor one row down. This
               causes the screen to scroll up when reaching the last row */
            waddch(inner_win, '\n');

            // add an extra blank line at the end of the batch
            if ((count % PROCESSES_PER_BATCH) == 0) waddch(inner_win, '\n');
        }
    }
}
