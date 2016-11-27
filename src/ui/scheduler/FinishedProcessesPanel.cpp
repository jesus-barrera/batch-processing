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

    setHeading(heading.str());
}

void FinishedProcessesPanel::printRow(Process *process) {
    stringstream row;

    row << left << setw(2) << process->pid << " | "
        << left << setw(3) << process->left_operand << " "
        << left << setw(2) << Process::operators[process->operation] << " "
        << left << setw(3) << process->right_operand << " = ";

        if (process->termination_status == Process::SUCCESS) {
            row << process->result;
        } else {
            row << "ERROR!";
        }

    waddstr(data_win, row.str().c_str());
}
