#ifndef _MEMORY_PANEL_INCLUDED_
#define _MEMORY_PANEL_INCLUDED_

#include "ui/Panel.h"
#include "MemoryView.h"

class MemoryPanel: public Panel, public MemoryView {
public:
    MemoryPanel(
        WINDOW *parent,
        int nlines, int ncols,
        int begin_y, int begin_x);

    ~MemoryPanel();

    void post();
    void unpost();
};

#endif
