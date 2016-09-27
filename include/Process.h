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

    enum {
        READY,
        RUNNING,
        TERMINATED,
        INTERRUPTED,
        ERROR
    };

    static const char *operators[];

    short state;
    unsigned int program_number;
    string programmer_name;
    short operation;
    int left_operand;
    int right_operand;
    int result;
    unsigned int estimated_time;
    unsigned int elapsed_time;

    static short getOperation(const char* operator_str);
    static Process *newRandom();

    unsigned int getTimeLeft();
    int run();
};

#endif
