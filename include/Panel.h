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

private:
    WINDOW *win;

protected:
    WINDOW *subwin;
    std::string title;
};

#endif
