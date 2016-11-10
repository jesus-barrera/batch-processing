#include "ui/scheduler/SummaryDisplay.h"
#include "ui/screen.h"

const std::string SummaryDisplay::labels[] = {
    "Quantum: ",
    "Procesos nuevos: ",
    "Tiempo Global: "
};

SummaryDisplay::SummaryDisplay() {
    int startx;

    subwin = derwin(content, 2, COLS, 0, 0);

    startx = 0;

    for (int i = 0; i < NUM_OF_FIELDS; i++, startx += 5) {
        startx += labels[i].size();

        fields[i] = new_field(1, 5, 0, startx, 0, 0);

        set_field_back(fields[i], A_BOLD);
    }

    fields[NUM_OF_FIELDS] = NULL;

    form = new_form(fields);
    set_form_sub(form, subwin);
}

SummaryDisplay::~SummaryDisplay() {
    free_form(form);

    for (int i = 0; i < NUM_OF_FIELDS; i++) {
        free_field(fields[i]);

        fields[i] = NULL;
    }

    delwin(subwin);
}

void SummaryDisplay::post() {
    post_form(form);
    postLabels();
}

void SummaryDisplay::unpost() {
    unpost_form(form);
}

void SummaryDisplay::setQuantum(int quantum) {
    setFieldValue(QUANTUM_FIELD, quantum);
}

void SummaryDisplay::setNewProcesses(int num) {
    setFieldValue(NEW_PROCESSES_FIELD, num);
}

void SummaryDisplay::setGlobalTime(int gtime) {
    setFieldValue(GLOBAL_TIME_FIELD, gtime);
}

void SummaryDisplay::setFieldValue(int index, int value) {
    char *buffer = field_buffer(fields[index], 0);

    sprintf(buffer, "%d", value);

    set_field_buffer(fields[index], 0, buffer);
}

void SummaryDisplay::postLabels() {
    for (int i = 0, startx = 0; i < NUM_OF_FIELDS; i++) {
        mvwprintw(subwin, 0, startx, labels[i].c_str());

        startx += labels[i].size() + 5;
    }
}
