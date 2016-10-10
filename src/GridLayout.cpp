#include "../include/GridLayout.h"

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
    Element *element = new Element();

    // set real dimenstions
    element->height = nrows * row_height;
    element->width = ncols * col_width;

    // set real position
    element->y = start_row * row_height;
    element->x = start_col * col_width;

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
