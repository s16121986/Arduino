#ifndef HomeLib_h
#define HomeLib_h

#define PWM_MAX_VALUE 254
//#define PWM_DELAY 500

unsigned long int CURRENT_TIME;

#include <EEPROM.h>
#include <MsTimer2.h>
#include <DHT.h>
#include "lib/init.h"
#include "lib/functions.cpp"
#include "lib/Timeout.cpp"
#include "lib/PinA.cpp"
#include "lib/PinD.cpp"
#include "lib/Memory.cpp"
#include "lib/serial/Port.cpp"
#include "lib/serial/Debug.cpp"

#endif