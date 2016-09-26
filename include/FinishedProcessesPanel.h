#ifndef _FIINISHED_PROCESSES_PANEL_INCLUDED_
#define _FIINISHED_PROCESSES_PANEL_INCLUDED_

#include "./ProcessesListPanel.h"

class FinishedProcessesPanel: public ProcessesListPanel {
public:
    FinishedProcessesPanel(WINDOW *parent, int nrows, int ncols, int begin_x, int begin_y);

protected:
    void printProcess(Process *process);
};

#endif
