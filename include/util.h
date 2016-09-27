#ifndef _UTIL_INCLUDED_
#define _UTIL_INCLUDED_

#include <ncurses.h>
#include <string>

void printCentered(WINDOW *win, std::string text, int row, int attrs);

std::string& trim(std::string &str);

int random(unsigned int min, unsigned int max);

#endif
