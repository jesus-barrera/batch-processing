#include "ui/GridLayout.h"

GridLayout::GridLayout(WINDOW *window, int nrows, int ncols) {
    setProperties(window, nrows, ncols);
}

GridLayout::~GridLayout() {
    clear();
}

void GridLayout::setProperties(WINDOW *window, int nrows, int ncols) {
    int y, x;

    // get window size
    getmaxyx(window, y, x);

    // set layout properties
    row_height = (double)y / nrows;
    col_width  = (double)x / ncols;
}

void GridLayout::add(int nrows, int ncols, int start_row, int start_col) {
    Element *element;
    int max_y, max_x;

    element = new Element();

    // set position
    element->y = start_row * row_height;
    element->x = start_col * col_width;

    max_y = (start_row + nrows) * row_height;
    max_x = (start_col + ncols) * col_width;

    // set dimenstions
    element->height = max_y - element->y;
    element->width = max_x - element->x;

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
