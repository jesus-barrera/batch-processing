#include <sstream>
#include <iomanip>

#include "ui/scheduler/ReadyProcessesPanel.h"

ReadyProcessesPanel::ReadyProcessesPanel(
    WINDOW *parent,
    int nlines, int ncols,
    int begin_x, int begin_y)

    : ProcessesListPanel(parent, "Procesos Listos", nlines, ncols, begin_x, begin_y) {

    std::stringstream heading;

    // set column heading
    heading << setw(2) << "ID" << " | "
            << setw(2) << "TE" << " | "
            << setw(2) << "TR";

    setHeading(heading.str());
}

void ReadyProcessesPanel::printRow(Process *process) {
    wprintw(
        data_win,
        "%-2lu | %-2lu | %-2lu",
        process->pid,
        process->estimated_time,
        process->getTimeLeft()
    );
}
