//#include "Temperature.cpp"
//#include "Humidity.cpp"
//#include "Temperature.cpp"

//#include "Home.h"


namespace Climatic{
	using namespace Modbus;

	void setup() {
		//dht.begin();
		bme.begin();//0x76
	}
	/*void readSensors() {
		sensorsRegisters[TEMPERATURE] = bme.readTemperature();
		sensorsRegisters[HUMIDITY] = readHumidity();
		sensorsRegisters[CARBON_DIOXIDE] = analogRead(PIN_GAS);
		sensorsRegisters[PRESSURE] = bme.readPressure();
	}*/
	unsigned int readSensorRegister(unsigned char reg) {
		/*Serial.print("reg[");
		Serial.print(reg);
		Serial.print("]=");
		Serial.println(sensorsRegisters[reg]);*/
		//return sensorsRegisters[reg];
		switch (reg) {
			case TEMPERATURE:
				return bme.readTemperature();
			case HUMIDITY:
				return bme.readHumidity();
			case PRESSURE:
				return bme.readPressure();
			case CARBON_DIOXIDE:
				return analogRead(PIN_GAS);
			case DUST:
				return DustSensor::read();
			default:
				return 0;
		}/**/
	}
	void setup() {
		bindCoilRead(getMotorState, 1);
		bindCoilWrite(setMotorState, 1);
		bindHoldingRegisterRead(getHoldingValue, HOLDING_REGISTERS_SIZE);
		bindRegisterWrite(setHoldingValue, HOLDING_REGISTERS_SIZE);
	}
}