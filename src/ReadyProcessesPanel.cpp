#include <sstream>
#include <iomanip>

#include "../include/BatchProcessesPanel.h"

BatchProcessesPanel::BatchProcessesPanel(
    WINDOW *parent,
    int nlines, int ncols,
    int begin_x, int begin_y)

    : ProcessesListPanel(parent, "Lote actual", nlines, ncols, begin_x, begin_y) {

    std::stringstream heading;

    // set column heading
    heading << setw(4) << "ID" << " | "
            << setw(2) << "TE" << " | "
            << setw(2) << "TR";

    setColumnsHeading(heading.str());
}

void BatchProcessesPanel::printProcess(Process *process) {
    char *format;

    if (process->state == Process::RUNNING) {
        format = (char *)"> %-2lu | %-2lu | %-2lu";
    } else if (process->elapsed_time > 0){
        format = (char *)"* %-2lu | %-2lu | %-2lu";
    } else {
        format = (char *)"  %-2lu | %-2lu | %-2lu";
    }

    wprintw(
        inner_win,
        format,
        process->program_number,
        process->estimated_time,
        process->getTimeLeft()
    );
}
