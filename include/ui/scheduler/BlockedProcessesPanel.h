#ifndef _BLOCKED_PROCESSES_PANEL_INCLUDED_
#define _BLOCKED_PROCESSES_PANEL_INCLUDED_

#include "./ProcessesListPanel.h"

class BlockedProcessesPanel: public ProcessesListPanel {
public:
    BlockedProcessesPanel(WINDOW *parent, int nlines, int ncols, int begin_x, int begin_y);

protected:
    void printProcess(Process *process);
};

#endif
