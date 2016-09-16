#include "../include/BatchPanel.h"

BatchPanel::BatchPanel(WINDOW *parent, int nlines, int ncols, int begin_x, int begin_y)
        : Panel(parent, "Lote actual", nlines, ncols, begin_x, begin_y) {


}

void BatchPanel::post() {
    Panel::post();
    mvwprintw(subwin, 0, 0, "%-4s %-4s", "ID", "TME");
}

void BatchPanel::display(Batch *batch) {
    Batch::iterator it;
    Process *process;
    int row;

    row = 1; // skip title row

    wmove(subwin, row, 0);
    wclrtobot(subwin); // clear data

    for (it = batch->begin(); it != batch->end(); it++, row ++) {
        process = *it;
        mvwprintw(subwin, row, 0, "%-4d %-4d", process->program_number, process->estimated_time);
    }
}
