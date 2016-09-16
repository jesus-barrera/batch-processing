#include <climits>
#include <cstdlib>
#include <ncurses.h>

#include "../include/ProcessForm.h"
#include "../include/Process.h"
#include "../include/util.h"

const std::string ProcessForm::labels[NUM_OF_FIELDS] = {
    "No. Programa: ",
    "Nombre: ",
    "Operacion (+, -, *, /, %%, ^, %%%%): ",
    "Operando Izq.: ",
    "Operando Der.: ",
    "Tiempo estimado: ",
};

const std::string ProcessForm::descriptions[NUM_OF_FIELDS] = {
    "Entero mayor a cero, no debe repetirse",
    "Solo se permiten letras",
    "+:suma, -:resta, *:multiplicacion, %%:modulo, ^:potencia, %%%%:porcentaje",
    "Cualquier numero entero",
    "Cualquier numero entero, diferente de 0 cuando la operacion es %% o /",
    "Numero entero positivo mayor a 0"
};

const int ProcessForm::FIELDS_Y_OFFSET = 2;

ProcessForm::ProcessForm(WINDOW *win) {
    window = win;

    // set up fields
    for (int i = 0, row = 0; i < NUM_OF_FIELDS; i++, row += FIELDS_Y_OFFSET) {
        fields[i] = new_field(1, 10, row, labels[i].length(), 0, 0);

        set_field_back(fields[i], A_UNDERLINE);
        field_opts_off(fields[i], O_AUTOSKIP | O_NULLOK);
    }

    fields[NUM_OF_FIELDS] = NULL;

    set_field_type(
        fields[PROGRAM_NUMBER_FIELD],
        unique_num_type,
        &program_numbers
    );

    set_field_type(fields[PROGRAMMER_NAME_FIELD], TYPE_ALPHA, 1);
    set_field_type(fields[OPERATION_FIELD], TYPE_ENUM, Process::operators, 0, 0);
    set_field_type(fields[LEFT_OPERAND_FIELD], TYPE_INTEGER, 1, 0, 0);
    set_field_type(fields[RIGHT_OPERAND_FIELD], TYPE_INTEGER, 1, 0, 0);
    set_field_type(fields[ESTIMATED_TIME_FIELD], TYPE_INTEGER, 1, 1, UINT_MAX);

    // set up form
    form = new_form(fields);

    set_field_init(form, &fieldInitHook);
    set_form_sub(form, window);
}

ProcessForm::~ProcessForm() {
    free_form(form);

    for (int i = 0; i < NUM_OF_FIELDS; i++) {
        free_field(fields[i]);
        fields[i] = NULL;
    }
}

/**
 * Post the form in the window.
 */
void ProcessForm::post() {
    post_form(form);
    printLabels();
}

/**
 * Unpost the form from the window.
 */
void ProcessForm::unpost() {
    unpost_form(form);
}

/**
 * Handle user input
 */
void ProcessForm::handleKey(int key) {
    switch (key) {
        case KEY_DOWN:
            prevField();
            break;

        case KEY_UP:
            nextField();
            break;

        case KEY_BACKSPACE:
            removeChar();
            break;

        default:
           form_driver(form, key);
           break;
    }
}

/**
 * Restores the cursor position.
 */
void ProcessForm::setCursor() {
    pos_form_cursor(form);
}

/*
 * Set focus to the given field.
 */
bool ProcessForm::goToField(int field_index) {
    return field_index >= 0 && field_index < NUM_OF_FIELDS &&
           set_current_field(form, fields[field_index]) == E_OK;
}

/**
 * Clears the form fields.
 */
void ProcessForm::clear() {
    for (int i = 0; i < NUM_OF_FIELDS; i++) {
        set_field_buffer(fields[i], 0, "");
        set_field_status(fields[i], FALSE);
    }
}

/**
 * Validates each field in the form. Use form_driver to request field validation
 * and field_status to check the field is filled.
 */
bool ProcessForm::validate() {
    bool is_valid = true;

    for (int i = 0; i < NUM_OF_FIELDS && is_valid; i++) {
        set_current_field(form, fields[i]);

        if (form_driver(form, REQ_VALIDATION) == E_INVALID_FIELD || !field_status(fields[i])) {
            is_valid = false;
        }
    }

    if (is_valid && isZeroDivision()) {
        set_current_field(form, fields[RIGHT_OPERAND_FIELD]);
        is_valid = false;
    }

    return is_valid;
}

/**
 * Display a message in this form.
 */
void ProcessForm::displayMessage(string message) {
    displayMessage(form, message);
}

/**
 * The following get methods convert the contents of the corresponding field
 * buffer to a proper data value.
 */

int ProcessForm::getProgramNumber() {
    return atoi(field_buffer(fields[PROGRAM_NUMBER_FIELD], 0));
}

string ProcessForm::getProgrammerName() {
    string name(field_buffer(fields[PROGRAMMER_NAME_FIELD], 0));
    trim(name);

    return name;
}

short ProcessForm::getOperation() {
    string operand(field_buffer(fields[OPERATION_FIELD], 0));
    trim(operand);

    return Process::getOperation(operand.c_str());
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

/**
 * Display a message in at the end of form.
 */
void ProcessForm::displayMessage(FORM *form, string message) {
    WINDOW *form_win;

    form_win = form_sub(form);

    // clear message line
    wmove(form_win, NUM_OF_FIELDS * FIELDS_Y_OFFSET + 1, 0);
    wclrtoeol(form_win);

    // print message
    wprintw(form_win, message.c_str());
}

/**
 * Called when a field changes. Displays the current field description.
 */
void ProcessForm::fieldInitHook(FORM *form) {
    int index;

    index = field_index(current_field(form));

    displayMessage(form, descriptions[index]);
}

/**
 * Used to handle up arrow key press, chage the focus to the next field.
 */
void ProcessForm::nextField() {
    form_driver(form, REQ_PREV_FIELD);
    form_driver(form, REQ_END_LINE);
}

/**
 *  Used to handle the down arrow key press, change focus to the previous field.
 */
void ProcessForm::prevField() {
    form_driver(form, REQ_NEXT_FIELD);
    form_driver(form, REQ_END_LINE);
}

/**
 *  Used to handle backspace key press, removes the char behind the cursor.
 */
void ProcessForm::removeChar() {
    form_driver(form, REQ_PREV_CHAR);
    form_driver(form, REQ_DEL_CHAR);
}

/**
 * Prints the field labels in the form window.
 */
void ProcessForm::printLabels() {
    wattron(window, A_BOLD);

    for (int i = 0, row = 0; i < NUM_OF_FIELDS; i++, row += FIELDS_Y_OFFSET) {
        mvwprintw(window, row, 0, labels[i].c_str());
    }

    wattroff(window, A_BOLD);
}

/**
 * Checks if the field "operation" is whether / or % and right_operand is 0
 * causing a division by zero.
 */
bool ProcessForm::isZeroDivision() {
    short operation;
    int right_operand;

    operation = getOperation();
    right_operand = getRightOperand();

    return (operation == Process::DIV_OP || operation == Process::MOD_OP) && right_operand == 0;
}
