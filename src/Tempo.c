#include "TinyTimber.h"
#include "sciTinyTimber.h"
#include "stdio.h"
#include "tempo.h"

#define ARG_UNUSED 0

extern Serial sci0;
void printTimeIntervalMs(Time t) {
    char buf[100];
    snprintf(buf, 100, "time interval: %d ms\n", 1000 * SEC_OF(t) + MSEC_OF(t));
    SCI_WRITE(&sci0, buf);
}

/*void checkPressAnHold(Tempo* self, int unused) {
    Time sample = T_SAMPLE(self->timer);
    if (self->timer >= SEC(1)) self->mode = MODE_PRESS_AND_HOLD;
}
    */

void buttonPressed(Tempo* self, int unused) {
    Time sample = T_SAMPLE(&self->timer);
    if (sample < MSEC(100)) return;
    if (self->mode == MODE_MOMENTARY) {
        printTimeIntervalMs(sample);
        return;
    }
    T_RESET(&self->timer);
    //AFTER(SEC(1), self, checkPressAndHold, ARG_UNUSED);
    //SIO_TRIG(1);
}