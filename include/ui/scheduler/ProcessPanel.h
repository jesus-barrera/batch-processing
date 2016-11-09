#ifndef _PROCESS_PANEL_INCLUDED_
#define _PROCESS_PANEL_INCLUDED_

#include "ui/Panel.h"
#include "Process.h"

class ProcessPanel: public Panel {
public:
    enum {
        PROGRAM_NUMBER_FIELD,
        PROGRAMMER_NAME_FIELD,
        OPERATION_FIELD,
        ESTIMATED_TIME_FIELD,
        ELAPSED_TIME_FIELD,
        TIME_LEFT_FIELD,
        QUANTUM_FIELD,
        NUM_OF_FIELDS
    };

    static const std::string labels[NUM_OF_FIELDS];

    ProcessPanel(WINDOW *parent, int nlines, int ncols, int begin_x, int begin_y);

    void post();
    void display(Process *process);
    void clear();
    void clearField(int index);

    void setProgramNumber(unsigned int program_number);
    void setProgrammerName(std::string &name);
    void setOperation(short int operation, int left_operand, int right_operand);
    void setEstimatedTime(unsigned int estimated_time);
    void setElapsedTime(unsigned int elapsed_time);
    void setTimeLeft(unsigned int time_left);
    void setQuantum(unsigned int time_left);

private:
    void printLabels();
    void setField(int field_index, const char *format, ...);
};

#endif
