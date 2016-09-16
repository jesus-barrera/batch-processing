#ifndef _BATCH_INCLUDED_
#define _BATCH_INCLUDED_

#include <deque>
#include "./Process.h"

#define PROCESSES_PER_BATCH 5

typedef deque<Process *> Batch;

#endif
