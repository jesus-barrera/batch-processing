#ifndef _MEMORY_MANAGER_INCLUDED_
#define _MEMORY_MANAGER_INCLUDED_

#include <map>
#include <vector>

using namespace std;

class Process;
class MemoryView;
class PageTableView;

class MemoryManager {
public:
    typedef vector<int> PageTable;
    typedef map<unsigned int, PageTable*> TableMap; // stores processes page tables

    static const int MEMORY_SIZE = 128;
    static const int FRAME_SIZE = 4;
    static const int TOTAL_FRAMES = MEMORY_SIZE / FRAME_SIZE;
    static const int OS_SIZE = 8;

    MemoryManager();
    ~MemoryManager();

    bool loadProcess(Process *process);
    void removeProcess(unsigned int pid);
    unsigned int getFreeSpace();

    friend MemoryView;
    friend PageTableView;

private:
    TableMap tables;

    PageTable free_frames;
    PageTable *os_pages;

    void initMemory();
    PageTable *allocate(unsigned int size);
    void free(PageTable *table);
};

#endif
