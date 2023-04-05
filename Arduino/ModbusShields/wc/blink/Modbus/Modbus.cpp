namespace Modbus {
	ModbusClient modbus(SLAVE_ID, PIN_TX);
	ModbusListener modbusListener(&modbus);
}

#include "./HoldingRegisters.cpp"
#include "./Coils.cpp"

namespace Modbus {
	void setup() {
		modbus.begin(9600);

		delay(1000);

		HoldingRegisters::setup();
		Coils::setup();
	}

	void loop() {
		modbusListener.run();
	}
}