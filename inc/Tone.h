#ifndef _TONE_H
#define _TONE_H
#include "TinyTimber.h"

typedef struct {
  Object super;
  int running;
  int period;
  int waveStatus;
  int amplitude;
  int deadLine;
} Tone;

#define initTone()                                                              \
  { initObject(), 0, 0, 0, 0, 0}

void ToneGenerate(Tone* self, int unused);
void ToneSetPeriod(Tone* self, int period);
void ToneSetAmplitude(Tone* self, int ampl);
void ToneToggle(Tone* self, int unused);

#endif