//#define DEBUG_MODE
//#include <Wire.h>

#include <Wire.h>                                       // Подключаем библиотеку Wire
#include <Adafruit_Sensor.h>                            // Подключаем библиотеку Adafruit_Sensor
#include <Adafruit_BME280.h>                            // Подключаем библиотеку Adafruit_BME280

//#define SEALEVELPRESSURE_HPA (1013.25)                  // Задаем высоту

//#define DEBUG_PORT
#define SLAVE_ID          71
#define PIN_TX              2
#define PIN_GAS            A0
#define PIN_DUST_LED       A0
#define PIN_DUST_MEASURE   A0
//#define PIN_TEST  3

//#include <ArduinoModbus.h>
//#include <ModbusRtu.h>
//#include <Modbus.h>
//#include "climatic/PowerSafe.cpp"
#include <Stdlib.h>
//#include "climatic/Climatic.h"
#include "climatic/Controller.cpp"

//#include <MsTimer2.h>
//#include <DHT.h>
//DHT tempIn(PIN_TEMP_IN, DHT11);

ISR(WDT_vect) {

	wdt_disable(); //прерывание сработало, отключаем таймер, после чего продолжается выполнение основной программы
}

void setup() {
  Serial.begin(9600);
  while(!Serial);
#ifdef DEBUG_PORT
	Serial.println("//setup");
#endif

	PowerSafe::setup(1);

	//Modbus::setup(SLAVE_ID, PIN_TX);
	//Modbus::begin(9600);

	Controller::setup();

	pinMode(PIN_DUST_LED, OUTPUT);
}

void loop() {
	while (!Modbus::available()) {
		PowerSafe::sleep();
	}

	Controller::run();
	//Climatic::listen();
	//delay(1);
}
