#include "../include/Process.h"

void Process::setName(string& name) {
    this->name = name;
}

string Process::getName() {
    return this->name;
}

void Process::setOperation(char operation) {
    this->operation = operation;
}

char Process::getOperation() {
    return this->operation;
}

void Process::setLeftOperand(int leftOperand) {
    this->leftOperand  = leftOperand;
}

int Process::getLeftOperand() {
    return this->leftOperand;
}

void Process::setRightOperand(int rightOperand) {
    this->rightOperand = rightOperand;
}

int Process::getRightOperand() {
    return this->rightOperand;
}

int Process::run() {
    
}
