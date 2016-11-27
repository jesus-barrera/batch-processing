#include "MemoryManager.h"
#include "Process.h"

MemoryManager::MemoryManager() {
    initMemory();

    os_pages = allocate(OS_SIZE);
}

MemoryManager::~MemoryManager() {
    free(os_pages);
}

bool MemoryManager::loadProcess(Process *process) {
    if (process->size <= getFreeSpace()) {
        tables[process->pid] = allocate(process->size);

        return true;
    } else {

        return false;
    }
}

void MemoryManager::removeProcess(unsigned int pid) {
    // free used frames
    free(tables[pid]);

    // remove table
    tables.erase(tables.find(pid));
}

unsigned int MemoryManager::getFreeSpace() {
    return free_frames.size() * FRAME_SIZE;
}

void MemoryManager::initMemory() {
    // set all frames as available
    for (int i = 0; i < TOTAL_FRAMES; i++) {
        free_frames.push_back(i);
    }
}

MemoryManager::PageTable *MemoryManager::allocate(unsigned int size) {
    unsigned int pages;

    // split into pages
    pages = size / FRAME_SIZE + (size % FRAME_SIZE != 0);

    if (pages <= free_frames.size()) {
        PageTable::iterator start, end;
        PageTable *table;

        // get necessary frames
        start = free_frames.begin();
        end = start + pages;

        // create new table using available frames
        table = new PageTable(start, end);

        // remove used frames
        free_frames.erase(start, end);

        return table;
    } else {

        return nullptr;
    }
}

void MemoryManager::free(PageTable *table) {
    free_frames.insert(free_frames.end(), table->begin(), table->end());

    delete(table);
}
