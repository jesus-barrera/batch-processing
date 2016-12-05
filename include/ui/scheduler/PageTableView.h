#ifndef _PAGE_TABLE_INCLUDED_
#define _PAGE_TABLE_INCLUDED_

#include <string>
#include <ncurses.h>
#include "MemoryManager.h"

using namespace std;

class PageTableView {
public:
    PageTableView(WINDOW *window = stdscr);

    void setData(MemoryManager &memory);

private:
    WINDOW *window;

    void printRow(
            string label,
            MemoryManager::PageTable &pages,
            bool new_line = true);

    void printPages(MemoryManager::PageTable &pages);
    void printLabel(string &label);
};

#endif
