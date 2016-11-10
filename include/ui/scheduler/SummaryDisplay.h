#ifndef _SUMMARY_DISPLAY_INCLUDED_
#define _SUMMARY_DISPLAY_INCLUDED_

#include <ncurses.h>
#include <form.h>
#include <string>

class SummaryDisplay {
public:
    SummaryDisplay();
    ~SummaryDisplay();

    void post();
    void unpost();

    void setQuantum(int quantum);
    void setNewProcesses(int num);
    void setGlobalTime(int gtime);

private:
    enum {
        QUANTUM_FIELD,
        NEW_PROCESSES_FIELD,
        GLOBAL_TIME_FIELD,
        NUM_OF_FIELDS
    };

    static const std::string labels[];

    WINDOW *subwin;
    FORM *form;
    FIELD *fields[NUM_OF_FIELDS + 1]; // NULL terminated array

    void setFieldValue(int index, int value);
    void postLabels();
};

#endif
