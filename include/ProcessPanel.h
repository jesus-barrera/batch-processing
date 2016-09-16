#ifndef _PROCESS_PANEL_INCLUDED_
#define _PROCESS_PANEL_INCLUDED_

#include "./Panel.h"
#include "./Process.h"

class ProcessPanel: public Panel {
public:
    enum {
        PROGRAM_NUMBER_FIELD,
        NAME_FIELD,
        OPERATION_FIELD,
        ESTIMATED_TIME_FIELD,
        ELAPSED_TIME_FIELD,
        NUM_OF_FIELDS
    };

    static const std::string labels[NUM_OF_FIELDS];

    ProcessPanel(WINDOW *parent, int nlines, int ncols, int begin_x, int begin_y);

    void post();
    void display(Process *process);
    void setProgramNumber(unsigned int program_number);
    void setName(std::string &name);
    void setOperation(char operation, int left_operand, int right_operand);
    void setEstimatedTime(unsigned int estimated_time);
    void setElapsedTime(unsigned int elapsed_time);

private:
    void clearField(int index);
    void printLabels();
};

#endif
