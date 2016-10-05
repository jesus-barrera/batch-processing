#ifndef _PROCESSES_LIST_PANEL_INCLUDED_
#define _PROCESSES_LIST_PANEL_INCLUDED_

#include "./Panel.h"
#include "./ProcessList.h"

class ProcessesListPanel: public Panel {
public:
    ProcessesListPanel(WINDOW *parent, std::string title, int nlines, int ncols, int begin_x, int begin_y);
    virtual ~ProcessesListPanel();

    void post();
    void setProcesses(ProcessList &list);

protected:
    void setColumnsHeading(std::string heading);
    virtual void printProcess(Process *process) = 0;

private:
    std::string columns_heading;
};

#endif
