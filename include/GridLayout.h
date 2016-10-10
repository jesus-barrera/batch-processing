#ifndef _GRID_LAYOUT_INCLUDED_
#define _GRID_LAYOUT_INCLUDED_

#include <vector>
#include <ncurses.h>

class GridLayout {
public:
    /**
     * Represents a layout element. The height, width, x and y properties
     * correspond to the real dimentions of the element over the window.
     */
    class Element {
    public:
        int height;
        int width;
        int x;
        int y;
    };

    /**
     * Creates a new layout of nrows rows by ncols columns for window.
     */
    GridLayout(WINDOW *window, int nrows, int ncols);

    ~GridLayout();

    /**
     * Adds a new element to the layout.
     */
    void add(int nrows, int ncols, int start_row, int start_col);

    /**
     * Gets an element from the layout.
     */
    const Element &operator[](int n);

private:
    double row_height;
    double col_width;

    std::vector<Element*> elements;

    void clear();
};

#endif
