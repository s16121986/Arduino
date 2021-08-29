#define DEBUG_PORT

#define SLAVE_ID       72
#define PIN_TX          2

#define PIN_COLD_POWER  4
#define PIN_COLD_CTRL   5
#define PIN_HOT_CTRL    6
#define PIN_HOT_POWER   7

unsigned int CURRENT_TIME;
unsigned long int GIDROLOCK_OPEN_DELAY = 1500; //Время поворота шарового крана на 90 градусов 15 sec

//#include <stdint.h> //value types
#include "lib/Gidrolock.cpp"

Gidrolock motorCold(PIN_COLD_POWER, PIN_COLD_CTRL);
Gidrolock motorHot(PIN_HOT_POWER, PIN_HOT_CTRL);

#include "lib/Controller.cpp"

#include <Modbus.h>
#include "lib/Master.cpp"
#include <MsTimer2.h>

void listen() {
	CURRENT_TIME = millis();
	Controller::listen();
}

void setup() {
	CURRENT_TIME = millis();

	pinMode(PIN_COLD_POWER, OUTPUT);
	pinMode(PIN_COLD_CTRL, OUTPUT);
	pinMode(PIN_HOT_POWER, OUTPUT);
	pinMode(PIN_HOT_CTRL, OUTPUT);

	Modbus::setup(SLAVE_ID, PIN_TX);
	Modbus::begin(9600);
	//Serial.begin(9600);
	while(!Serial);
#ifdef DEBUG_PORT
	Serial.println("//setup");
#endif

	delay(100);

	MsTimer2::set(40, listen);
	MsTimer2::start();

	Master::setup();
}

void loop() {
	Master::loop();
	//Controller::loop();
}
