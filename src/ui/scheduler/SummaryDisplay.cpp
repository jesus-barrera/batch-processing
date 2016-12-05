#include "ui/scheduler/SummaryDisplay.h"
#include "ui/screen.h"
#include "Process.h"

const std::string SummaryDisplay::labels[NUM_OF_FIELDS] = {
    "Tiempo Global: ",
    "Quantum: ",
    "Nuevos: ",
    "Sig. PID: ",
    "Sig. Tam: ",
    "Suspendidos: ",
    "Sig. PID: ",
    "Sig. Tam: "
};

SummaryDisplay::SummaryDisplay(WINDOW *window) {
    this->window = window;

    initFields();
    initForm();
}

SummaryDisplay::~SummaryDisplay() {
    free_form(form);

    for (FIELD *field: fields) {
        free_field(field);
    }
}

void SummaryDisplay::setQuantum(int quantum) {
    setFieldValue(QUANTUM_FIELD, quantum);
}

void SummaryDisplay::setNewCount(int num) {
    setFieldValue(NEW_COUNT_FIELD, num);
}

void SummaryDisplay::setSuspendedCount(int num) {
    setFieldValue(SUSPENDED_COUNT_FIELD, num);
}

void SummaryDisplay::setGlobalTime(int gtime) {
    setFieldValue(GLOBAL_TIME_FIELD, gtime);
}

void SummaryDisplay::setNewProcess(Process *process) {
    if (process) {
        setFieldValue(NEW_PROCESS_PID_FIELD, process->pid);
        setFieldValue(NEW_PROCESS_SIZE_FIELD, process->size);
    } else {
        clearField(NEW_PROCESS_PID_FIELD);
        clearField(NEW_PROCESS_SIZE_FIELD);
    }
}

void SummaryDisplay::setSuspendedProcess(Process *process) {
    if (process) {
        setFieldValue(SUSPENDED_PROCESS_PID_FIELD, process->pid);
        setFieldValue(SUSPENDED_PROCESS_SIZE_FIELD, process->size);
    } else {
        clearField(SUSPENDED_PROCESS_PID_FIELD);
        clearField(SUSPENDED_PROCESS_SIZE_FIELD);
    }
}

void SummaryDisplay::post() {
    post_form(form);
}

void SummaryDisplay::unpost() {
    unpost_form(form);
}

void SummaryDisplay::setFieldValue(int index, int value) {
    char *buffer = field_buffer(fields[index], 0);

    sprintf(buffer, "%d", value);

    set_field_buffer(fields[index], 0, buffer);
}

void SummaryDisplay::clearField(int index) {
    set_field_buffer(fields[index], 0, "");
}

void SummaryDisplay::initFields() {
    int row, col, label_size;

    row = 0;
    col = 0;

    for (int i = 0; i < NUM_OF_FIELDS; i++) {
        label_size = labels[i].size();

        fields[i + NUM_OF_FIELDS] = newLabel(row, col, labels[i]);
        fields[i] = newField(row, col + label_size);

        col += FIELD_WIDTH + label_size;
    }

    fields.back() = nullptr;
}

void SummaryDisplay::initForm() {
    form = new_form(fields.data());
    set_form_sub(form, window);
}

FIELD *SummaryDisplay::newField(int row, int col) {
    FIELD *field;

    field = new_field(1, FIELD_WIDTH, row, col, 0, 0);

    set_field_fore(field, A_BOLD);

    return field;
}

FIELD *SummaryDisplay::newLabel(int row, int col, const string &text) {
    FIELD *label;

    label = new_field(1, text.size(), row, col, 0, 0);

    set_field_buffer(label, 0, text.c_str());

    return label;
}
