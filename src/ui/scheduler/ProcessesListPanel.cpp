#include "ui/scheduler/ProcessesListPanel.h"

ProcessesListPanel::ProcessesListPanel(
        WINDOW *parent,
        std::string title,
        int nlines, int ncols,
        int begin_x, int begin_y)

        : Panel(parent, title, nlines, ncols, begin_x, begin_y),
          ProcessesTable(inner_win) {
}

ProcessesListPanel::~ProcessesListPanel() { }

void ProcessesListPanel::post() {
    Panel::post();
    ProcessesTable::post();
}
