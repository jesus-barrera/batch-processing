#include <limits>
#include <cstdlib>

#include <ncurses.h>

#include "../include/ProcessForm.h"

const std::string ProcessForm::labels[] = {
    "No. Programa: ",
    "Nombre: ",
    "Operacion (+, -, *, /, %, ^, %%%%): ",
    "Operando Izq.: ",
    "Operando Der.: ",
    "Tiempo estimado: ",
};

const char *ProcessForm::valid_operations[] = {"+", "-", "*", "/", "%", "^", "%%", NULL};

const int ProcessForm::FIELDS_Y_OFFSET = 2;

ProcessForm::ProcessForm(WINDOW *win) {
    window = win;
    int x, y;

    x = 0;
    y = 0;

    // create fields
    for (int i = 0; i < NUM_OF_FIELDS; i++, y += FIELDS_Y_OFFSET) {
        fields[i] = new_field(1, 10, y, x + labels[i].length(), 0, 0);
        set_field_back(fields[i], A_UNDERLINE);
        field_opts_off(fields[i], O_AUTOSKIP | O_NULLOK);
    }

    fields[NUM_OF_FIELDS] = NULL;

    // set field types
    set_field_type(fields[PROGRAM_NUMBER_FIELD], TYPE_INTEGER, 1, 0, std::numeric_limits<int>::max());
    set_field_type(fields[NAME_FIELD], TYPE_ALNUM, 1);
    set_field_type(fields[OPERATION_FIELD], TYPE_ENUM, valid_operations, 0, 0);
    set_field_type(fields[LEFT_OPERAND_FIELD], TYPE_INTEGER, 1, 0, 0);
    set_field_type(fields[RIGHT_OPERAND_FIELD], TYPE_INTEGER, 1, 0, 0);
    set_field_type(fields[ESTIMATED_TIME_FIELD], TYPE_INTEGER, 1, 1, std::numeric_limits<int>::max());

    form = new_form(fields);

    set_form_sub(form, window);
}

ProcessForm::~ProcessForm() {
    free_form(form);

    for (int i = 0; i < NUM_OF_FIELDS; i++) {
        free_field(fields[i]);
        fields[i] = NULL;
    }
}

void ProcessForm::post() {
    post_form(form);
    printLabels();
}

void ProcessForm::unpost() {
    unpost_form(form);
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

void ProcessForm::setCursor() {
    pos_form_cursor(form);
}

void ProcessForm::clear() {
    for (int i = 0; i < NUM_OF_FIELDS; i++) {
        set_current_field(form, fields[i]);
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

    y = 0;
    x = 0;

    wattron(window, A_BOLD);

    for (int i = 0; i < NUM_OF_FIELDS; i++, y += FIELDS_Y_OFFSET) {
        mvwprintw(window, y, x, labels[i].c_str());
    }

    wattroff(window, A_BOLD);
}

bool ProcessForm::isZeroDivision() {
    char *operation = field_buffer(fields[OPERATION_FIELD], 0);
    int right_operand = atoi(field_buffer(fields[RIGHT_OPERAND_FIELD], 0));

    set_current_field(form, fields[RIGHT_OPERAND_FIELD]);

    return (operation[0] == '%' || operation[0] == '/') && right_operand == 0;
}
