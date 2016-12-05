#include <algorithm>
#include <sstream>
#include <iomanip>

#include "ui/scheduler/MemoryView.h"

map<int, int> MemoryView::color_pairs;

void MemoryView::init() {
    // set color pairs
    init_pair(OS_COLOR_PAIR, COLOR_WHITE, COLOR_BLACK);
    init_pair(READY_COLOR_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(BLOCKED_COLOR_PAIR, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(RUNNING_COLOR_PAIR, COLOR_RED, COLOR_BLACK);

    // map process states to a color pair
    color_pairs[Process::READY] = READY_COLOR_PAIR;
    color_pairs[Process::BLOCKED] = BLOCKED_COLOR_PAIR;
    color_pairs[Process::RUNNING] = RUNNING_COLOR_PAIR;
};

MemoryView::MemoryView(WINDOW *window) {
    setWindow(window);

    initFields();
    initForm();
}

MemoryView::~MemoryView() {
    free_form(form);

    for (FIELD *field : fields) {
        free_field(field);
    }
}

void MemoryView::setData(MemoryManager &memory, ProcessList &pcb) {
    Process *process;
    MemoryManager::PageTable *pages;
    int pair;

    clearFrames();

    // print OS pages
    printPages(memory.os_pages, memory.OS_SIZE, COLOR_PAIR(OS_COLOR_PAIR));

    // print processes pages
    for (auto &it : memory.tables) {
        process = pcb[it.first - 1];
        pages = it.second;
        pair = COLOR_PAIR(color_pairs[process->state]);

        printPages(pages, process->size, pair);
    }
}

void MemoryView::post() {
    post_form(form);
}

void MemoryView::unpost() {
    unpost_form(form);
}

void MemoryView::setWindow(WINDOW *window) {
    this->window = window;

    getmaxyx(window, max_y, max_x);
}

void MemoryView::initFields() {
    int col, row;

    row = 0;
    col = 0;

    for (int i = 0; i < MemoryManager::TOTAL_FRAMES; i++) {
        fields[MemoryManager::TOTAL_FRAMES + i] = newFrameNumField(row, col, i);
        fields[i] = newFrameField(row, col + FRAME_NUM_WIDTH);

        if ((row += FRAME_SPACING) >= max_y) {
            col += COLUMN_WIDTH; // move to next column
            row = 0;
        }
    }

    fields.back() = nullptr;
}

void MemoryView::initForm() {
    form = new_form(fields.data());
    set_form_sub(form, window);
}

FIELD *MemoryView::newFrameField(int row, int col) {
    FIELD *field;

    field = new_field(1, FRAME_WIDTH, row, col, 0, 0);
    field_opts_off(field, O_ACTIVE | O_EDIT);

    return field;
}

FIELD *MemoryView::newFrameNumField(int row, int col, int frame_num) {
    FIELD *field;
    stringstream num_str;

    // create field
    field = new_field(1, FRAME_NUM_WIDTH, row, col, 0, 0);
    field_opts_off(field, O_ACTIVE | O_EDIT);

    // set field content
    num_str << setw(FRAME_NUM_WIDTH) << left << frame_num;
    set_field_buffer(field, 0, num_str.str().c_str());

    return field;
}

void MemoryView::clearFrames() {
    for (int i = 0; i < MemoryManager::TOTAL_FRAMES; i++) {
        set_field_buffer(fields[i], 0, "");
    }
}

void MemoryView::printPages(
            MemoryManager::PageTable *pages,
            int size,
            int color_pair) {

    int i, j, remain;

    remain = size % MemoryManager::FRAME_SIZE;

    i = 0;
    j = pages->size() - (remain != 0);

    while (i < j) {
        printPage((*pages)[i], MemoryManager::FRAME_SIZE, color_pair);
        i++;
    }

    if (remain) {
        printPage((*pages)[i], remain, color_pair);
    }
}

void MemoryView::printPage(int frame_number, int used_blocks, int color_pair) {
    string block;
    string frame;
    FIELD *field;

    field = fields[frame_number];

    block.assign(BLOCK_SIZE, BLOCK_CHAR);
    block.append(BLOCK_SPACING, ' ');

    for (int i = 0; i < used_blocks; i++) {
        frame.append(block);
    }

    set_field_fore(field, A_ALTCHARSET | color_pair);
    set_field_buffer(field, 0, frame.c_str());
}
