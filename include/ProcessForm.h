#ifndef _PROCESS_FORM_INCLUDED_
#define _PROCESS_FORM_INCLUDED_

#include <string>
#include "./field_types.h"

using namespace std;

class ProcessForm {
public:
    enum {
        PROGRAM_NUMBER_FIELD,
        PROGRAMMER_NAME_FIELD,
        OPERATION_FIELD,
        LEFT_OPERAND_FIELD,
        RIGHT_OPERAND_FIELD,
        ESTIMATED_TIME_FIELD,
        NUM_OF_FIELDS
    };

    static const int FIELDS_Y_OFFSET;

    static const string labels[];
    static const string descriptions[];

    ProcessForm(WINDOW *window = NULL);
    ~ProcessForm();

    void post();
    void unpost();
    void handleKey(int key);
    void setCursor();
    bool goToField(int field_index);
    void clear();
    bool validate();
    void displayMessage(string message);

    int getProgramNumber();
    string getProgrammerName();
    short getOperation();
    int getLeftOperand();
    int getRightOperand();
    int getEstimatedTime();

    values_list program_numbers;
    
private:
    WINDOW *window;
    FORM *form;
    FIELD *fields[NUM_OF_FIELDS + 1];

    static void displayMessage(FORM *form, string message);
    static void fieldInitHook(FORM *form);

    void nextField();
    void prevField();
    void removeChar();

    void printLabels();
    bool isZeroDivision();
};

#endif
