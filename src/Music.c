#include <stdlib.h>
#include <stdio.h>
#include "Tone.h"
#include "tinyTimber.h"

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
    SYNC(&tone, ToneEnable, ARG_UNUSED);
    AFTER((noteLens[noteIdx] - WAIT_MS_OVER_BPM) / self->tempo, self, MusicPlayNothing, noteIdx);
}

void MusicPlayNothing(Music* self, int noteIdx) {
    SYNC(&tone, ToneDisable, ARG_UNUSED);
    if (noteIdx < (sizeof(notes) / sizeof(int)) - 1) (WAIT_OVER_BPM) / self->tempo, self, MusicPlayNote, noteIdx +1);
    else AFTER(self, MusicPlayBJ, ARG_UNUSED),;
}

void MusicPlayBJ(Musc* self, int unused) {
    BEFORE(noteLens[0] / self->tempo, MusicPlayNote, 0);
}