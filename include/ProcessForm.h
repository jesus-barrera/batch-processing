#ifndef _PROCESS_FORM_INCLUDED_
#define _PROCESS_FORM_INCLUDED_

#include <string>
#include <form.h>

using namespace std;

class ProcessForm {
public:
    enum {
        PROGRAM_NUMBER_FIELD,
        NAME_FIELD,
        OPERATION_FIELD,
        LEFT_OPERAND_FIELD,
        RIGHT_OPERAND_FIELD,
        ESTIMATED_TIME_FIELD,
        NUM_OF_FIELDS
    };

    static const int FIELDS_Y_OFFSET;

    static const string labels[];
    static const char *valid_operations[];

    ProcessForm(WINDOW *window = NULL);
    ~ProcessForm();

    void post();
    void unpost();
    void handleKey(int key);
    void setCursor();
    void clear();
    bool validate();

    int getProgramNumber();
    string getName();
    char getOperation();
    int getLeftOperand();
    int getRightOperand();
    int getEstimatedTime();

private:
    WINDOW *window;
    FORM *form;
    FIELD *fields[NUM_OF_FIELDS + 1];

    void printLabels();
    bool isZeroDivision();
};

#endif
