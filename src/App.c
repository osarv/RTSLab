#include "App.h"
#include "Tone.h"
#include "TinyTimber.h"
#include "canTinyTimber.h"
#include "sciTinyTimber.h"
#include "stdlib.h"
#include "stdio.h"
#include "Music.h"

#define ARG_UNUSED 0
extern App app;
extern Tone tone;
extern Music music;
extern Can can0;
extern Serial sci0;

typedef struct {
  enum {
    MSG_START_BJ,
    MSG_MUTE_UNMUTE,
    MSG_VOLUME_UP,
    MSG_VOLUME_DOWN,
    MSG_NEW_TEMPO,
    MSG_NEW_KEY
  } type;
  int val;
} CANPayload;

CANPayload unpackCANMsg(CANMsg msg) {
  CANPayload* pLoad = (CANPayload*)msg.buff;
  return *pLoad;
}

CANMsg packCANMsg(CANPayload pLoad) {
  CANMsg* msg;
  msg->msgId = 1;
  msg->nodeId = 1;
  msg.length = 8;
  memcpy(&(msg.buff), &(pLoad), 8);
  return msg;
}

void CANPLoadAct(CANPayload pLoad) {
  switch(pLoad.type) {
    case MSG_START_BJ: ASYNC(&music, MusicPlayBJ, ARG_UNUSED); break;
    case MSG_MUTE_UNMUTE: SYNC(&music, MusicMuteUnmute, ARG_UNUSED); break;
    case MSG_VOLUME_UP: SYNC(&music, MusicIncreaseVolume, ARG_UNUSED); break;
    case MSG_VOLUME_DOWN: SYNC(&music, MusicDecreaseVolume, ARG_UNUSED); break;
    case MSG_NEW_KEY: SYNC(&music, MusicSetKey, pLoad.val); break;
    case MSG_NEW_TEMPO: SYNC(&music, MusicSetTempo, pLoad.val); break;
  }
}

void receiver(App *self, int unused) {
  CANMsg msg;
  msg.msgId = 0;
  msg.nodeId = 0;
  CAN_RECEIVE(&can0, &msg);
  SCI_WRITE(&sci0, "Can msg received: ");
  CANPayload pLoad = unpackCANMsg(msg);
  SCI_WRITE(&sci0, msg.buff);
}

void reader(App* self, int c) {
  switch (c) {
    case ',': SYNC(&music, MusicIncreaseVolume, ARG_UNUSED); break;
    case '.': SYNC(&music, MusicDecreaseVolume, ARG_UNUSED); break;
    case 'm': SYNC(&music, MusicMuteUnmute, ARG_UNUSED); break;
    case '0': self->buf[self->len] = c; self->len++; break;
    case '1': self->buf[self->len] = c; self->len++; break;
    case '2': self->buf[self->len] = c; self->len++; break;
    case '3': self->buf[self->len] = c; self->len++; break;
    case '4': self->buf[self->len] = c; self->len++; break;
    case '5': self->buf[self->len] = c; self->len++; break;
    case '6': self->buf[self->len] = c; self->len++; break;
    case '7': self->buf[self->len] = c; self->len++; break;
    case '8': self->buf[self->len] = c; self->len++; break;
    case '9': self->buf[self->len] = c; self->len++; break;
    case '-': self->buf[self->len] = c; self->len++; break;
    case 'k':
      self->buf[self->len] = '\0';
      int key = atoi(self->buf);
      self->len = 0;
      if (key <= 5 && key >= -5) {
        ASYNC(&music, MusicSetKey, key);
        break;
      }
      else {
        SCI_WRITE(&sci0, "Input key not allowed! >:(\n");
        break;
      }
    case 't':
      self->buf[self->len] = '\0';
      int tempo = atoi(self->buf);
      self->len = 0;
      if (tempo <= 240 && tempo >= 60){
        ASYNC(&music, MusicSetTempo, tempo);
        break;
      }
      else {
        SCI_WRITE(&sci0, "Input tempo not allowed! >:(\n");
        break;
      } 
  }
}

void startApp(App *self, int arg) {
  CANMsg msg;

  CAN_INIT(&can0);
  SCI_INIT(&sci0);
  SCI_WRITE(&sci0, "Hello, hello...\n");

  msg.msgId = 1;
  msg.nodeId = 1;
  msg.length = 6;
  msg.buff[0] = 'H';
  msg.buff[1] = 'e';
  msg.buff[2] = 'l';
  msg.buff[3] = 'l';
  msg.buff[4] = 'o';
  msg.buff[5] = 0;
  CAN_SEND(&can0, &msg);

  ASYNC(&tone, ToneGenerate, ARG_UNUSED);
  ASYNC(&music, MusicPlayBJ, ARG_UNUSED);
}

int main() {
  INSTALL(&sci0, sci_interrupt, SCI_IRQ0);
  INSTALL(&can0, can_interrupt, CAN_IRQ0);
  TINYTIMBER(&app, startApp, 0);
  return 0;
}
