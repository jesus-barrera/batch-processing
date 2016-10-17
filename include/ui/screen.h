#ifndef _SCREEN_INCLUDED_
#define _SCREEN_INCLUDED_

#include <string>
#include <ncurses.h>

#define HEADER_LINES 3
#define FOOTER_LINES 1
#define CONTENT_LINES (LINES - (FOOTER_LINES + HEADER_LINES))

extern WINDOW *header;
extern WINDOW *content;
extern WINDOW *footer;

void startScreen();
void endScreen();
void setHeader(std::string title);
void setFooter(std::string message);

#endif
