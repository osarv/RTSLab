#include "App.h"
#include "Tone.h"
#include "Load.h"
#include "canTinyTimber.h"
#include "sciTinyTimber.h"

App app = initApp();
Load load = initLoad();
Tone tone = initTone();
Can can0 = initCan(CAN_PORT0, &app, receiver);
Serial sci0 = initSerial(SCI_PORT0, &app, reader);
