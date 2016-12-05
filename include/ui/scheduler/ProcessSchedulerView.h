#ifndef _PROCESS_SCHEDULER_VIEW_INCLUDED_
#define _PROCESS_SCHEDULER_VIEW_INCLUDED_

#include "SummaryDisplay.h"
#include "BlockedProcessesPanel.h"
#include "FinishedProcessesPanel.h"
#include "ReadyProcessesPanel.h"
#include "ProcessPanel.h"
#include "MemoryPanel.h"
#include "PCBTable.h"
#include "PageTableView.h"

class ProcessScheduler;

class ProcessSchedulerView {
public:
    enum {
        READY_PANEL,
        BLOCKED_PANEL,
        PROCESS_PANEL,
        TERMINATED_PANEL,
        MEMORY_PANEL,
        NUM_PANELS
    };

    ProcessScheduler *scheduler;

    ProcessSchedulerView(ProcessScheduler *scheduler);
    ~ProcessSchedulerView();

    void displayPanels();
    void displayPCBs();
    void displayPageTables();

    void update();

private:
    // view elements
    SummaryDisplay *summary;

    // subwindows
    WINDOW *panels_window, *summary_window;

    // main panels
    ReadyProcessesPanel *ready_panel;
    ProcessPanel *process_panel;
    BlockedProcessesPanel *blocked_panel;
    FinishedProcessesPanel *terminated_panel;
    MemoryPanel *memory_panel;

    // secondary views
    PCBTable *pcb_table;
    PageTableView *page_table;

    void initWindows();
    void initPanels();
    void updateSummary();
    void updatePanels();
};

#endif
