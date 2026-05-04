#ifndef _MUSIC_H
#define _MUSIC_H

#include "TinyTimber.h"

typedef struct {
  Object super;
  int tempo;
  int key;
} Load;

#define initMusic()                                                              \
  { initObject(), 120, 0 }

void MusicSetTempo(Music* self, int tempo);
void MusicSetKey(Music* self, int key);
void MusicPlayNote(Music* self, int noteIdx);
void MusicPlayNothing(Music* self, int noteIdx);
void MusicPlayBJ(Music* self, int unused);
void MusicMute(Music* self, int unused);
void MusicUnmute(Music* self, int unused);

#endif