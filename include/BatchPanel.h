#ifndef _BATCH_PANEL_INCLUDED_
#define _BATCH_PANEL_INCLUDED_

#include "./Panel.h"
#include "./Batch.h"

class BatchPanel: public Panel {
public:
    typedef void (*PrintRowFunc)(WINDOW *, Process *);

    BatchPanel(WINDOW *parent, std::string title, int nlines, int ncols, int begin_x, int begin_y);

    void setPrintRowFunc(PrintRowFunc func);
    void setHeader(string str);
    void display(Batch *data);
    void post();

private:
    Batch *processes;
    std::string header;
    PrintRowFunc print_row_func;
};

#endif
