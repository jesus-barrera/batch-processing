#ifndef _BATCH_PANEL_INCLUDED_
#define _BATCH_PANEL_INCLUDED_

#include "./Panel.h"
#include "./Batch.h"

class BatchPanel: public Panel {
public:
    BatchPanel(WINDOW *parent, int nlines, int ncols, int begin_x, int begin_y);

    void display(Batch *data);
    void post();
};

#endif
