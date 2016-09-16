#ifndef _UTIL_INCLUDED_
#define _UTIL_INCLUDED_

#include <ncurses.h>
#include <string>

void printCentered(WINDOW *win, std::string text, int row, int attrs);
void clearLine(WINDOW *win, int line = -1, bool has_border = false);

#endif
