#ifndef _MUSIC_H
#define _MUSIC_H

#include "TinyTimber.h"

typedef struct {
  Object super;
  int mute;
  int volume;
  int tempo;
  int key;
} Music;

#define initMusic()                                                              \
  { initObject(), 0, 0, 120, 0 }

void MusicPlayBJ(Music* self, int unused);
void MusicIncreaseVolume(Music* self, int unused);
void MusicDecreaseVolume(Music* self, int unused);
void MusicSetTempo(Music* self, int tempo);
void MusicSetKey(Music* self, int key);
void MusicMuteUnmute(Music* self, int unused);

#endif