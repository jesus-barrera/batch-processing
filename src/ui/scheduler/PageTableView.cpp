#include <sstream>
#include "ui/scheduler/PageTableView.h"

PageTableView::PageTableView(WINDOW *window) {
    this->window = window;
}

void PageTableView::setData(MemoryManager &memory) {
    stringstream label;
    wmove(window, 0, 0);

    // print OS table
    printRow("SO: ", *memory.os_pages);

    // print process tables
    for (auto &it: memory.tables) {
        label << "P" << it.first << ": ";

        printRow(label.str(), *it.second);

        label.str("");
    }

    // print free frames
    printRow("Libres: ", memory.free_frames);
}

void PageTableView::printRow(
        string label,
        MemoryManager::PageTable &pages,
        bool new_line) {

    printLabel(label);
    printPages(pages);

    if (new_line) waddch(window, '\n');
}

void PageTableView::printPages(MemoryManager::PageTable &pages) {
    int i, j;

    i = 0;
    j = pages.size() - 1;

    while (i < j) {
        wprintw(window, "%d, ", pages[i]);
        i++;
    }

    if (j != -1) wprintw(window, "%d", pages[i]);
}

void PageTableView::printLabel(string &label) {
    wattron(window, A_BOLD);
    wprintw(window, label.c_str());
    wattroff(window, A_BOLD);
}
