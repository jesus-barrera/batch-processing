#ifndef _FIELD_INCLUDED_
#define _FIELD_INCLUDED_

#include <ncurses.h>
#include <string>
#include <sstream>

template <class T>
class Field {
public:
    Field(WINDOW *parent_win, std::string label, int start_y, int start_x);

    void setValue(T value);
    void clearValue();

    void post();
    void unpost();

private:
    WINDOW *parent_win;
    std::string label;
    T value;

    int start_y;
    int start_x;

    void setCursor(int y_offset, int x_offset);
};

template <class T>
Field<T>::Field(WINDOW *parent_win, std::string label, int start_y, int start_x) {
    this->parent_win = parent_win;
    this->label = label;
    this->start_y = start_y;
    this->start_x = start_x;
}

template <class T>
void Field<T>::setValue(T value) {
    std::stringstream str_value;
    this->value = value;

    str_value << value;

    clearValue();
    waddstr(parent_win, str_value.str().c_str());
}

template <class T>
void Field<T>::clearValue() {
    setCursor(0, label.size());
    wclrtoeol(parent_win);
}

template <class T>
void Field<T>::post() {
    unpost();
    waddstr(parent_win, label.c_str());
}

template <class T>
void Field<T>::unpost() {
    setCursor(0, 0);
    wclrtoeol(parent_win);
}

template <class T>
void Field<T>::setCursor(int y_offset, int x_offset) {
    wmove(parent_win, start_y + y_offset, start_x + x_offset);
}

#endif
