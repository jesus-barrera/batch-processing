#ifndef _PROCESS_INCLUDED_
#define _PROCESS_INCLUDED_

#include <string>

using namespace std;

class Process {
public:
    string name;
    char operation;
    int left_operand;
    int right_operand;
    int result;
    unsigned int estimated_time;
    unsigned int elapsed_time;
    unsigned int program_number;

    int run();
};

#endif
