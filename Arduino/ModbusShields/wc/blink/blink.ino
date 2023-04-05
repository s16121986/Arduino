//#define DEBUG_PORT
//#define MODBUS_DEBUG

//#include <Modbus.h>
#include <MsTimer2.h>

#define SLAVE_ID           75
#define PIN_TX              2

#define PIN_WC_SWITCH       3
#define PIN_WC_IR           4
#define PIN_WC_SERVO        5
#define PIN_WC_SOUND        6
#define PIN_FL_LIGHT        9 //PWM
#define PIN_FL_IRRIGATION  10
//#define PIN_FL_SOIL       10
//#define PIN_HUMIDITY       8
#define PIN_TEST        8

unsigned long int CURRENT_TIME;

#include "./Controller/WC.cpp"
#include "./Controller/Flowers.cpp"
//#include "./Modbus/Modbus.cpp"

void listen() {
	CURRENT_TIME = millis();
	WC::listen();
	//Flowers::listen();
}

void setup() {
    pinMode(PIN_WC_IR, INPUT);
    //pinMode(PIN_TEST, OUTPUT);
    pinMode(PIN_WC_SERVO, OUTPUT);
    pinMode(PIN_WC_SOUND, OUTPUT);
    pinMode(PIN_FL_LIGHT, OUTPUT);
    pinMode(PIN_FL_IRRIGATION, OUTPUT);

	//Modbus::setup();
	//Serial.begin(9600);

	while(!Serial);
#ifdef DEBUG_PORT
	Serial.println("//setup");
#endif
	delay(100);
	CURRENT_TIME = millis();
	MsTimer2::set(40, listen);

	delay(100);
	MsTimer2::start();/**/
}

void loop() {
	//Modbus::loop();
	WC::loop();
	//Flowers::loop();
}
