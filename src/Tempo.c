#include "TinyTimber.h"
#include "sciTinyTimber.h"
#include "stdio.h"
#include "tempo.h"
#include "music.h"

#define ARG_UNUSED 0

extern Serial sci0;
extern SysIO sio;
extern Music music;

void printWithIntArg(char* msg, int arg) {
    char buf[100];
    snprintf(buf, 100, msg, arg);
    SCI_WRITE(&sci0, buf);
}

void checkTempoReset(Tempo* self, int unused) {
    Time sample = T_SAMPLE(&self->lastPressTimer);
    if (SIO_READ(&sio)) return;
    if (sample >= SEC(2)) {
        SCI_WRITE(&sci0, "tempo reset to default!\n");
        SYNC(&music, MusicSetTempo, 120);
    }
}

void checkPressAndHold(Tempo* self, int unused) {
    Time sample = T_SAMPLE(&self->lastPressTimer);
    if (SIO_READ(&sio)) return;
    if (sample >= SEC(1)) {
        self->mode = MODE_PRESS_AND_HOLD;
        SCI_WRITE(&sci0, "press and hold mode entered!\n");
        SIO_TRIG(&sio, 1);
    }
}

int TOT_MS_OF(Time t) {
    return 1000 * SEC_OF(t) + MSEC_OF(t);
}

int samplesCohere(Time history[2], int histCnt, Time sample) {
    for (int i = 0; i < histCnt; i++) {
        if (sample - history[i] > MSEC(100)) return 0;
        if (history[i] - sample > MSEC(100)) return 0;
    }
    return 1;
}

Time avgHistory(Time history[3]) {
    Time t = 0;
    for (int i = 0; i < 3; i++) {
        t += history[i];
    }
    return t / 3;
}

void buttonEvent(Tempo* self, int unused) {
    Time sample = T_SAMPLE(&self->lastPressTimer);

    //reject debounces
    if (sample < MSEC(100)) return;

    if (SIO_READ(&sio)) {
        //released
        SIO_TRIG(&sio, 0);
        if (self->mode == MODE_MOMENTARY) return;
        Time sample = T_SAMPLE(&self->lastPressTimer);
        printWithIntArg("time held: %d ms\n", TOT_MS_OF(sample));
        self->mode = MODE_MOMENTARY;
        SCI_WRITE(&sci0, "leaving press and hold!\n");

    }
    else {
        //pressed
        if (self->firstPress) self->firstPress = 0;
        else printWithIntArg("time interval: %d ms\n", TOT_MS_OF(sample));
        
        if (samplesCohere(self->history, self->histCnt, sample)) {
            self->history[self->histCnt] = sample;
            self->histCnt++;
            if (self->histCnt >= 3) {
                int bpm = SEC(60) / avgHistory(self->history);
                if (bpm <= 300 && bpm >= 30) SYNC(&music, MusicSetTempo, bpm);
                self->histCnt = 0;
            }
        }
        else {
            self->history[0] = sample;
            self->histCnt = 1;
        }

        T_RESET(&self->lastPressTimer);
        AFTER(SEC(1), self, checkPressAndHold, ARG_UNUSED);
        AFTER(SEC(2), self, checkTempoReset, ARG_UNUSED);
    }
}