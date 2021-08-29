namespace HoldingRegisters{
	enum REGISTERS{
		FLUSH_HALF_DELAY, //Время неполного смыва
		FLUSH_FULL_DELAY, //Время полного смыва
        FLUSH_HALF_TIME, //Время нахождения для неполного смыва
        FLUSH_FULL_TIME, //Время нахождения для полного смыва
        IR_EVENT_TIMEOUT, //Пауза перед авто смывом
        IR_MODE,
        SOUND_MODE,
        //
        REGISTERS_SIZE
	};

	uint16_t registersValues[REGISTERS_SIZE] = {
		1000, //FLUSH_HALF_DELAY
    	2500, //FLUSH_FULL_DELAY
    	20000, //FLUSH_HALF_TIME
    	60000, //FLUSH_FULL_TIME
    	500, //IR_EVENT_TIMEOUT
		1, //IR_MODE
		0 //SOUND_MODE
	};

	uint8_t count() { return REGISTERS_SIZE; }
	uint16_t get(uint16_t address) { return registersValues[address]; }
	void set(uint16_t address, uint16_t value) {
		registersValues[address] = value;
	}
	bool has(uint16_t address) { return address < REGISTERS_SIZE; }

	uint16_t msFromUint16(uint16_t value) { return value * 10; }
	uint16_t secFromUint16(uint16_t value) { return value * 1000; }
	uint16_t msToUint16(uint16_t value) { return value / 10; }
	uint16_t secToUint16(uint16_t value) { return value / 1000; }
	uint16_t getVariable(uint16_t address) {
		uint16_t value = get(address);
		switch (address) {
            case FLUSH_HALF_DELAY:
            case FLUSH_FULL_DELAY:
            case IR_EVENT_TIMEOUT:
                return msToUint16(value);
            case FLUSH_HALF_TIME:
            case FLUSH_FULL_TIME:
                return secToUint16(value);
            default:
                return value;
        }
	}
	void setVariable(uint16_t address, uint16_t value) {
		uint16_t setValue;
		switch (address) {
            case FLUSH_HALF_DELAY:
            case FLUSH_FULL_DELAY:
            case IR_EVENT_TIMEOUT:
                setValue = msFromUint16(value);
                break;
            case FLUSH_HALF_TIME:
            case FLUSH_FULL_TIME:
                setValue = secFromUint16(value);
                break;
            default:
                setValue = value;
        }
		set(address, setValue);
	}

}