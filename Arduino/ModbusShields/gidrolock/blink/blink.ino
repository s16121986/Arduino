#define DEBUG_PORT
#define MODBUS_DEBUG

#include <Modbus.h>
#include <MsTimer2.h>

#define SLAVE_ID       72
#define PIN_TX          2

#define PIN_BT_LEAK     3 //Датчик протечки ванная
#define PIN_KH_LEAK     4 //Датчик протечки кухня
#define PIN_COLD_POWER  5
#define PIN_COLD_CTRL   6
#define PIN_HOT_CTRL    7
#define PIN_HOT_POWER   8

unsigned int CURRENT_TIME;

#include "./Controller/Controller.cpp"
#include "./Modbus/Modbus.cpp"

void listen() {
	CURRENT_TIME = millis();
	Controller::listen();
}

void setup() {
	CURRENT_TIME = millis();

	pinMode(PIN_BT_LEAK, INPUT);
	pinMode(PIN_KH_LEAK, INPUT);

	Modbus::setup();
	//Serial.begin(9600);
	while (!Serial);
#ifdef DEBUG_PORT
	Serial.println("//setup");
#endif

	delay(100);

	MsTimer2::set(40, listen);
	MsTimer2::start();
}

void loop() {
	Modbus::loop();

	//Controller::loop();
}
