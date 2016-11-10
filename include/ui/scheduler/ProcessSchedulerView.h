#ifndef _PROCESS_SCHEDULER_VIEW_INCLUDED_
#define _PROCESS_SCHEDULER_VIEW_INCLUDED_

#include "SummaryDisplay.h"
#include "BlockedProcessesPanel.h"
#include "FinishedProcessesPanel.h"
#include "ReadyProcessesPanel.h"
#include "ProcessPanel.h"
#include "PCBTable.h"

class ProcessScheduler;

class ProcessSchedulerView {
public:
    enum {
        READY_PANEL,
        BLOCKED_PANEL,
        PROCESS_PANEL,
        TERMINATED_PANEL,
        NUM_PANELS
    };

    ProcessScheduler *scheduler;

    ProcessSchedulerView(ProcessScheduler *scheduler);
    ~ProcessSchedulerView();

    void postPanels();
    void postTable();

    void update();

private:
    SummaryDisplay summary;
    PCBTable *pcb_table;

    ReadyProcessesPanel *ready_panel;
    ProcessPanel *process_panel;
    BlockedProcessesPanel *blocked_panel;
    FinishedProcessesPanel *terminated_panel;

    WINDOW *panels_win;

    void initPanels();
};

#endif
