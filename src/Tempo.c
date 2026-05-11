#include "TinyTimber.h"
#include "sciTinyTimber.h"
#include "stdio.h"
#include "tempo.h"

#define ARG_UNUSED 0

extern Serial sci0;
extern SysIO button;

void printWithIntArg(char* msg, int arg) {
    char buf[100];
    snprintf(buf, 100, msg, arg);
    SCI_WRITE(&sci0, buf);
}

void checkPressAndHold(Tempo* self, int unused) {
    Time sample = T_SAMPLE(&self->lastPressTimer);
    if (SIO_READ(&button)) return;
    if (sample >= SEC(1)) {
        self->mode = MODE_PRESS_AND_HOLD;
        SCI_WRITE(&sci0, "press and hold mode entered!\n");
        SIO_TRIG(&button, 1);
    }
}

int TOT_MS_OF(Time t) {
    return 1000 * SEC_OF(t) + MSEC_OF(t);
}

void buttonEvent(Tempo* self, int unused) {
    Time sample = T_SAMPLE(&self->lastPressTimer);

    //reject debounces
    if (sample < MSEC(100)) return;

    if (SIO_READ(&button)) {
        //released
        SIO_TRIG(&button, 0);
        if (self->mode == MODE_MOMENTARY) return;
        Time sample = T_SAMPLE(&self->lastPressTimer);
        printWithIntArg("time held: %d ms\n", TOT_MS_OF(sample));
        self->mode = MODE_MOMENTARY;

    }
    else {
        //pressed
        if (self->firstPress) self->firstPress = 0;
        else printWithIntArg("time interval: %d ms\n", TOT_MS_OF(sample));
        T_RESET(&self->lastPressTimer);
        AFTER(SEC(1), self, checkPressAndHold, ARG_UNUSED);
    }
}