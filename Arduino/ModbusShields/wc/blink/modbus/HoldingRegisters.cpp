namespace Modbus{ namespace HoldingRegisters{
	using namespace Modbus;
	using namespace ModbusClientUtil;

	enum REGISTERS{
		WC_FLUSH_HALF_DELAY, //Время неполного смыва
		WC_FLUSH_FULL_DELAY, //Время полного смыва
        WC_FLUSH_HALF_TIME, //Время нахождения для неполного смыва
        WC_FLUSH_FULL_TIME, //Время нахождения для полного смыва
        //WC_IR_EVENT_TIMEOUT, //Пауза перед авто смывом
        WC_IR_MODE,
        WC_SOUND_MODE,
		FLOWERS_IRRIGATE, //Время автополива
		FLOWERS_LIGHT_BRIGHTNESS,
		FLOWERS_SOIL_HUMIDITY,
        //
        REGISTERS_SIZE
	};

	uint16_t getFlushHalfDelay() { return msToUint16(WC::FLUSH_HALF_DELAY); }

	uint16_t getFlushFullDelay() { return msToUint16(WC::FLUSH_FULL_DELAY); }

	//uint16_t getIrEventTimeout() { return msToUint16(IR::EVENT_TIMEOUT); }

	uint16_t getFlushHalfTime() { return secToUint16(WC::FLUSH_HALF_TIME); }

	uint16_t getFlushFullTime() { return secToUint16(WC::FLUSH_FULL_TIME); }

	uint16_t getIrMode() { return WC::IR_MODE; }

	uint16_t getSoundMode() { return WC::SOUND_MODE; }

	uint16_t getFlowersLightBrightness() { return Flowers::LIGHT_BRIGHTNESS; }

	void setFlushHalfDelay(uint16_t value) { WC::FLUSH_HALF_DELAY = msFromUint16(value); }

	void setFlushFullDelay(uint16_t value) { WC::FLUSH_FULL_DELAY = msFromUint16(value); }

	void setFlushHalfTime(uint16_t value) { WC::FLUSH_HALF_TIME = secFromUint16(value); }

	void setFlushFullTime(uint16_t value) { WC::FLUSH_FULL_TIME = secFromUint16(value); }

	//void setIrEventTimeout(uint16_t value) { IR::EVENT_TIMEOUT = msFromUint16(value); }

	void setIrMode(uint16_t value) { WC::IR_MODE = value; }

	void setSoundMode(uint16_t value) { WC::SOUND_MODE = value; }

	void setup() {
		modbusListener.registerHoldingRegisterRead(WC_FLUSH_HALF_DELAY, getFlushFullTime);
		modbusListener.registerHoldingRegisterRead(WC_FLUSH_HALF_DELAY, getFlushHalfDelay);
		modbusListener.registerHoldingRegisterRead(WC_FLUSH_FULL_DELAY, getFlushFullDelay);
		modbusListener.registerHoldingRegisterRead(WC_FLUSH_HALF_TIME, getFlushHalfTime);
		modbusListener.registerHoldingRegisterRead(WC_FLUSH_FULL_TIME, getFlushFullTime);
		//modbusListener.registerHoldingRegisterRead(WC_IR_EVENT_TIMEOUT, getIrEventTimeout);
		modbusListener.registerHoldingRegisterRead(WC_IR_MODE, getIrMode);
		modbusListener.registerHoldingRegisterRead(WC_SOUND_MODE, getSoundMode);
		modbusListener.registerHoldingRegisterRead(FLOWERS_LIGHT_BRIGHTNESS, getFlowersLightBrightness);

		modbusListener.registerHoldingRegisterWrite(WC_FLUSH_HALF_DELAY, setFlushFullTime);
		modbusListener.registerHoldingRegisterWrite(WC_FLUSH_HALF_DELAY, setFlushHalfDelay);
		modbusListener.registerHoldingRegisterWrite(WC_FLUSH_FULL_DELAY, setFlushFullDelay);
		modbusListener.registerHoldingRegisterWrite(WC_FLUSH_HALF_TIME, setFlushHalfTime);
		modbusListener.registerHoldingRegisterWrite(WC_FLUSH_FULL_TIME, setFlushFullTime);
		//modbusListener.registerHoldingRegisterWrite(WC_IR_EVENT_TIMEOUT, setIrEventTimeout);
		modbusListener.registerHoldingRegisterWrite(WC_IR_MODE, setIrMode);
		modbusListener.registerHoldingRegisterWrite(WC_SOUND_MODE, setSoundMode);
		modbusListener.registerHoldingRegisterWrite(FLOWERS_IRRIGATE, Flowers::irrigate);
		modbusListener.registerHoldingRegisterWrite(FLOWERS_LIGHT_BRIGHTNESS, Flowers::setBrightness);

		modbusListener.registerInputRegisterRead(FLOWERS_SOIL_HUMIDITY, Flowers::getSoilHumidity);
	}
} }