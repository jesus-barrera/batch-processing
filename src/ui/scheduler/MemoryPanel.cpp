#include "ui/scheduler/MemoryPanel.h"

MemoryPanel::MemoryPanel(
        WINDOW *parent,
        int nlines, int ncols,
        int begin_y, int begin_x)
        : Panel(parent, "Memoria", nlines, ncols, begin_y, begin_x),
          MemoryView(inner_win) { }

MemoryPanel::~MemoryPanel() {}

void MemoryPanel::post() {
    Panel::post();
    MemoryView::post();
}
