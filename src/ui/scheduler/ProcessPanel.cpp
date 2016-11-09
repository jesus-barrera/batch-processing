#include <cstdarg>
#include "ui/scheduler/ProcessPanel.h"

const std::string ProcessPanel::labels[] = {
    "No. programa: ",
    "Nombre: ",
    "Operacion: ",
    "T. Estimado: ",
    "T. Transcurrido: ",
    "T. Restante: ",
    "Quantum: "
};

ProcessPanel::ProcessPanel(WINDOW *parent, int nlines, int ncols, int begin_x, int begin_y)
        : Panel(parent, "Proceso en ejecucion", nlines, ncols, begin_x, begin_y) {
}

void ProcessPanel::post() {
    Panel::post();
    printLabels();
}

/**
 * Fills the fields with the process data.
 */
void ProcessPanel::display(Process *process) {
    if (process != NULL) {
        setProgrammerName(process->programmer_name);
        setProgramNumber(process->program_number);
        setOperation(process->operation, process->left_operand, process->right_operand);
        setEstimatedTime(process->estimated_time);
        setElapsedTime(process->service_time);
        setTimeLeft(process->getTimeLeft());
    } else {
        clear();
    }
}

/**
 * Clears all fields. The labels are left.
 */
void ProcessPanel::clear() {
    for (int i = 0; i < NUM_OF_FIELDS; i++) {
        clearField(i);
    }
}

/**
 * Clears a field's value leaving the label. The pointer is left at the begining
 * of the field, this is, inmediatly after the field's label.
 */
void ProcessPanel::clearField(int index) {
    wmove(inner_win, index, labels[index].size());
    wclrtoeol(inner_win);
}

/* *** The following methods are used to set a field's value. *** */

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

void ProcessPanel::setTimeLeft(unsigned int time_left) {
    setField(TIME_LEFT_FIELD, "%lu", time_left);
}

void ProcessPanel::setQuantum(unsigned int quantum) {
    setField(QUANTUM_FIELD, "%lu", quantum);
}

/**
 * Sets the labels in place, one per row.
 */
void ProcessPanel::printLabels() {
    wattron(inner_win, A_DIM);

    for (int i = 0; i < NUM_OF_FIELDS; i++) {
        mvwprintw(inner_win, i, 0, labels[i].c_str());
    }

    wattroff(inner_win, A_DIM);
}

/**
 * Sets the value of a field by it's index. The previous value is first removed
 * before the new value is written. A string format describes how the value is
 * printed.
 */
void ProcessPanel::setField(int field_index, const char *format, ...) {
    va_list params;

    va_start(params, format);

    clearField(field_index);
    vwprintw(inner_win, format, params);

    va_end(params);
}
