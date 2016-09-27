#include <cmath>
#include <cstring>
#include <sstream>

#include "../include/Process.h"
#include "../include/util.h"

using namespace std;

const char *Process::operators[NUM_OF_OPS + 1] = {
    "+", "-", "*", "/", "%", "^", "%%", NULL
};

short Process::getOperation(const char *operator_str) {
    for (int op = 0; op < NUM_OF_OPS; op++) {
        if (strcmp(operators[op], operator_str) == 0) {
            return op;
        }
    }

    return -1;
}

Process *Process::newRandom() {
    static unsigned int count = 0;
    stringstream name;
    Process *process;
    int right_operand_minvalue;

    process = new Process();

    process->program_number = ++count;

    // generate programmer's name
    name << "Programa " << process->program_number;
    process->programmer_name = name.str();

    // set operation and operands
    process->operation     = random(1, NUM_OF_OPS) - 1;
    process->left_operand  = random(0, 100);
    right_operand_minvalue = (process->operation == DIV_OP ||
                              process->operation == MOD_OP);

    process->right_operand = random(right_operand_minvalue, 100);

    // set other data
    process->estimated_time = random(3, 15);
    process->elapsed_time = 0;
    process->state = Process::READY;

    return process;
}

unsigned int Process::getTimeLeft() {
    return estimated_time - elapsed_time;
}

int Process::run() {
    switch (operation) {
        case ADD_OP:
            result = left_operand + right_operand;
            break;

        case SUB_OP:
            result = left_operand - right_operand;
            break;

        case DIV_OP:
            result = left_operand / right_operand;
            break;

        case MOD_OP:
            result = left_operand % right_operand;
            break;

        case MUL_OP:
            result = left_operand * right_operand;
            break;

        case POW_OP:
            result = pow(left_operand, right_operand);
            break;

        case PCT_OP:
            result = left_operand * right_operand / 100.0;
            break;

        default:
            result = 0;
            break;
    }

    return result;
}
