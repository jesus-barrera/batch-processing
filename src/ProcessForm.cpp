#include <limits>
#include <ncurses.h>
#include <cstdlib>
#include "../include/ProcessForm.h"

const std::string ProcessForm::labels[] = {
    "No. Programa: ",
    "Nombre: ",
    "Operacion (+, -, *, /, %): ",
    "Operando Izq.: ",
    "Operando Der.: ",
    "Tiempo estimado: ",
};

const char *ProcessForm::valid_operations[] = {"+", "-", "*", "/", "%", NULL};

const int ProcessForm::FIELDS_Y_OFFSET = 2;

ProcessForm::ProcessForm(int start_y, int start_x) {
    this->start_y = start_y;
    this->start_x = start_x;

    for (int i = 0; i < NUM_OF_FIELDS; i++, start_y += FIELDS_Y_OFFSET) {
        fields[i] = new_field(1, 10, start_y, start_x + labels[i].length(), 0, 0);
        set_field_back(fields[i], A_UNDERLINE);
        field_opts_off(fields[i], O_AUTOSKIP | O_NULLOK);
    }

    set_field_type(fields[PROGRAM_NUMBER_FIELD], TYPE_INTEGER, 1, 0, std::numeric_limits<int>::max());
    set_field_type(fields[NAME_FIELD], TYPE_ALNUM, 1);
    set_field_type(fields[OPERATION_FIELD], TYPE_ENUM, valid_operations, 0, 0);
    set_field_type(fields[LEFT_OPERAND_FIELD], TYPE_INTEGER, 1, 0, 0);
    set_field_type(fields[RIGHT_OPERAND_FIELD], TYPE_INTEGER, 1, 0, 0);
    set_field_type(fields[ESTIMATED_TIME_FIELD], TYPE_INTEGER, 1, 1, std::numeric_limits<int>::max());

    fields[NUM_OF_FIELDS] = NULL;

    form = new_form(fields);

    post_form(form);
    printLabels();
}

ProcessForm::~ProcessForm() {
    unpost_form(form);
    free_form(form);

    for (int i = 0; i < NUM_OF_FIELDS; i++) {
        free_field(fields[i]);
        fields[i] = NULL;
    }
}

void ProcessForm::handleKey(int key) {
    switch (key) {
        case KEY_DOWN:
           form_driver(form, REQ_NEXT_FIELD);
           form_driver(form, REQ_END_LINE);
           break;
        case KEY_UP:
           form_driver(form, REQ_PREV_FIELD);
           form_driver(form, REQ_END_LINE);
           break;

        case KEY_BACKSPACE:
            form_driver(form, REQ_PREV_CHAR);
            form_driver(form, REQ_DEL_CHAR);
            break;

        default:
           form_driver(form, key);
           break;
    }
}

void ProcessForm::goToFirstField() {
    form_driver(form, REQ_FIRST_FIELD);
}

void ProcessForm::clear() {
    for (int i = 0; i < NUM_OF_FIELDS; i++) {
        set_current_field(form, fields[i]);
        set_field_status(fields[i], FALSE);
        form_driver(form, REQ_CLR_FIELD);
    }
}

bool ProcessForm::validate() {
    for (int i = 0; i < NUM_OF_FIELDS; i++) {
        set_current_field(form, fields[i]);

        if (form_driver(form, REQ_VALIDATION) == E_INVALID_FIELD || !field_status(fields[i])) {
            return false;
        }
    }

    return !isZeroDivision();
}

int ProcessForm::getProgramNumber() {
    return atoi(field_buffer(fields[PROGRAM_NUMBER_FIELD], 0));
}

string ProcessForm::getName() {
    return field_buffer(fields[NAME_FIELD], 0);
}

char ProcessForm::getOperation() {
    return field_buffer(fields[OPERATION_FIELD], 0)[0];
}

int ProcessForm::getLeftOperand() {
    return atoi(field_buffer(fields[LEFT_OPERAND_FIELD], 0));
}

int ProcessForm::getRightOperand() {
    return atoi(field_buffer(fields[RIGHT_OPERAND_FIELD], 0));
}

int ProcessForm::getEstimatedTime() {
    return atoi(field_buffer(fields[ESTIMATED_TIME_FIELD], 0));
}

void ProcessForm::printLabels() {
    int x, y;

    y = start_y;
    x = start_x;

    for (int i = 0; i < NUM_OF_FIELDS; i++, y += FIELDS_Y_OFFSET) {
        mvprintw(y, x, labels[i].c_str());
    }
}

bool ProcessForm::isZeroDivision() {
    char *operation = field_buffer(fields[OPERATION_FIELD], 0);
    int right_operand = atoi(field_buffer(fields[RIGHT_OPERAND_FIELD], 0));

    set_current_field(form, fields[RIGHT_OPERAND_FIELD]);

    return (operation[0] == '%' || operation[0] == '/') && right_operand == 0;
}
