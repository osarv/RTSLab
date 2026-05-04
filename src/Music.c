#include <stdlib.h>
#include <stdio.h>
#include "Tone.h"
#include "tinyTimber.h"
#include "sciTinyTimber.h"

#define ARG_UNUSED 0
typdef enum {
  HALF,
  QUARTER,
  EIGHTH
} NoteLen;

const int notes[] = {0, 2, 4, 0, 0, 2, 4, 0, 4, 5, 7, 4, 5, 7, 7, 9, 7, 5, 4, 0, 7, 9, 7, 5, 4, 0, 0, -5, 0, 0, -5, 0};
const int periods[] = {2024, 1911, 1804, 1703, 1607, 1517, 1432, 1351, 1276, 1204, 1136, 1073, 1012, 956, 902, 851, 804, 758, 716, 676, 638, 602, 568, 536, 506};
const NoteLen noteLens[] = {QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, HALF, QUARTER, QUARTER, HALF, EIGHTH, EIGHTH, EIGHTH, EIGHTH, QUARTER, QUARTER, EIGHTH, EIGHTH, EIGHTH, EIGHTH, QUARTER, QUARTER, QUARTER, QUARTER, HALF, QUARTER, QUARTER, HALF};

void MusicSetTempo(Music* self, int tempo) {
    self->tempo = tempo;
}

void MusicSetKey(Music* self, int key) {
    self->key = key;
}

#define WAIT_OVER_BPM 50 * 100 * 120
void MusicPlayNote(Music* self, int noteIdx) {
    SYNC(&tone, ToneSetPeriod, periods[notes[noteIdx + key]]);
    SYNC(&tone, ToneToggle, ARG_UNUSED);
    int noteLen noteLens[noteIdx];
    int sendPlayNoteBLine = noteLen / self->tempo;
    int sendPauseNoteBLine = (noteLen  - WAIT_MS_OVER BPM) / self->tempo;
    SEND(sendPlayNoteBLine, 2 * sendPlayNoteBLine, self, MusicPlayNothing, noteIdx);
    SEND(sendPauseNoteBLine, sendPlayNoteBLine, self, MusicPlayNothing, noteIdx);
}

void MusicPauseNote() {

}

#define MAX_VOL 15
void MusicIncreaseVolume(Music* self, int unused) {
    if (self->volume < MAX_VOL) self->volume++;
    ToneSetAmplitude(self->volume);
}

void MusicDecreaseVolume(Music* self, int unused) {
    if (self->volume > 0) self->volume--;
    ToneSetAmplitude(self->volume);
}

void MusicMuteUnmute(Music* self, int unused) {
    self->mute ^= 1;
    char buf[100];
    if (self->mute) snprintf(buf, 100, "mute");
    else snprintf(buf, 100, "unmute");
    SCI_WRITE(&sci0, buf);
}

void MusicPlayBJ(Musc* self, int unused) {
    BEFORE(noteLens[0] / self->tempo, MusicPlayNote, 0);
}