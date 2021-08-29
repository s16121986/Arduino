namespace Master{
	using namespace Modbus;

	enum HOLDING_REGISTERS{
		OPEN_DELAY,
        //
        HOLDING_REGISTERS_SIZE
	};

	uint16_t getHoldingValue(uint16_t regAdd) {
		switch (regAdd) {
			case OPEN_DELAY:
                return GIDROLOCK_OPEN_DELAY / 1000;
		}
		return 0;
	}
	uint16_t getMotorState(uint8_t address) {
		return toUint16(Controller::getMotorState(address));
	}
	void setMotorState(uint16_t address, bool state) {
		if (state)
			Controller::openMotor(address);
		else
			Controller::closeMotor(address);
	}
	void setHoldingValue(uint16_t address, uint16_t value) {
#ifdef DEBUG_PORT
	Serial.print("set holding register[");
	Serial.print(address);
	Serial.print("]: ");
	Serial.println(value);
#endif
		switch (address) {
			case OPEN_DELAY:
				GIDROLOCK_OPEN_DELAY = value * 1000;
#ifdef DEBUG_PORT
	Serial.print("OPEN_DELAY set to: ");
	Serial.println(GIDROLOCK_OPEN_DELAY);
#endif
				break;
		}
	}

	void setup() {
		bindCoilRead(getMotorState, Controller::MOTORS_SIZE);
		bindCoilWrite(setMotorState, Controller::MOTORS_SIZE);
		bindHoldingRegisterRead(getHoldingValue, HOLDING_REGISTERS_SIZE);
		bindRegisterWrite(setHoldingValue, HOLDING_REGISTERS_SIZE);
	}
}