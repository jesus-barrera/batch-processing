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

    // termination reasons
    enum { SUCCESS, ERROR };

    // process status
    enum { NEW, READY, RUNNING, BLOCKED, TERMINATED };

    static const char *operators[];

    short termination_status;
    int state;
    unsigned int program_number;
    string programmer_name;
    short operation;
    int left_operand;
    int right_operand;
    int result;

    int blocked_time;

    // times
    int termination_time;
    int estimated_time;

    int service_time;
    int arrival_time;
    int turnaround_time;
    int waiting_time;
    int response_time;

    static short getOperation(const char* operator_str);
    static Process *newRandom();

    Process();
    unsigned int getTimeLeft();
    int run();
};

#endif
