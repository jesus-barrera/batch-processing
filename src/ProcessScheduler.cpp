#include <sstream>
#include <fstream>

#include "ProcessScheduler.h"
#include "ui/scheduler/ProcessSchedulerView.h"
#include "ui/screen.h"

const char *ProcessScheduler::SUSPENDED_FILE_NAME = "suspendidos.txt";

const char *ProcessScheduler::HELP =
    "e: interrupcion | " \
    "w: error | " \
    "p: pausa | " \
    "u: crear proceso | " \
    "b: ver BCPs | " \
    "t: ver paginas | " \
    "s: suspendido | " \
    "r: recargar";

ProcessScheduler::ProcessScheduler() {
    num_of_processes = 0;
    global_time = 0;

    view = new ProcessSchedulerView((ProcessScheduler *)this);
}

ProcessScheduler::~ProcessScheduler() {
    delete(view);

    for (Process *process: pcb_table) {
        delete(process);
    }

    pcb_table.clear();
}

void ProcessScheduler::setQuantum(int quantum) {
    this->quantum = quantum;
}

/**
 * Writes all elements to screen.
 */
void ProcessScheduler::post() {
    view->displayPanels();
}

/**
 * Generates a given number of processes randomly. All processes are stored in
 * the new processes list.
 */
void ProcessScheduler::generateProcesses(int num_of_processes) {
    Process *process;

    for (int count = 0; count < num_of_processes; count++) {
        process = Process::newRandom();
        process->state = Process::NEW;

        new_processes.push_back(process);
        pcb_table.push_back(process);
    }

    this->num_of_processes += num_of_processes;
}

/**
 * Run all the processes.
 */
void ProcessScheduler::runSimulation() {
    initSimulation();

    while (terminated_processes.size() < num_of_processes) {
        if (timer.getSeconds() >= 1) {
            global_time++;

            update();

            timer.restart();
        }

        handleKey(getch());

        load();
        serve();

        view->update();
    }

    endSimulation();
}

/**
 * Shows terminated processes information.
 */
void ProcessScheduler::showResults() {
    view->displayPCBs();
}

void ProcessScheduler::initSimulation() {
    setFooter(HELP); // print help
    timeout(GETCH_TIMEOUT); // set getch timeout

    timer.restart();
}

void ProcessScheduler::endSimulation() {
    timeout(-1);
}

void ProcessScheduler::update() {
    updateReadyProcesses();
    updateRunningProcess();
    updateBlockedProcesses();
}

/**
 * Loads as many processes as posible into the ready list. Returns the number of
 * loaded processes.
 */
int ProcessScheduler::load() {
    int loaded;
    Process *process;

    loaded = 0;

    while (!new_processes.empty() &&
           memory.loadProcess(process = new_processes.front())) {

        process->state = Process::READY;
        process->arrival_time = global_time;
        process->waiting_time = 0;
        process->service_time = 0;

        ready_processes.push_back(process);
        new_processes.pop_front();

        loaded++;
    }

    return loaded;
}

/**
 * Serves the next ready process.
 */
void ProcessScheduler::serve() {
    if (!running_process && ready_processes.size() > 0) {
        running_process = ready_processes.front();
        ready_processes.pop_front();

        if (running_process->response_time == -1)
            running_process->response_time = global_time - running_process->arrival_time;

        running_process->state = Process::RUNNING;
        cpu_time = 0;
    }
}

/**
 * Calculates how many processes are currently in memory.
 */
int ProcessScheduler::getTotalActiveProcesses() {
    return ready_processes.size() + blocked_processes.size() + (running_process != nullptr);
}

void ProcessScheduler::updateReadyProcesses() {
    for (ProcessList::iterator it = ready_processes.begin(); it != ready_processes.end(); it++) {
        (*it)->waiting_time++;
    }
}

/**
 * Updates the running process service time, and check if it has terminated.
 */
