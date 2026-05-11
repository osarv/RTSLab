#ifndef _TEMPO_H
#define _TEMPO_H
#include "TinyTimber.h"
#include "sioTinyTimber.h"

#define MODE_MOMENTARY 0
#define MODE_PRESS_AND_HOLD 1

typedef struct {
  Object super;
  int mode;
  Timer lastPressTimer;
  int firstPress;
  int histCnt;
  Time history[3];
} Tempo;


#define initTempo()                                                              \
  { initObject(), MODE_MOMENTARY, initTimer(), 1, 0, {0} }

void buttonEvent(Tempo* self, int unused);

#endif