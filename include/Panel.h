#ifndef _PANEL_INCLUDED_
#define _PANEL_INCLUDED_

#include <string>

#include <ncurses.h>

class Panel {
public:
    static const int BORDER_WIDTH;

    Panel(WINDOW *parent, std::string title,  int nlines, int ncols, int begin_x, int begin_y);
    ~Panel();

    void post();
    void unpost();

private:
    WINDOW *outer_win;

protected:
    WINDOW *inner_win;
    std::string title;
};

#endif
