#ifndef _PROCESS_INCLUDED_
#define _PROCESS_INCLUDED_

#include <string>

using namespace std;

class Process {
public:
    enum {
        ADD_OP,
        SUB_OP,
        MUL_OP,
        DIV_OP,
        MOD_OP,
        POW_OP,
        PCT_OP,
        NUM_OF_OPS
    };

    static const char *operators[];

    string programmer_name;
    short operation;
    int left_operand;
    int right_operand;
    int result;
    unsigned int estimated_time;
    unsigned int elapsed_time;
    unsigned int program_number;

    static short getOperation(const char* operator_str);
    int run();
};

#endif
