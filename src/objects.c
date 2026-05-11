#include "App.h"
#include "Tone.h"
#include "Tempo.h"
#include "canTinyTimber.h"
#include "sciTinyTimber.h"
#include "sioTinyTimber.h"
#include "Music.h"

App app = initApp();
Tone tone = initTone();
Tempo tempo = initTempo();
Music music = initMusic();
SysIO sio = initSysIO(SIO_PORT0, &tempo, buttonEvent);
Can can0 = initCan(CAN_PORT0, &app, receiver);
Serial sci0 = initSerial(SCI_PORT0, &app, reader);
