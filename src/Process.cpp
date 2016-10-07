#include <cmath>
#include <cstring>
#include <sstream>

#include "../include/Process.h"
#include "../include/util.h"

using namespace std;

const char *Process::operators[NUM_OF_OPS + 1] = {
    "+", "-", "*", "/", "%", "^", "%%", NULL
};

/**
 * Gets the operation value for a given operator.
 */
short Process::getOperation(const char *operator_str) {
    for (int op = 0; op < NUM_OF_OPS; op++) {
        if (strcmp(operators[op], operator_str) == 0) {
            return op;
        }
    }

    return -1;
}

/**
 * Allocates a new Process object an fills it with random values.
 */
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
    process->operation = random(1, NUM_OF_OPS) - 1;

    if (process->operation == POW_OP) {
        process->left_operand = random(0, 9);
        process->right_operand = random(0, 9);
    } else {
        process->left_operand  = random(0, 100);
        right_operand_minvalue = (process->operation == DIV_OP ||
                                  process->operation == MOD_OP);
        process->right_operand = random(right_operand_minvalue, 100);
    }

    // set other data
    process->estimated_time = random(3, 15);
    process->service_time = 0;
    process->response_time = -1;

    return process;
}

unsigned int Process::getTimeLeft() {
    return estimated_time - service_time;
}

/**
 * Performs the operation described by the process' operation and operands. The
 * result is stored in the result attribute and returned.
 */
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
