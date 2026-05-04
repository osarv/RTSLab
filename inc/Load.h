#ifndef _LOAD_H
#define _LOAD_H

#include "TinyTimber.h"

typedef struct {
  Object super;
  int loopRange;
  int deadLine;
} Load;

#define initLoad()                                                              \
  { initObject(), 0}

void LoadRun(Load* self, int unused);
void LoadToggleDeadline(Load* self, int unused);
void LoadIncrease(Load* self, int unused);
void LoadDecrease(Load* self, int unused);

#endif