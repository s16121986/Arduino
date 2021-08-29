//#include <Wire.h>

#define DEBUG_PORT
#define MODBUS_DEBUG

//RS485 pins
#define SLAVE_ID       75
#define PIN_TX          2

#define PIN_SWITCH      4
#define PIN_IR          5
#define PIN_SERVO       6
#define PIN_SOUND       7
#define PIN_TEST        8

unsigned long int CURRENT_TIME;

//#include "include/PowerSafe.cpp"
//#include "lib/Config.cpp"
#include "modbus/Coils.cpp"
#include "modbus/HoldingRegisters.cpp"
#include <MsTimer2.h>
#include "lib/Button.cpp"
#ifdef PIN_IR
#include "lib/IR.cpp"
#endif
#include "lib/Servo.cpp"
#include "lib/Controller.cpp"
#ifdef SLAVE_ID
#include <Modbus.h>
#include "modbus/Master.cpp"
#endif
/*bool sleepFlag;

ISR(WDT_vect) {
	if (sleepFlag && Controller::isActive())
		wakeUp();
#ifdef DEBUG_PORT
	Serial.println("_WDT_vect");
#endif
	wdt_disable(); //прерывание сработало, отключаем таймер, после чего продолжается выполнение основной программы
}


void wakeUp() {
#ifdef DEBUG_PORT
	Serial.println("//wakeUp");
#endif
	PowerSafe::wakeUp();
	sleepFlag = false;
	MsTimer2::start();
}
void sleepMode() {
#ifdef DEBUG_PORT
	Serial.println("//sleep");
#endif
	sleepFlag = true;
	MsTimer2::stop();
	Controller::activity(false);
}*/

void listen() {
	CURRENT_TIME = millis();
	//Controller::activity(true);
	//if (Servo::disabled())
	//	return;
	Controller::listen();
}

void setup() {
	pinMode(PIN_SWITCH, INPUT);
	pinMode(PIN_IR, INPUT);
	pinMode(PIN_TEST, OUTPUT);
	pinMode(PIN_SERVO, OUTPUT);
	pinMode(PIN_SOUND, OUTPUT);

#ifdef ModbusLib_h
	Modbus::setup(SLAVE_ID, PIN_TX);
	Modbus::begin(9600);
	//Master::setup();
#else
	Serial.begin(9600);
#endif
	while(!Serial);
#ifdef DEBUG_PORT
	Serial.println("//setup");
#endif
	delay(100);

	CURRENT_TIME = millis();
	MsTimer2::set(40, listen);

	delay(100);
#ifdef POWER_SAFE_MODE
	sleepFlag = true;
	PowerSafe::setup(1);
#else
	MsTimer2::start();
#endif
}

void loop() {
#ifdef ModbusLib_h
	Master::loop();
#endif
	/*while (sleepFlag) {
#ifdef DEBUG_PORT
		Serial.println("//sleep waiting");
#endif
		PowerSafe::sleep();
	}

	if (Controller::isInactive())
		sleepMode();
	else*/
		Controller::loop();
}
