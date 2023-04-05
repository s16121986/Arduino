#define DEBUG_PORT
#define MODBUS_DEBUG

#include <SoftwareSerial.h>
//#include <AltSoftSerial.h>
#include <Modbus.h>

//AltSoftSerial RS485Serial;
SoftwareSerial RS485Serial(8, 9);
ModbusClient modbus;
ModbusListener modbusListener(modbus);

#define SLAVE_ID       75

void coilWrite(bool flag) {
	Serial.print("coilWrite: ");
	Serial.print(flag);
	Serial.println(";");
}

void setup() {
	//RS485Serial.begin(9600);
	modbus.setup(RS485Serial, SLAVE_ID);

	Serial.begin(9600);
	while (!Serial);
#ifdef DEBUG_PORT
	Serial.println("//setup");
#endif

	RS485Serial.begin(9600);
	modbus.begin(9600);

	modbusListener.registerCoilWrite(5, coilWrite);

	delay(100);
}

void loop() {
	modbusListener.run();
	//modbus.available();

	/*if (!altSerial.available())
		return;

	while (altSerial.available()) {
		Serial.print("[");
		Serial.print(altSerial.read());
		Serial.print("]");
		delayMicroseconds(1562);
	}
	Serial.println(";");*/
	//Modbus::loop();

	//Controller::loop();
}
