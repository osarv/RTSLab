#include "Load.h"
#include "sciTinyTimber.h"
#include "stdio.h"

extern Serial sci0;

#define ARG_UNUSED 0
void LoadRun(Load* self, int unused) {
    for (int i = 0; i < self->loopRange; i++);
    if (self->deadLine) SEND(USEC(1300), 2 * USEC(1300), self, LoadRun, ARG_UNUSED);
    else AFTER(USEC(1300), self, LoadRun, ARG_UNUSED);
}

void LoadToggleDeadline(Load* self, int unused) {
    self->deadLine ^= 1;
    char buf[100];
    if (self->deadLine) snprintf(buf, 100, "load deadline on\n");
    else snprintf(buf, 100, "load deadline off\n");
    SCI_WRITE(&sci0, buf);
}

void LoadIncrease(Load* self, int unused) {
    self->loopRange += 500;
    char buf[100];
    snprintf(buf, 100, "new load range is %d\n", self->loopRange);
    SCI_WRITE(&sci0, buf);
}

void LoadDecrease(Load* self, int unused) {
    self->loopRange -= 500;
    if (self->loopRange <= 0) self->loopRange = 0;
    char buf[100];
    snprintf(buf, 100, "new load range is %d\n", self->loopRange);
    SCI_WRITE(&sci0, buf);
}