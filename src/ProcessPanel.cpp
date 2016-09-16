#include "../include/ProcessPanel.h"

const std::string ProcessPanel::labels[] = {
    "No. programa: ",
    "Nombre: ",
    "Operacion: ",
    "TE: ",
    "TT: "
};

ProcessPanel::ProcessPanel(WINDOW *parent, int nlines, int ncols, int begin_x, int begin_y)
        : Panel(parent, "Proceso en ejecucion", nlines, ncols, begin_x, begin_y) {
}

void ProcessPanel::post() {
    Panel::post();
    printLabels();
}

void ProcessPanel::display(Process *process) {
    setName(process->name);
    setProgramNumber(process->program_number);
    setOperation(process->operation, process->left_operand, process->right_operand);
    setEstimatedTime(process->estimated_time);
    setElapsedTime(process->elapsed_time);
}

void ProcessPanel::setProgramNumber(unsigned int program_number) {
    clearField(PROGRAM_NUMBER_FIELD);
    wprintw(subwin, "%lu", program_number);
}

void ProcessPanel::setName(std::string &name) {
    clearField(NAME_FIELD);
    wprintw(subwin, "%s", name.c_str());
}

void ProcessPanel::setOperation(char operation, int left_operand, int right_operand) {
    clearField(OPERATION_FIELD);
    wprintw(subwin, "%d %c %d", left_operand, operation, right_operand);
}

void ProcessPanel::setEstimatedTime(unsigned int estimated_time) {
    clearField(ESTIMATED_TIME_FIELD);
    wprintw(subwin, "%lu", estimated_time);
}

void ProcessPanel::setElapsedTime(unsigned int elapsed_time) {
    clearField(ELAPSED_TIME_FIELD);
    wprintw(subwin, "%lu", elapsed_time);
}

void ProcessPanel::clearField(int index) {
    wmove(subwin, index, labels[index].size());
    wclrtoeol(subwin);
}

void ProcessPanel::printLabels() {
    for (int i = 0; i < NUM_OF_FIELDS; i++) {
        mvwprintw(subwin, i, 0, labels[i].c_str());
    }
}
