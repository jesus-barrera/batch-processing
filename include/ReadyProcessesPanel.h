#ifndef _READY_PROCESSES_PANEL_INCLUDED_
#define _READY_PROCESSES_PANEL_INCLUDED_

#include "./ProcessesListPanel.h"

class ReadyProcessesPanel: public ProcessesListPanel {
public:
    ReadyProcessesPanel(WINDOW *parent, int nlines, int ncols, int begin_x, int begin_y);

protected:
    void printProcess(Process *process);
};

#endif
