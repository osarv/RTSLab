#ifndef _APP_H
#define _APP_H

#include "TinyTimber.h"

#define BSIZE 100
typedef struct {
  Object super;
  int len;
  int conductor;
  char buf[BSIZE];
} App;

#define initApp()                                                              \
  { initObject(), 0, 0, {0} }

void reader(App *, int);
void receiver(App *, int);
void startApp(App *, int);

#endif