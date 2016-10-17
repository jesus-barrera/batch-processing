#include <sstream>
#include <iomanip>

#include "ui/scheduler/FinishedProcessesPanel.h"

FinishedProcessesPanel::FinishedProcessesPanel(
    WINDOW *parent,
    int nlines, int ncols,
    int begin_x, int begin_y)

    : ProcessesListPanel(parent, "Procesos terminados", nlines, ncols, begin_x, begin_y) {

    std::stringstream heading;

    // set column heading
    heading << setw(2) << "ID" << " | "
            << " Operacion";

    setColumnsHeading(heading.str());
}

void FinishedProcessesPanel::printProcess(Process *process) {
    if (process->termination_status == Process::SUCCESS) {
        wprintw(
            inner_win,
            "%-2d | %-4d %-2s %-4d = %-4d",
            process->program_number,
            process->left_operand,
            Process::operators[process->operation],
            process->right_operand,
            process->result
        );
    } else {
        wprintw(
            inner_win,
            "%-2d | %-4d %-2s %-4d = %-4s",
            process->program_number,
            process->left_operand,
            Process::operators[process->operation],
            process->right_operand,
            "ERROR!"
        );
    }
}
