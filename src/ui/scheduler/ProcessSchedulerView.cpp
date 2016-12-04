#include "ui/scheduler/ProcessSchedulerView.h"
#include "ui/screen.h"
#include "ui/GridLayout.h"
#include "ProcessScheduler.h"

ProcessSchedulerView::ProcessSchedulerView(ProcessScheduler *scheduler) {
    this->scheduler = scheduler;

    initPanels();

    pcb_table = new PCBTable(content);
}

ProcessSchedulerView::~ProcessSchedulerView() {
    delete(ready_panel);
    delete(blocked_panel);
    delete(process_panel);
    delete(terminated_panel);
    delete(memory_panel);
    delete(pcb_table);
    delwin(panels_win);
}

/**
 * Writes all elements to screen.
 */
void ProcessSchedulerView::displayPanels() {
    wclear(content);

    summary.post();
    ready_panel->post();
    blocked_panel->post();
    process_panel->post();
    terminated_panel->post();
    memory_panel->post();

    update();
}

void ProcessSchedulerView::displayPCBs() {
    pcb_table->post();
    pcb_table->setData(scheduler->pcb_table);
}

void ProcessSchedulerView::displayPageTables() {
    //
}

/**
 * Updates panels with the current data.
 */
void ProcessSchedulerView::update() {
    summary.setNewProcesses(scheduler->new_processes.size());
    summary.setGlobalTime(scheduler->global_time);
    summary.setQuantum(scheduler->quantum);

    process_panel->display(scheduler->running_process);
    ready_panel->setData(scheduler->ready_processes);
    blocked_panel->setData(scheduler->blocked_processes);
    terminated_panel->setData(scheduler->terminated_processes);
    memory_panel->setData(scheduler->memory, scheduler->pcb_table);

    if (scheduler->running_process) {
        process_panel->setQuantum(scheduler->cpu_time);
    }
}

void ProcessSchedulerView::initPanels() {
    GridLayout grid;

    panels_win = derwin(content, CONTENT_LINES - 2, COLS, 2, 0);

    syncok(panels_win, TRUE);

    grid.setProperties(panels_win, 2, 6);

    grid.add(1, 1, 0, 0); // ready panel
    grid.add(1, 1, 0, 1); // blocked panel
    grid.add(1, 2, 1, 0); // process panel
    grid.add(2, 2, 0, 2); // terminated panel
    grid.add(2, 2, 0, 4); // memory panel

    ready_panel = new ReadyProcessesPanel(
        panels_win,
        grid[READY_PANEL].height,
        grid[READY_PANEL].width,
        grid[READY_PANEL].y,
        grid[READY_PANEL].x
    );

    blocked_panel = new BlockedProcessesPanel(
        panels_win,
        grid[BLOCKED_PANEL].height,
        grid[BLOCKED_PANEL].width,
        grid[BLOCKED_PANEL].y,
        grid[BLOCKED_PANEL].x
    );

    process_panel = new ProcessPanel(
        panels_win,
        grid[PROCESS_PANEL].height,
        grid[PROCESS_PANEL].width,
        grid[PROCESS_PANEL].y,
        grid[PROCESS_PANEL].x
    );

    terminated_panel = new FinishedProcessesPanel(
        panels_win,
        grid[TERMINATED_PANEL].height,
        grid[TERMINATED_PANEL].width,
        grid[TERMINATED_PANEL].y,
        grid[TERMINATED_PANEL].x
    );

    memory_panel = new MemoryPanel(
        panels_win,
        grid[MEMORY_PANEL].height,
        grid[MEMORY_PANEL].width,
        grid[MEMORY_PANEL].y,
        grid[MEMORY_PANEL].x
    );
}
