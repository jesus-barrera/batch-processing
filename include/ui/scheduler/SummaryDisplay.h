#ifndef _SUMMARY_DISPLAY_INCLUDED_
#define _SUMMARY_DISPLAY_INCLUDED_

#include <string>
#include <array>

#include <ncurses.h>
#include <form.h>

class Process;

using namespace std;

class SummaryDisplay {
public:
    SummaryDisplay(WINDOW *window = stdscr);
    ~SummaryDisplay();

    void setQuantum(int quantum);
    void setNewProcesses(int num);
    void setGlobalTime(int gtime);
    void setNextProcess(Process *process);

    void post();

private:
    enum {
        QUANTUM_FIELD,
        NEW_PROCESSES_FIELD,
        GLOBAL_TIME_FIELD,
        NEXT_PROCESS_PID_FIELD,
        NEXT_PROCESS_SIZE_FIELD,
        NUM_OF_FIELDS
    };

    static const int FIELD_WIDTH = 5;

    static const string labels[NUM_OF_FIELDS];

    WINDOW *window;
    FORM *form;

    // we place labels in form fields, so an extra field for each actual field is
    // required, also the array must be null terminated, thus the + 1.
    array<FIELD *, NUM_OF_FIELDS * 2 + 1> fields;

    void setFieldValue(int index, int value);
    void clearField(int index);
    void initFields();
    void initForm();
    FIELD *newField(int row, int col);
    FIELD *newLabel(int row, int col, const string &text);
};

#endif
