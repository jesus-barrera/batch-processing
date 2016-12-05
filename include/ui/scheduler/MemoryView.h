#ifndef _MEMORY_VIEW_INCLUDED_
#define _MEMORY_VIEW_INCLUDED_

#include <map>
#include <ncurses.h>
#include <form.h>

#include "MemoryManager.h"
#include "ProcessList.h"

using namespace std;

class MemoryView {
public:
    enum {
        OS_COLOR_PAIR=1,
        READY_COLOR_PAIR,
        BLOCKED_COLOR_PAIR,
        RUNNING_COLOR_PAIR};

    // use 2 fields per frame, one for the frame number and one for the used blocks
    static const int TOTAL_FIELDS = MemoryManager::TOTAL_FRAMES * 2;
    static const int BLOCK_SIZE = 2;
    static const int BLOCK_SPACING = 1;
    static const int BLOCK_CHAR = 97;
    static const int FRAME_WIDTH = (MemoryManager::FRAME_SIZE
                                    * (BLOCK_SIZE + BLOCK_SPACING));
    static const int FRAME_NUM_WIDTH = 3;
    static const int FRAME_SPACING = 2;
    static const int COLUMN_WIDTH = FRAME_NUM_WIDTH + FRAME_WIDTH;

    static void init();

    MemoryView(WINDOW *window = stdscr);
    ~MemoryView();

    void setData(MemoryManager &memory, ProcessList &pcb);

    void post();
    void unpost();

private:
    static map<int, int> color_pairs;
    int max_y;
    int max_x;

    WINDOW *window;
    array<FIELD *, TOTAL_FIELDS + 1> fields;  // null terminated array, thus the + 1
    FORM *form;

    void setWindow(WINDOW *window);
    void initFields();
    void initForm();
    FIELD *newFrameField(int row, int col);
    FIELD *newFrameNumField(int row, int col, int frame_num);
    void clearFrames();
    void printPages(MemoryManager::PageTable *pages, int size, int color_pair);
    void printPage(int frame_number, int used_blocks, int color_pair);
};

#endif
