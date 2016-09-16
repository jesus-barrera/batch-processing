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

    ProcessForm(int start_y = 0, int start_x = 0);
    ~ProcessForm();

    void post();
    void handleKey(int key);
    void goToFirstField();
    void clear();
    bool validate();

    int getProgramNumber();
    string getName();
    char getOperation();
    int getLeftOperand();
    int getRightOperand();
    int getEstimatedTime();

private:
    FORM *form;
    FIELD *fields[NUM_OF_FIELDS + 1];

    int start_x, start_y;

    void printLabels();
    bool isZeroDivision();
};
