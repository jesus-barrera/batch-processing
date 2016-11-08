#ifndef _PROCESSES_LIST_PANEL_INCLUDED_
#define _PROCESSES_LIST_PANEL_INCLUDED_

#include "ui/Panel.h"
#include "ui/Table.h"
#include "Process.h"

typedef Table<Process *> ProcessesTable;

class ProcessesListPanel: public Panel, public ProcessesTable {
public:
    ProcessesListPanel(
        WINDOW *parent,
        std::string title,
        int nlines, int ncols,
        int begin_x, int begin_y);

    virtual ~ProcessesListPanel();

    void post();

protected:
    virtual void printRow(Process *process) = 0;
};

#endif
