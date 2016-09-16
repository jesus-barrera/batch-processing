#include <algorithm>
#include "../include/field_types.h"

FIELDTYPE *unique_num_type;

/**
 * unique_num_type definition
 */
bool uniqueNumTypeFieldValidate(FIELD *field, const void *pile);
bool uniqueNumTypeCharValidate(int c, const void *pile);
void *uniqueNumTypeMakeStr(va_list *arg);
void *uniqueNumTypeCopyStr(const void *pile);
void uniqueNumTypeFreeStr(void *pile);

/**
 * Allocate field types
 */
void initFieldTypes() {
    unique_num_type = new_fieldtype(&uniqueNumTypeFieldValidate, &uniqueNumTypeCharValidate);

    set_fieldtype_arg(
        unique_num_type,
        &uniqueNumTypeMakeStr,
        &uniqueNumTypeCopyStr,
        &uniqueNumTypeFreeStr
    );
}

/**
 * Deallocate field types
 */
void endFieldTypes() {
    free_fieldtype(unique_num_type);
}

/**
 * unique_num_type field validation. Returns tru if the value is not already in
 * the values list.
 */
bool uniqueNumTypeFieldValidate(FIELD *field, const void *pile) {
    const values_list *values;
    int value;

    values = static_cast<const values_list *>(pile);
    value = atoi(field_buffer(field, 0));

    return find(values->begin(), values->end(), value) == values->end();
}

/**
 * unique_num_type char validation. Returns true if the char typed is a digit.
 */
bool uniqueNumTypeCharValidate(int c, const void *pile) {
    return c >= '0' && c <= '9';
}

/**
 * unique_num_type make storage.
 */
void *uniqueNumTypeMakeStr(va_list *args) {
    return va_arg(args, void *);
}

void *uniqueNumTypeCopyStr(const void *pile) {
    // const values_list *original;
    // values_list *copy;
    //
    // original = static_cast<const values_list *>(pile);
    //
    // copy = new values_list(*original);
    //
    // return static_cast<void *>(copy);

    return (void *)pile;
}

void uniqueNumTypeFreeStr(void *pile) {
    // delete(static_cast<values_list *>(pile));
}
