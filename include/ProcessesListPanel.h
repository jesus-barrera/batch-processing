#ifndef _PROCESSES_LIST_PANEL_INCLUDED_
#define _PROCESSES_LIST_PANEL_INCLUDED_

#include "./Panel.h"
#include "./Batch.h"

class ProcessesListPanel: public Panel {
public:
    ProcessesListPanel(WINDOW *parent, std::string title, int nlines, int ncols, int begin_x, int begin_y);

    void post();
    void setProcesses(Batch *data);

protected:
    std::string columns_heading;

    virtual void printProcess(Process *process) = 0;
};

#endif
