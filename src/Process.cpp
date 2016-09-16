#include <cmath>
#include <cstring>

#include "../include/Process.h"

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
