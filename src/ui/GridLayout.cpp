#include "ui/GridLayout.h"

GridLayout::GridLayout(WINDOW *window, int nrows, int ncols) {
    int y, x;

    // get window size
    getmaxyx(window, y, x);

    // set layout properties
    row_height = (double)y / nrows;
    col_width  = (double)x / ncols;
}

GridLayout::~GridLayout() {
    clear();
}

void GridLayout::add(int nrows, int ncols, int start_row, int start_col) {
    Element *element;
    int begin_y, begin_x;
    int max_y, max_x;

    element = new Element();

    begin_y = start_row * row_height;
    begin_x = start_col * col_width;

    max_y = (start_row + nrows) * row_height;
    max_x = (start_col + ncols) * col_width;

    // set dimenstions
    element->height = max_y - begin_y;
    element->width = max_x - begin_x;

    // set position
    element->y = begin_y;
    element->x = begin_x;

    // save element
    elements.push_back(element);
}

const GridLayout::Element &GridLayout::operator[](int n) {
    return *elements[n];
}

void GridLayout::clear() {
    while (!elements.empty()) {
        delete(elements.back());
        elements.pop_back();
    }
}
