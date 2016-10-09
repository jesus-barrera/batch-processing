#include <sstream>

#include "../include/ProcessScheduler.h"
#include "../include/screen.h"

ProcessScheduler::ProcessScheduler() {
    // counters
    new_processes_counter = new Field<unsigned int>(screen, "Procesos nuevos: ", 0, 0);
    total_time_counter = new Field<unsigned int>(screen, "Tiempo total: ", 1, 0);

    // place panels
    ready_panel = new ReadyProcessesPanel(screen, 8, 17, 2, 0);
    blocked_panel = new BlockedProcessesPanel(screen, 8, 17, 2, 17);
    process_panel = new ProcessPanel(screen, 8, 34, 10, 0);
    finished_panel = new FinishedProcessesPanel(screen, 16, 35, 2, 42);

    help_win = derwin(screen, 1, SCREEN_COLS, SCREEN_LINES - 1, 0);
    syncok(help_win, TRUE);
}

ProcessScheduler::~ProcessScheduler() {
    delete(new_processes_counter);
    delete(total_time_counter);

    delete(ready_panel);
    delete(blocked_panel);
    delete(process_panel);
    delete(finished_panel);

    delwin(help_win);

    while (!finished_processes.empty()) {
        delete(finished_processes.back());
        finished_processes.pop_back();
    }
}

/**
 * Writes all elements to screen.
 */
void ProcessScheduler::post() {
    curs_set(0);
    noecho();

    // show counters
    new_processes_counter->post();
    new_processes_counter->setValue(new_processes.size());

    total_time_counter->post();
    total_time_counter->setValue(0);

    // show panels
    ready_panel->post();
    blocked_panel->post();
    process_panel->post();
    finished_panel->post();
}

/**
 * Prints a message in the help window. Previous content is erased.
 */
void ProcessScheduler::setMessage(std::string message) {
    werase(help_win);
    mvwaddstr(help_win, 0, 0, message.c_str());
}

/**
 * Generates a given number of processes randomly. All processes are stored in
 * the new processes list.
 */
void ProcessScheduler::generateProcesses(int num_of_processes) {
    for (int count = 0; count < num_of_processes; count++) {
        new_processes.push_back(Process::newRandom());
    }
}

void ProcessScheduler::waitForKey(string message, int key) {
    bool is_nodelay_set;

    setMessage(message);

    is_nodelay_set = is_nodelay(screen);
    if (is_nodelay_set) nodelay(screen, FALSE);

    if (key == ANY_KEY) {
        wgetch(screen);
    } else {
        while (wgetch(screen) != key);
    }

    if (is_nodelay_set) nodelay(screen, TRUE);
}

/**
 * Run all the processes.
 */
void ProcessScheduler::runSimulation() {
    unsigned int new_time;
    unsigned int old_time;
    unsigned int num_of_processes;

    wtimeout(screen, 500);

    printHelp();

    num_of_processes = new_processes.size();
    running_process = nullptr;

    timer.start();
    new_time = 0;

    while (finished_processes.size() < num_of_processes) {
        load();
        serve();

        updateView();
        handleKey(wgetch(screen));

        time_step = (new_time = timer.getSeconds()) - old_time;
        old_time = new_time;

        updateRunningProcess();
        updateBlockedProcesses();
    }

    timer.pause();

    updateView();

    wtimeout(screen, -1);
}

/**
 *
 */
void ProcessScheduler::showResults() {
    ProcessList::iterator it;
    Process *process;

    werase(screen);
    wattron(screen, A_REVERSE);
    mvwprintw(
        screen,
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
    wattroff(screen, A_REVERSE);

    for (it = finished_processes.begin(); it != finished_processes.end(); it++) {
        process = *it;

        wprintw(
            screen,
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
 * Serves the next ready process
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

int ProcessScheduler::getTotalActiveProcesses() {
    return ready_processes.size() + blocked_processes.size() + (running_process != nullptr);
}

/**
 * Updates the running process service time, and check if it has terminated.
 */
void ProcessScheduler::updateRunningProcess() {
    if (running_process != nullptr) {
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

    finished_processes.push_back(process);
}

/**
 * Handles a key press for a runnig process.
 */
void ProcessScheduler::handleKey(int key) {
    switch (key) {
        case INTERRUPT_KEY:
            if (running_process) {
                interrupt();
            }
            break;

        case ERROR_KEY:
            if (running_process) {
                terminate(Process::ERROR);
            }
            break;

        case PAUSE_KEY:
            pause();
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
    timer.pause();

    waitForKey("Pausado, presiona 'c' para continuar...", CONTINUE_KEY);

    printHelp();
    timer.start();
}

/**
 * Updates panels with the current data.
 */
void ProcessScheduler::updateView() {
    new_processes_counter->setValue(new_processes.size());
    total_time_counter->setValue(timer.getSeconds());

    process_panel->display(running_process);
    ready_panel->setProcesses(ready_processes);
    blocked_panel->setProcesses(blocked_processes);
    finished_panel->setProcesses(finished_processes);
}

/**
 * Shows a list of options in the help window.
 */
void ProcessScheduler::printHelp() {
    stringstream message;
    string separator;

    separator = ", ";

    message << INTERRUPT_KEY << ": interrupcion" << separator;
    message << ERROR_KEY << ": error" << separator;
    message << PAUSE_KEY << ": pausar";

    setMessage(message.str());
}
