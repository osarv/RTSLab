#include <stdlib.h>
#include <stdio.h>
#include "sciTinyTimber.h"
#include "stdio.h"
#include "tone.h"

extern Serial sci0;
#define ARG_UNUSED 0
#define DAC_TONE (volatile unsigned char*) (0x4000741C)

void ToneGenerate(Tone* self, int unused) {
  if (self->running && !self->mute) {
    self->waveStatus ^= 1;
    *DAC_TONE = self->waveStatus * self->amplitude;
  }
  AFTER(self->period, self, ToneGenerate, ARG_UNUSED);
}

void ToneSetPeriod(Tone* self, int period) {
  self->period = period;
}

void ToneSetAmplitude(Tone* self, int ampl) {
  self->amplitude = ampl;
  char buf[100];
  snprintf(buf, 100, "volume is %d\n", ampl);
  SCI_WRITE(&sci0, buf);
}

void ToneToggleRunning(Tone* self, int unused) {
  self->running ^= 1;
}

void ToneToggleMute(Tone* self, int unused) {
  self->mute ^= 1;
  char buf[100];
  if (self->mute) snprintf(buf, 100, "mute\n");
  else snprintf(buf, 100, "unmute\n");
  SCI_WRITE(&sci0, buf);
}