//#define DEBUG_MODE
//#define DEBUG_PORT

#include <Home.h>
#include "config/constants.hpp"

//#include "pins_uno.hpp"

//#include "config/Config.cpp"
#include "Device/Device.cpp"

void listen() { 
	CURRENT_TIME = millis();

#ifdef Device_h
	Device::listen();
#endif
}

void setup() {
	SerialPort::setup();
	CURRENT_TIME = millis();

	MsTimer2::set(40, listen);
	MsTimer2::start();
	delay(1000);
	Device::setup();

#ifdef DEBUG_PORT
	Serial.println("//started");
#endif
}
void loop() {

#ifdef Device_h
	Device::loop();
#endif

	if (!SerialPort::isAvailable())
		return;

	uint8_t buffer[4];

#ifdef DEBUG_PORT
	SerialDebug::read(buffer);
#else
	SerialPort::read(buffer);
#endif

#ifdef Device_h
#ifdef DEBUG_PORT
		Serial.print("Device buffer: ");
		Serial.print(buffer[0]);
		Serial.print(" ");
		Serial.print(buffer[1]);
		Serial.print(" ");
		Serial.print(buffer[2]);
		Serial.print(" ");
		Serial.print(buffer[3]);
		Serial.println(";");
#endif
	switch (buffer[0]) {
		case IO_TRIGGER_BIT:
			Device::trigger(buffer[1], buffer[2], buffer[3]);
			break;
		case IO_PING_BIT:
			Device::send(IO_PING_BIT, 0, 0, 0);
#ifdef DEBUG_PORT
			Serial.println("pong!");
#endif
			break;
		case IO_DEBUG_BIT:
			Device::debug(buffer[1], buffer[2]);
			break;
		//case IO_CONFIG_BIT:
		//	Config::trigger(buffer[1], buffer[2]);
		//	break;
	}
#endif

	SerialPort::clear();
}
