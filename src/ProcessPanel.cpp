#include <cstdarg>
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
    setProgrammerName(process->programmer_name);
    setProgramNumber(process->program_number);
    setOperation(process->operation, process->left_operand, process->right_operand);
    setEstimatedTime(process->estimated_time);
    setElapsedTime(process->elapsed_time);
}

void ProcessPanel::clear() {
    for (int i = 0; i < NUM_OF_FIELDS; i++) {
        clearField(i);
    }
}

void ProcessPanel::clearField(int index) {
    wmove(subwin, index, labels[index].size());
    wclrtoeol(subwin);
}

void ProcessPanel::setProgramNumber(unsigned int program_number) {
    setField(PROGRAM_NUMBER_FIELD, "%lu", program_number);
}

void ProcessPanel::setProgrammerName(std::string &name) {
    setField(PROGRAMMER_NAME_FIELD, "%s", name.c_str());
}

void ProcessPanel::setOperation(short operation, int left_operand, int right_operand) {
    setField(OPERATION_FIELD, "%d %s %d", left_operand, Process::operators[operation], right_operand);
}

void ProcessPanel::setEstimatedTime(unsigned int estimated_time) {
    setField(ESTIMATED_TIME_FIELD, "%lu", estimated_time);
}

void ProcessPanel::setElapsedTime(unsigned int elapsed_time) {
    setField(ELAPSED_TIME_FIELD, "%lu", elapsed_time);
}

void ProcessPanel::printLabels() {
    for (int i = 0; i < NUM_OF_FIELDS; i++) {
        mvwprintw(subwin, i, 0, labels[i].c_str());
    }
}

void ProcessPanel::setField(int field_index, const char *format, ...) {
    va_list params;

    va_start(params, format);

    clearField(field_index);
    vwprintw(subwin, format, params);

    va_end(params);
}
