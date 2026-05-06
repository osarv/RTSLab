#include "App.h"
#include "Tone.h"
#include "TinyTimber.h"
#include "canTinyTimber.h"
#include "sciTinyTimber.h"
#include "stdlib.h"
#include "stdio.h"
#include "Music.h"
#include "string.h"

#define ARG_UNUSED 0
extern App app;
extern Tone tone;
extern Music music;
extern Can can0;
extern Serial sci0;

  typedef enum {
    MSG_START_BJ,
    MSG_STOP_BJ,
    MSG_MUTE_UNMUTE,
    MSG_VOLUME_UP,
    MSG_VOLUME_DOWN,
    MSG_NEW_TEMPO,
    MSG_NEW_KEY
  } CANMsgType;

typedef struct {
  CANMsgType type;
  int val;
} CANPayload;

CANPayload unpackCANMsg(CANMsg msg) {
  CANPayload pLoad;
  memcpy(&pLoad, &msg.buff, sizeof(CANPayload));
  return pLoad;
}

void CanSendMsg(CANMsgType type, int val) {
  CANMsg msg;
  msg.msgId = 1;
  msg.nodeId = 1;
  msg.length = 8;
  CANPayload pLoad = {type, val};
  memcpy(&msg.buff, &pLoad, sizeof(CANPayload));
  CAN_SEND(&can0, &msg);
}

void CANPLoadAcknowledge(CANPayload pLoad) {
  switch(pLoad.type) {
    case MSG_START_BJ: SCI_WRITE(&sci0, "starting song\n"); ASYNC(&music, MusicPlayBJ, ARG_UNUSED); break;
    case MSG_STOP_BJ: SCI_WRITE(&sci0, "stopping song\n"); ASYNC(&music, MusicStopBJ, ARG_UNUSED); break;
    case MSG_MUTE_UNMUTE: SCI_WRITE(&sci0, "mute/unmute\n"); SYNC(&music, MusicMuteUnmute, ARG_UNUSED); break;
    case MSG_VOLUME_UP: SCI_WRITE(&sci0, "volume up\n"); SYNC(&music, MusicIncreaseVolume, ARG_UNUSED); break;
    case MSG_VOLUME_DOWN: SCI_WRITE(&sci0, "volume down\n"); SYNC(&music, MusicDecreaseVolume, ARG_UNUSED); break;
    case MSG_NEW_KEY: SCI_WRITE(&sci0, "new key\n"); SYNC(&music, MusicSetKey, pLoad.val); break;
    case MSG_NEW_TEMPO: SCI_WRITE(&sci0, "new tempo\n"); SYNC(&music, MusicSetTempo, pLoad.val); break;
  }
}


void CANPLoadAct(CANPayload pLoad) {
  switch(pLoad.type) {
    case MSG_START_BJ: ASYNC(&music, MusicPlayBJ, ARG_UNUSED); break;
    case MSG_STOP_BJ: ASYNC(&music, MusicStopBJ, ARG_UNUSED); break;
    case MSG_MUTE_UNMUTE: SYNC(&music, MusicMuteUnmute, ARG_UNUSED); break;
    case MSG_VOLUME_UP: SYNC(&music, MusicIncreaseVolume, ARG_UNUSED); break;
    case MSG_VOLUME_DOWN: SYNC(&music, MusicDecreaseVolume, ARG_UNUSED); break;
    case MSG_NEW_KEY: SYNC(&music, MusicSetKey, pLoad.val); break;
    case MSG_NEW_TEMPO: SYNC(&music, MusicSetTempo, pLoad.val); break;
  }
}

void receiver(App *self, int unused) {
  CANMsg msg;
  CAN_RECEIVE(&can0, &msg);
  SCI_WRITE(&sci0, "Can msg received: ");
  CANPayload pLoad = unpackCANMsg(msg);
  CANPLoadAcknowledge(pLoad);
  if (!self->conductor) CANPLoadAct(pLoad);
}


void reader(App* self, int c) {
  int key;
  int tempo;
  switch (c) {
    case 'c': self->conductor = 1; SCI_WRITE(&sci0, "you have entered conductor mode\n"); break;
    case 'e': self->conductor = 0; SCI_WRITE(&sci0, "you have entered musician mode\n"); break;
    case 'p': CanSendMsg(MSG_START_BJ, ARG_UNUSED); break;
    case 's': CanSendMsg(MSG_STOP_BJ, ARG_UNUSED); break;
    case ',': CanSendMsg(MSG_VOLUME_UP, ARG_UNUSED); break;
    case '.': CanSendMsg(MSG_VOLUME_DOWN, ARG_UNUSED); break;
    case 'm': CanSendMsg(MSG_MUTE_UNMUTE, ARG_UNUSED); break;
    case '0': self->buf[self->len] = c; self->len++; return;
    case '1': self->buf[self->len] = c; self->len++; return;
    case '2': self->buf[self->len] = c; self->len++; return;
    case '3': self->buf[self->len] = c; self->len++; return;
    case '4': self->buf[self->len] = c; self->len++; return;
    case '5': self->buf[self->len] = c; self->len++; return;
    case '6': self->buf[self->len] = c; self->len++; return;
    case '7': self->buf[self->len] = c; self->len++; return;
    case '8': self->buf[self->len] = c; self->len++; return;
    case '9': self->buf[self->len] = c; self->len++; return;
    case '-': self->buf[self->len] = c; self->len++; return;
    case 'k':
      self->buf[self->len] = '\0';
      int key = atoi(self->buf);
      self->len = 0;
      if (key <= 5 && key >= -5) {
        CanSendMsg(MSG_NEW_KEY, key); 
        break;
      }
      else {
        SCI_WRITE(&sci0, "Input key not allowed! >:(\n");
        return;
      }

    case 't':
      self->buf[self->len] = '\0';
      int tempo = atoi(self->buf);
      self->len = 0;
      if (tempo <= 240 && tempo >= 60){
        CanSendMsg(MSG_NEW_TEMPO, tempo); 
        break;
      }
      else {
        SCI_WRITE(&sci0, "Input tempo not allowed! >:(\n");
        return;
      } 
  }

  if (self->conductor) {
    switch(c) {
      case 'p': SYNC(&music, MusicPlayBJ, ARG_UNUSED); break;
      case 's': SYNC(&music, MusicStopBJ, ARG_UNUSED); break;
      case ',': SYNC(&music, MusicIncreaseVolume, ARG_UNUSED); break;
      case '.': SYNC(&music, MusicDecreaseVolume, ARG_UNUSED); break;
      case 'm': SYNC(&music, MusicMuteUnmute, ARG_UNUSED); break;
      case 'k': SYNC(&music, MusicSetKey, key); break;
      case 't': SYNC(&music, MusicSetTempo, tempo); break;
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
}

int main() {
  INSTALL(&sci0, sci_interrupt, SCI_IRQ0);
  INSTALL(&can0, can_interrupt, CAN_IRQ0);
  TINYTIMBER(&app, startApp, 0);
  return 0;
}
