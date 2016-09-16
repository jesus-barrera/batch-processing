#include "../include/Process.h"

int Process::run() {
    switch (operation) {
        case '+':
            result = left_operand + right_operand;
            break;
        case '-':
            result = left_operand - right_operand;
            break;
        case '/':
            result = left_operand / right_operand;
        case '*':
            result = left_operand * right_operand;
            break;
        case '%':
            result = left_operand % right_operand;
            break;
        default:
            result = 0;
    }

    return 0;
}
