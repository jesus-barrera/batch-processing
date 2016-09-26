#ifndef _BATCH_PROCESSES_PANEL_INCLUDED_
#define _BATCH_PROCESSES_PANEL_INCLUDED_

#include "./ProcessesListPanel.h"

class BatchProcessesPanel: public ProcessesListPanel {
public:
    BatchProcessesPanel(WINDOW *parent, int nlines, int ncols, int begin_x, int begin_y);

protected:
    void printProcess(Process *process);
};

#endif
