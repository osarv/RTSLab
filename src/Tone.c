#include <stdlib.h>
#include <stdio.h>
#include "sciTinyTimber.h"
#include "stdio.h"
#include "tone.h"

extern Serial sci0;
#define ARG_UNUSED 0
#define DAC (volatile unsigned char*) (0x4000741C)

void ToneGenerate(Tone* self, int arg) {
  if (self->running) {
    self->waveStatus ^= 1;
    *DAC = self->waveStatus * self->amplitude;
  }
  AFTER(self->period, self, ToneGenerate, ARG_UNUSED);
}

void ToneToggleDeadline(Tone* self, int unused) {
    self->deadLine ^= 1;
    char buf[100];
    if (self->deadLine) snprintf(buf, 100, "tone generator deadline on\n");
    else snprintf(buf, 100, "tone generator deadline off\n");
    SCI_WRITE(&sci0, buf);
}

void ToneSetPeriod(Tone* self, int period) {
  self->period = period;
    char buf[100];
    snprintf(buf, 100, "new period is %d us\n", USEC_OF(period));
    SCI_WRITE(&sci0, buf);
}

void ToneIncreaseAmplitude(Tone* self, int unused) {
  if (self->amplitude >= 10) return;
  self->amplitude++;
  char buf[100];
  snprintf(buf, 100, "new volume is %d\n", self->amplitude);
  SCI_WRITE(&sci0, buf);
}

void ToneDecreaseAmplitude(Tone* self, int unused) {
  if (self->amplitude <= 0) return;
  self->amplitude--;
  char buf[100];
  snprintf(buf, 100, "new volume is %d\n", self->amplitude);
  SCI_WRITE(&sci0, buf);
}

void ToneToggle(Tone* self, int arg) {
  self->running ^= 1;
  char buf[100];
  if (self->running) snprintf(buf, 100, "unmute\n");
  else snprintf(buf, 100, "mute\n");
  SCI_WRITE(&sci0, buf);
}