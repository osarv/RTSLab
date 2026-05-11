#include <stdlib.h>
#include <stdio.h>
#include "Music.h"
#include "Tone.h"
#include "tinyTimber.h"
#include "sciTinyTimber.h"
#include "sioTinyTimber.h"

extern Tone tone;
extern Serial sci0;
extern SysIO LED;

#define ARG_UNUSED 0
typedef enum {
  EIGHTH = 1,
  QUARTER = 2,
  HALF = 4,
} NoteLen;

const int notes[] = {0, 2, 4, 0, 0, 2, 4, 0, 4, 5, 7, 4, 5, 7, 7, 9, 7, 5, 4, 0, 7, 9, 7, 5, 4, 0, 0, -5, 0, 0, -5, 0};
const int periods[] = {2024, 1911, 1804, 1703, 1607, 1517, 1432, 1351, 1276, 1204, 1136, 1073, 1012, 956, 902, 851, 804, 758, 716, 676, 638, 602, 568, 536, 506};
const NoteLen noteLens[] = {QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, HALF, QUARTER, QUARTER, HALF, EIGHTH, EIGHTH, EIGHTH, EIGHTH, QUARTER, QUARTER, EIGHTH, EIGHTH, EIGHTH, EIGHTH, QUARTER, QUARTER, QUARTER, QUARTER, HALF, QUARTER, QUARTER, HALF};

void MusicSetTempo(Music* self, int tempo) {
    self->tempo = tempo;
    char buf[100];
    snprintf(buf, 100, "new tempo: %d bpm\n", tempo);
    SCI_WRITE(&sci0, buf);
}

void MusicSetKey(Music* self, int key) {
    self->key = key;
    char buf[100];
    snprintf(buf, 100, "new key: %d\n", key);
    SCI_WRITE(&sci0, buf);
}

#define WAIT_OVER_BPM (SEC(60) / 16)
void MusicPlayNote(Music* self, int noteIdx) {
    if (!self->playing) {
        self->canPlayAgain = 1;
        return;
    }

    SYNC(&tone, ToneSetPeriod, USEC(periods[notes[noteIdx] + self->key + 10]));
    int noteLen = noteLens[noteIdx];
    int sendPlayNoteBLine = (SEC(60) * noteLen) / 2 / self->tempo;
    int sendPauseNoteBLine = ((SEC(60) * noteLen)  - (WAIT_OVER_BPM * noteLen)) / 2 / self->tempo;
    SYNC(&tone, ToneToggleRunning, ARG_UNUSED);
    SEND(sendPauseNoteBLine, sendPlayNoteBLine, &tone, ToneToggleRunning, ARG_UNUSED);
    if (noteIdx >= 31) {
        self->playing = 0;
        AFTER(sendPlayNoteBLine, self, MusicPlayBJ, ARG_UNUSED);
    }
    else SEND(sendPlayNoteBLine, 2 * sendPlayNoteBLine, self, MusicPlayNote, noteIdx +1);
}

#define MAX_VOL 15
void MusicIncreaseVolume(Music* self, int unused) {
    if (self->volume < MAX_VOL) self->volume++;
    SYNC(&tone, ToneSetAmplitude, self->volume);
}

void MusicDecreaseVolume(Music* self, int unused) {
    if (self->volume > 0) self->volume--;
    SYNC(&tone, ToneSetAmplitude, self->volume);
}

void MusicMuteUnmute(Music* self, int unused) {
    SYNC(&tone, ToneToggleMute, ARG_UNUSED);
}

void MusicPlayBJ(Music* self, int noteNr) {
    if (self->playing || !self->canPlayAgain) {
        return;
    }
    self->playing = 1;
    BEFORE(noteLens[0] / self->tempo, self, MusicPlayNote, 0);
}

void MusicStopBJ(Music* self, int unused) {
    self->playing = 0;
    self->canPlayAgain = 1;
}