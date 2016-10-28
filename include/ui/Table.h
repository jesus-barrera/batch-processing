#ifndef _TABLE_INCLUDED_
#define _TABLE_INCLUDED_

#include <string>
#include <deque>

#include <ncurses.h>

using namespace std;

template <typename T>
class Table {
public:
    Table(WINDOW *window);
    virtual ~Table();

    void post();
    void setData(deque<T> &data);

protected:
    WINDOW *data_win;

    void setHeading(string str);
    virtual void printRow(T item) = 0;

private:
    WINDOW *window;
    chtype *heading;
};

template <typename T>
Table<T>::Table(WINDOW *window) {
    int nlines, ncols;

    this->window = window;

    getmaxyx(window, nlines, ncols);

    // create inner window where actual content is written. Leave the first row
    // for the heading.
    data_win = derwin(window, nlines - 1, ncols, 1, 0);

    scrollok(data_win, TRUE);
    syncok(data_win, TRUE);

    heading = nullptr;
}

template <typename T>
Table<T>::~Table() {
    delete[] heading;
}

template <typename T>
void Table<T>::post() {
    wclear(window);

    mvwaddchstr(window, 0, 0, heading);
}

template <typename T>
void Table<T>::setData(deque<T> &data) {
    size_t count;

    // clear previous data
    wclear(data_win);
    wmove(data_win, 0, 0);

    count = 0;

    while (count < data.size()) {
        printRow(data.at(count++));

        if (count != data.size()) {
            // use a line feed instead of moving the cursor one row down. This
            // causes the screen to scroll up when reaching the last row
            waddch(data_win, '\n');
        }
    }
}

template <typename T>
void Table<T>::setHeading(string str) {
    int max_x;
    size_t i;

    // fill heading with spaces.
    max_x = getmaxx(window);
    str.append(max_x - str.size(), ' ');

    if (heading) delete[] heading;

    heading = new chtype[str.size() + 1];

    for (i = 0; i < str.size(); i ++) {
        heading[i] = str[i] | A_REVERSE;
    }

    heading[i] = 0; // null terminated
}

#endif
