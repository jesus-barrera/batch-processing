#include "ui/scheduler/ProcessSchedulerView.h"
#include "ui/screen.h"
#include "ui/GridLayout.h"
#include "ProcessScheduler.h"

ProcessSchedulerView::ProcessSchedulerView(ProcessScheduler *scheduler) {
    this->scheduler = scheduler;

    initWindows();
    initPanels();

    summary = new SummaryDisplay(summary_window);

    // secondary views
    pcb_table = new PCBTable(content);
    page_table = new PageTableView(content);
}

ProcessSchedulerView::~ProcessSchedulerView() {
    delete(ready_panel);
    delete(blocked_panel);
    delete(process_panel);
    delete(terminated_panel);
    delete(memory_panel);
    delete(pcb_table);
    delete(page_table);
    delwin(summary_window);
    delwin(panels_window);
}

/**
 * Writes all elements to screen.
 */
void ProcessSchedulerView::displayPanels() {
    wclear(content);

    summary->post();
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
    wclear(content);

    page_table->setData(scheduler->memory);
}

/**
 * Updates panels with the current data.
 */
void ProcessSchedulerView::update() {
    updateSummary();
    updatePanels();
}

void ProcessSchedulerView::initWindows() {
    summary_window = derwin(content, 2, COLS, 0, 0);
    syncok(summary_window, TRUE);

    panels_window = derwin(content, CONTENT_LINES - 2, COLS, 2, 0);
    syncok(panels_window, TRUE);
}

void ProcessSchedulerView::initPanels() {
    GridLayout grid;

    grid.setProperties(panels_window, 2, 6);

    grid.add(1, 1, 0, 0); // ready panel
    grid.add(1, 1, 0, 1); // blocked panel
    grid.add(1, 2, 1, 0); // process panel
    grid.add(2, 2, 0, 2); // terminated panel
    grid.add(2, 2, 0, 4); // memory panel

    ready_panel = new ReadyProcessesPanel(
        panels_window,
        grid[READY_PANEL].height,
        grid[READY_PANEL].width,
        grid[READY_PANEL].y,
        grid[READY_PANEL].x
    );

    blocked_panel = new BlockedProcessesPanel(
        panels_window,
        grid[BLOCKED_PANEL].height,
        grid[BLOCKED_PANEL].width,
        grid[BLOCKED_PANEL].y,
        grid[BLOCKED_PANEL].x
    );

    process_panel = new ProcessPanel(
        panels_window,
        grid[PROCESS_PANEL].height,
        grid[PROCESS_PANEL].width,
        grid[PROCESS_PANEL].y,
        grid[PROCESS_PANEL].x
    );

    terminated_panel = new FinishedProcessesPanel(
        panels_window,
        grid[TERMINATED_PANEL].height,
        grid[TERMINATED_PANEL].width,
        grid[TERMINATED_PANEL].y,
        grid[TERMINATED_PANEL].x
    );

    memory_panel = new MemoryPanel(
        panels_window,
        grid[MEMORY_PANEL].height,
        grid[MEMORY_PANEL].width,
        grid[MEMORY_PANEL].y,
        grid[MEMORY_PANEL].x
    );
}

void ProcessSchedulerView::updateSummary() {
    Process *next_process;

    if (scheduler->new_processes.empty()) {
        next_process = nullptr;
    } else {
        next_process = scheduler->new_processes.front();
    }

    summary->setNewProcesses(scheduler->new_processes.size());
    summary->setGlobalTime(scheduler->global_time);
    summary->setQuantum(scheduler->quantum);
    summary->setNextProcess(next_process);
}

void ProcessSchedulerView::updatePanels() {
    process_panel->display(scheduler->running_process);
    ready_panel->setData(scheduler->ready_processes);
    blocked_panel->setData(scheduler->blocked_processes);
    terminated_panel->setData(scheduler->terminated_processes);
    memory_panel->setData(scheduler->memory, scheduler->pcb_table);

    if (scheduler->running_process) {
        process_panel->setQuantum(scheduler->cpu_time);
    }
}
