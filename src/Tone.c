#include <stdlib.h>
#include <stdio.h>
#include "sciTinyTimber.h"
#include "stdio.h"
#include "tone.h"

extern Serial sci0;
#define ARG_UNUSED 0
#define DAC (volatile unsigned char*) (0x4000741C)

void ToneGenerate(Tone* self, int unused) {
  if (self->running) {
    self->waveStatus ^= 1;
    *DAC = self->waveStatus * self->amplitude;
  }
  AFTER(self->period, self, ToneGenerate, ARG_UNUSED);
}

void ToneSetPeriod(Tone* self, int period) {
  self->period = period;
}

void ToneSetAmplitude(Tone* self, int ampl) {
  self->amplitude = ampl;
  char buf[100];
  snprintf(buf, 100, "volume is %d us\n", ampl);
  SCI_WRITE(&sci0, buf);
}

void ToneToggle(Tone* self, int unused) {
  self->running ^= 1;
}