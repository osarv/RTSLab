#ifndef _TONE_H
#define _TONE_H
#include "TinyTimber.h"

typedef struct {
  Object super;
  int mute;
  int running;
  int period;
  int waveStatus;
  int amplitude;
  int deadLine;
} Tone;

#define initTone()                                                              \
  { initObject(), 0, 0, 0, 0, 0, 0}

void ToneGenerate(Tone* self, int unused);
void ToneSetPeriod(Tone* self, int period);
void ToneSetAmplitude(Tone* self, int ampl);
void ToneToggleRunning(Tone* self, int unused);
void ToneToggleMute(Tone* self, int unused);

#endif