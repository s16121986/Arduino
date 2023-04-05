//#define DEBUG_MODE
//#define DEBUG_PORT

#define IO_START_BIT              62 // ">"
#define IO_STOP_BIT              255

#define EVENT_HIGH                 1
#define EVENT_LOW                  2
#define EVENT_HOLD                 3

#define DEVICE_ID                 10
#define START_PIN                 22
#define SENSORS_COUNT             16

//#include <EEPROM.h>
#include <MsTimer2.h>
#include "./lib/Sensor.cpp"

//unsigned long int CURRENT_TIME;
Sensor sensors[SENSORS_COUNT];

void send(uint8_t pin, uint8_t event) {
#ifdef DEBUG_PORT
	Serial.print("send: ");
	Serial.print(pin);
	Serial.print(" ");
	Serial.print(event);
	Serial.println(";");
#else
	Serial.write(IO_START_BIT);
	Serial.write(DEVICE_ID);
	Serial.write(pin);
	Serial.write(event);
	Serial.write(IO_STOP_BIT);
#endif
}

void listen() { 
	//CURRENT_TIME = millis();

	for (uint8_t i = 0; i < SENSORS_COUNT; i++) {
		sensors[i].listen();
	}
}

void setup() {
	Serial.begin(9600);
	while(!Serial);

	for (uint8_t i = 0; i < SENSORS_COUNT; i++) {
        sensors[i] = Sensor(i + START_PIN);
    }

	//CURRENT_TIME = millis();
	MsTimer2::set(40, listen);
	MsTimer2::start();
	delay(1000);

#ifdef DEBUG_PORT
	Serial.println("//started");
#endif
}

void loop() {
	for (uint8_t i = 0; i < SENSORS_COUNT; i++) {
		if (!sensors[i].isChanged())
			continue;

		if (sensors[i].isHold())
			send(sensors[i].pin, EVENT_HOLD);
		else if (sensors[i].isHigh())
			send(sensors[i].pin, EVENT_HIGH);
		else if (sensors[i].isLow())
			send(sensors[i].pin, EVENT_LOW);
	}
}
