#include <sstream>
#include <iomanip>

#include "ui/scheduler/BlockedProcessesPanel.h"

BlockedProcessesPanel::BlockedProcessesPanel(
    WINDOW *parent,
    int nlines, int ncols,
    int begin_x, int begin_y)

    : ProcessesListPanel(parent, "Bloqueados", nlines, ncols, begin_x, begin_y) {

    std::stringstream heading;

    // set column heading
    heading << setw(2) << "ID" << " | "
            << setw(3) << "TTB";

    setHeading(heading.str());
}

void BlockedProcessesPanel::printRow(Process *process) {
    wprintw(
        data_win,
        "%-2lu | %-3lu",
        process->program_number,
        process->blocked_time
    );
}
