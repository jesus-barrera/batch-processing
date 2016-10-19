#include <sstream>

#include "ProcessScheduler.h"
#include "ui/scheduler/ProcessSchedulerView.h"
#include "ui/screen.h"

const string ProcessScheduler::HELP =
    "e: interrupcion | " \
    "w: error | " \
    "p: pausa | " \
    "n: nuevo proceso | " \
    "b: ver BCPs";

ProcessScheduler::ProcessScheduler() {
    num_of_processes = 0;

    view = new ProcessSchedulerView((ProcessScheduler *)this);
}

ProcessScheduler::~ProcessScheduler() {
    delete(view);

    while (!terminated_processes.empty()) {
        delete(terminated_processes.back());

        terminated_processes.pop_back();
    }
}

/**
 * Writes all elements to screen.
 */
void ProcessScheduler::post() {
    view->post();
}

/**
 * Generates a given number of processes randomly. All processes are stored in
 * the new processes list.
 */
void ProcessScheduler::generateProcesses(int num_of_processes) {
    for (int count = 0; count < num_of_processes; count++) {
        new_processes.push_back(Process::newRandom());
    }

    this->num_of_processes += num_of_processes;
}

/**
 * Run all the processes.
 */
void ProcessScheduler::runSimulation() {
    unsigned int new_time;
    unsigned int old_time;

    initSimulation();

    new_time = 0;

    while (terminated_processes.size() < num_of_processes) {
        load();
        serve();

        view->update();

        handleKey(getch());

        time_step = (new_time = timer.getSeconds()) - old_time;
        old_time = new_time;

        update();
    }

    view->update();

    endSimulation();
}

void ProcessScheduler::showResults() {
    ProcessList::iterator it;
    Process *process;

    werase(content);
    wattron(content, A_REVERSE);
    mvwprintw(
        content,
        0, 0,
        "%s | %s | %s | %s | %s | %s | %s | %s\n",
        "PID",
        "Llegada",
        "Finaliza",
        "Retorno",
        "Respuesta",
        "Espera",
        "Servicio",
        "Estimado"

    );
    wattroff(content, A_REVERSE);

    for (it = terminated_processes.begin(); it != terminated_processes.end(); it++) {
        process = *it;

        wprintw(
            content,
            "%-3d | %-7d | %-8d | %-7d | %-9d | %-6d | %-8d | %-8d\n",
            process->program_number,
            process->arrival_time,
            process->termination_time,
            process->turnaround_time,
            process->response_time,
            process->waiting_time,
            process->service_time,
            process->estimated_time
        );
    }
}

void ProcessScheduler::initSimulation() {
    setFooter(HELP);
    timeout(GETCH_TIMEOUT);
    timer.restart();

    running_process = nullptr;
}

void ProcessScheduler::endSimulation() {
    timer.pause();
    timeout(-1);
}

void ProcessScheduler::update() {
    updateRunningProcess();
    updateBlockedProcesses();
}

/**
 * Loads as many processes as posible into the ready list. Returns the number of
 * loaded processes.
 */
int ProcessScheduler::load() {
    Process *process;
    int loaded;
    int max;

    max = MAX_ACTIVE_PROCESSES - getTotalActiveProcesses();

    for (loaded = 0; loaded < max && new_processes.size() > 0; loaded++) {
        process = new_processes.front();
        process->arrival_time = timer.getSeconds();

        ready_processes.push_back(process);
        new_processes.pop_front();
    }

    return loaded;
}

/**
 * Serves the next ready process.
 */
bool ProcessScheduler::serve() {
    if (!running_process && ready_processes.size() > 0) {
        running_process = ready_processes.front();

        if (running_process->response_time == -1)
            running_process->response_time = timer.getSeconds() - running_process->arrival_time;

        ready_processes.pop_front();

        return true;
    }

    return false;
}

/**
 * Calculates how many processes are currently in memory.
 */
int ProcessScheduler::getTotalActiveProcesses() {
    return ready_processes.size() + blocked_processes.size() + (running_process != nullptr);
}

/**
 * Updates the running process service time, and check if it has terminated.
 */
void ProcessScheduler::updateRunningProcess() {
    if (running_process) {
        running_process->service_time += time_step;

        if (running_process->service_time >= running_process->estimated_time) {
            terminate(Process::SUCCESS);
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

        process->blocked_time += time_step;

        if (process->blocked_time >= MAX_BLOCKED_TIME) {
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

    process->termination_time = timer.getSeconds();
    process->turnaround_time = process->termination_time - process->arrival_time;
    process->waiting_time = process->turnaround_time - process->service_time;

    terminated_processes.push_back(process);
}

/**
 * Handles a key press for a runnig process.
 */
void ProcessScheduler::handleKey(int key) {
    switch (key) {
        case 'e': case 'E':
            if (running_process) interrupt();
            break;

        case 'w': case 'W':
            if (running_process) terminate(Process::ERROR);
            break;

        case 'p': case 'P':
            pause();
            break;

        case 'n': case 'N':
            generateProcesses(1);
            break;

        default:
            break;
    }
}

void ProcessScheduler::interrupt() {
    running_process->blocked_time = 0;
    blocked_processes.push_back(running_process);

    running_process = NULL;
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
