#include <sstream>
#include <iomanip>

#include "ui/scheduler/PCBTable.h"

using namespace std;

const string PCBTable::SEPARATOR = " | ";

PCBTable::PCBTable(WINDOW *window)
        : ProcessesTable(window) {

    stringstream heading;

    heading << setw(3) << "PID" << SEPARATOR
            << setw(3) << "EST" << SEPARATOR
            << setw(3) << "TLL" << SEPARATOR
            << setw(3) << "TF" << SEPARATOR
            << setw(3) << "TRT" << SEPARATOR
            << setw(3) << "TE" << SEPARATOR
            << setw(3) << "TS" << SEPARATOR
            << setw(3) << "TR" << SEPARATOR
            << setw(3) << "TB" << SEPARATOR
            << setw(3) << "TME";

    setHeading(heading.str());
}

void PCBTable::post() {
    ProcessesTable::post();
}

void PCBTable::printRow(Process *process) {
    stringstream row;

    row << setw(3) << process->program_number << SEPARATOR
        << setw(3) << process->state << SEPARATOR
        << formatTime(process->arrival_time, 3) << SEPARATOR
        << formatTime(process->termination_time, 3) << SEPARATOR
        << formatTime(process->turnaround_time, 3) << SEPARATOR
        << formatTime(process->waiting_time, 3) << SEPARATOR
        << formatTime(process->service_time, 3) << SEPARATOR
        << formatTime(process->response_time, 3) << SEPARATOR
        << formatTime(process->blocked_time, 3) << SEPARATOR
        << formatTime(process->estimated_time, 3);

    waddstr(data_win, row.str().c_str());
}

string PCBTable::formatTime(int value, int width) {
    stringstream ss_time;

    ss_time << setw(width);

    if (value == -1) {
        ss_time << "NA";
    } else {
        ss_time << value;
    }

    return ss_time.str();
}
