#include <stdlib.h>
#include <stdio.h>
#include "Music.h"
#include "Tone.h"
#include "tinyTimber.h"
#include "sciTinyTimber.h"

extern Tone tone;
extern Serial sci0;

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
    snprintf(buf, 100, "new tempo: &d\n", tempo);
    SCI_WRITE(&sci0, buf);
}

void MusicSetKey(Music* self, int key) {
    self->key = key;
    char buf[100];
    snprintf(buf, 100, "new key: &d\n", key);
    SCI_WRITE(&sci0, buf);
}

#define WAIT_OVER_BPM (SEC(60) / 16)
void MusicPlayNote(Music* self, int noteIdx) {
    SYNC(&tone, ToneSetPeriod, periods[notes[noteIdx + self->key]]);
    int noteLen = noteLens[noteIdx];
    int sendPlayNoteBLine = (SEC(60) * noteLen) / 2 / self->tempo;
    int sendPauseNoteBLine = (SEC(60) * noteLen  - WAIT_OVER_BPM) / 2 / self->tempo;
    SYNC(&tone, ToneToggleRunning, ARG_UNUSED);
    SEND(sendPauseNoteBLine, sendPlayNoteBLine, &tone, ToneToggleRunning, noteIdx);
    SEND(sendPlayNoteBLine, 2 * sendPlayNoteBLine, self, MusicPlayNote, noteIdx);
}

#define MAX_VOL 15
void MusicIncreaseVolume(Music* self, int unused) {
    if (self->volume < MAX_VOL) self->volume++;
    ToneSetAmplitude(&tone, self->volume);
}

void MusicDecreaseVolume(Music* self, int unused) {
    if (self->volume > 0) self->volume--;
    ToneSetAmplitude(&tone, self->volume);
}

void MusicMuteUnmute(Music* self, int unused) {
    SYNC(&tone, ToneToggleMute, ARG_UNUSED);
}

void MusicPlayBJ(Music* self, int unused) {
    BEFORE(noteLens[0] / self->tempo, self, MusicPlayNote, 0);
}