void ProcessScheduler::updateRunningProcess() {
    if (running_process) {
        running_process->service_time++;
        cpu_time++;

        if (running_process->service_time >= running_process->estimated_time) {
            terminate(Process::SUCCESS);
        } else if (cpu_time >= quantum) {
            interrupt();
        }
    }
}

/**
 * Updates the blocked processes.
 */
void ProcessScheduler::updateBlockedProcesses() {
    Process *process;
    ProcessList::iterator it;

    it = blocked_processes.begin();

    while (it != blocked_processes.end()) {
        process = *it;

        process->blocked_time++;
        process->waiting_time++;

        if (process->blocked_time >= MAX_BLOCKED_TIME) {
            process->state = Process::READY;
            process->blocked_time = -1;

            ready_processes.push_back(process);
            it = blocked_processes.erase(it);
        } else {
            it++;
        }
    }
}

/**
 * Terminate the current running process.
 */
void ProcessScheduler::terminate(short reason) {
    Process *process;

    process = running_process;
    running_process = nullptr;

    if (reason == Process::SUCCESS) process->run();

    process->termination_status = reason;

    process->termination_time = global_time;
    process->turnaround_time = process->termination_time - process->arrival_time;

    process->state = Process::TERMINATED;

    terminated_processes.push_back(process);
    memory.removeProcess(process->pid);
}

/**
 * Handles a key press for a runnig process.
 */
void ProcessScheduler::handleKey(int key) {
    switch (key) {
        case 'e': case 'E':
            if (running_process) block();
            break;

        case 'w': case 'W':
            if (running_process) terminate(Process::ERROR);
            break;

        case 'p': case 'P':
            pause();
            break;

        case 'u': case 'U':
            generateProcesses(1);
            break;

        case 'b': case 'B':
            showBCPTable();
            break;

        case 't': case 'T':
            showPageTables();
            break;

        case 's': case 'S':
            suspend();
            break;

        case 'r': case 'R':
            reload();
            break;

        default:
            break;
    }
}

void ProcessScheduler::block() {
    running_process->blocked_time = 0;
    running_process->state = Process::BLOCKED;

    blocked_processes.push_back(running_process);
    running_process = NULL;
}

void ProcessScheduler::interrupt() {
    ready_processes.push_back(running_process);
    running_process->state = Process::READY;
    running_process = NULL;
}

void ProcessScheduler::suspend() {
    Process *process;

    if (!blocked_processes.empty()) {
        // remove process
        process = blocked_processes.front();
        blocked_processes.pop_front();

        memory.removeProcess(process->pid);

        // suspend
        process->state = Process::SUSPENDED;
        suspended_processes.push_back(process);

        saveFile();
    }
}

void ProcessScheduler::reload() {
    Process *process;

    if (!suspended_processes.empty() &&
        memory.loadProcess(process = suspended_processes.front())) {

        suspended_processes.pop_front();

        // blocking
        process->state = Process::BLOCKED;
        blocked_processes.push_back(process);

        saveFile();
    }
}

void ProcessScheduler::saveFile() {
    ofstream file(SUSPENDED_FILE_NAME, ios_base::out);

    for (Process *process: suspended_processes) {
        file << "P" << process->pid << endl;
    }

    file.close();
}

/**
 * Pauses the simulation.
 */
void ProcessScheduler::pause() {
    int ch;

    enterPause();

    while ((ch = getch()) != 'c' && ch != 'C');

    leavePause();
}

void ProcessScheduler::enterPause() {
    timer.pause();
    timeout(-1);
    setFooter("Pausado!, presiona c para continuar...");
}

void ProcessScheduler::leavePause() {
    timeout(GETCH_TIMEOUT);
    setFooter(HELP);
    timer.start();
}

void ProcessScheduler::showBCPTable() {
    view->displayPCBs();
    pause();
    view->displayPanels();
}

void ProcessScheduler::showPageTables() {
    view->displayPageTables();
    pause();
    view->displayPanels();
}
