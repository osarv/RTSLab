#include "App.h"
#include "Tone.h"
#include "Load.h"
#include "TinyTimber.h"
#include "canTinyTimber.h"
#include "sciTinyTimber.h"
#include "stdlib.h"
#include "stdio.h"
#include "Music.h"

#define ARG_UNUSED 0
extern App app;
extern Load load;
extern Tone tone;
extern Music music;
extern Can can0;
extern Serial sci0;

void receiver(App *self, int unused) {
  CANMsg msg;
  CAN_RECEIVE(&can0, &msg);
  SCI_WRITE(&sci0, "Can msg received: ");
  SCI_WRITE(&sci0, msg.buff);
}

/*void printKeyAndPeriods(int key) {
  char buf[10];
  snprintf(buf, 10, "Key: %d\n", key);
  SCI_WRITE(&sci0, buf);
  for (int i = 0; i < sizeof(notes) / sizeof(int); i++) {
    snprintf(buf, 10, "%d ", periods[notes[i] + key + 10]);
    SCI_WRITE(&sci0, buf);
  }
  SCI_WRITECHAR(&sci0, '\n');
}

void printTempo(int tempo){
  char buf[10];
  snprintf(buf, 10, "Tempo: %d\n", tempo);
  SCI_WRITE(&sci0, buf);
  SCI_WRITECHAR(&sci0, '\n');
}*/

void reader(App* self, int c) {
  switch (c) {
    //case ',': SYNC(&load, LoadIncrease, ARG_UNUSED); break;
    //case '.': SYNC(&load, LoadDecrease, ARG_UNUSED); break;
    case '+': SYNC(&tone, ToneIncreaseAmplitude, ARG_UNUSED); break;
    case '-': SYNC(&tone, ToneDecreaseAmplitude, ARG_UNUSED); break;
    case 'm': SYNC(&tone, ToneToggle, ARG_UNUSED); break;
    /*case 'd':
      SYNC(&tone, ToneToggleDeadline, ARG_UNUSED);
      SYNC(&load, LoadToggleDeadline, ARG_UNUSED);
      break;
    */  
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
    case 'k':
      self->buf[self->len] = '\0';
      int key = atoi(self->buf);
      self->len = 0;
      if (key <= 5 || key >= -5) {
        ASYNC(&music, MusicSetKey, key);
        break;
      }
      else {
        SCI_WRITE(&sci0, "Input key not allowed");
        break;
      }
    case 't':
      self->buf[self->len] = '\0';
      int tempo = atoi(self->buf);
      self->len = 0;
      if (tempo <= 240 || tempo >= 60){
        ASYNC(&music, MusicSetTempo, tempo);
        break;
      }
      else {
        SCI_WRITE(&sci0, "Input tempo not allowed");
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

  SYNC(&tone, ToneSetPeriod, USEC(500));
  ASYNC(&tone, ToneGenerate, ARG_UNUSED);
  ASYNC(&load, LoadRun, ARG_UNUSED);
}

int main() {
  INSTALL(&sci0, sci_interrupt, SCI_IRQ0);
  INSTALL(&can0, can_interrupt, CAN_IRQ0);
  TINYTIMBER(&app, startApp, 0);
  return 0;
}
