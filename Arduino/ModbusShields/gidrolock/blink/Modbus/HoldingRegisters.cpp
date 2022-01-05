namespace Modbus{ namespace HoldingRegisters{
	using namespace Modbus;
	using namespace ModbusClientUtil;

	enum REGISTERS{
		GIDROLOCK_OPEN_DELAY,
        //
        REGISTERS_SIZE
	};

	uint16_t getGidrolockOpenDelay() { return msToUint16(::GIDROLOCK_OPEN_DELAY); }

	void setGidrolockOpenDelay(uint16_t value) { ::GIDROLOCK_OPEN_DELAY = msFromUint16(value); }

	void setup() {
		modbusListener.registerHoldingRegisterRead(GIDROLOCK_OPEN_DELAY, getGidrolockOpenDelay);
		modbusListener.registerHoldingRegisterWrite(GIDROLOCK_OPEN_DELAY, setGidrolockOpenDelay);
	}
} }