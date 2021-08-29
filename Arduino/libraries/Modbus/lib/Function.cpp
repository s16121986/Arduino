namespace Modbus{

	void bindReadFunction(uint16_t (*f)(uint16_t reqAdd), uint8_t count, uint8_t readIndex) {
		registeredFunctions[readIndex] = true;
        readFunctions[readIndex] = f;
        registersCount[readIndex] = count;
	}
	
	void bindCoilRead(uint16_t (*f)(uint16_t reqAdd), uint8_t count) {
		bindReadFunction(f, count, READ_COIL);
	}
	
	void bindDiscreteInputRead(uint16_t (*f)(uint16_t reqAdd), uint8_t count) {
		bindReadFunction(f, count, READ_DISCRETE_INPUT);
	}
	
	void bindHoldingRegisterRead(uint16_t (*f)(uint16_t reqAdd), uint8_t count) {
		bindReadFunction(f, count, READ_HOLDING_REGISTER);
	}
	
	void bindInputRegisterRead(uint16_t (*f)(uint16_t reqAdd), uint8_t count) {
		bindReadFunction(f, count, READ_INPUT_REGISTER);
	}
	
	void bindCoilWrite(void (*f)(uint16_t reqAdd, bool flag), uint8_t count) {
#ifdef MODBUS_DEBUG
		Serial.println("registeredFunctions.WRITE_COIL=true");
#endif
		registeredFunctions[WRITE_COIL] = true;
        writeCoil = f;
        registersCount[WRITE_COIL] = count;
	}
	
	void bindRegisterWrite(void (*f)(uint16_t reqAdd, uint16_t value), uint8_t count) {
		registeredFunctions[WRITE_REGISTER] = true;
        writeRegister = f;
        registersCount[WRITE_REGISTER] = count;
	}

	bool checkFunction(uint16_t reqFunc) {
		switch (reqFunc) {
            case FUNCTION_READ_COILS:
                return registeredFunctions[READ_COIL];
            case FUNCTION_READ_DISCRETE_INPUTS:
                return registeredFunctions[READ_DISCRETE_INPUT];
            case FUNCTION_READ_HOLDING_REGISTERS:
                return registeredFunctions[READ_HOLDING_REGISTER];
            case FUNCTION_READ_INPUT_REGISTERS:
                return registeredFunctions[READ_INPUT_REGISTER];
            case FUNCTION_WRITE_SINGLE_COIL:
                return registeredFunctions[WRITE_COIL];
            case FUNCTION_WRITE_MULTIPLE_COILS:
                return registeredFunctions[WRITE_COIL];
            case FUNCTION_WRITE_SINGLE_REGISTER:
                return registeredFunctions[WRITE_REGISTER];
            case FUNCTION_WRITE_MULTIPLE_REGISTER:
                return registeredFunctions[WRITE_REGISTER];
        }
        return false;
	}
	
	bool checkAddress(uint8_t startingAddress, uint8_t noOfCoils, uint8_t sizeIndex) {
		if (startingAddress + noOfCoils <= registersCount[sizeIndex])
			return true;
#ifdef MODBUS_DEBUG
		Serial.println("checkAddress failed");
#endif
    	sendException(ILLEGAL_DATA_ADDRESS);
    	return false;
	}

	void readCoils() {
		uint8_t startingAddress = req.getAddress();
		uint8_t noOfCoils = req.getCoilsNumber();
		if (!checkAddress(startingAddress, noOfCoils, READ_COIL))
			return;

		uint8_t pos = 0;
		bool data[noOfCoils];
        for (uint8_t i = 0; i < noOfCoils; i++) {
            data[pos++] = (*readFunctions[READ_COIL])(startingAddress + i);
        }

        response.processFC1(data, noOfCoils);
	}
	
	void readDiscreteInputs() {
		uint8_t startingAddress = req.getAddress();
		uint8_t noOfCoils = req.getCoilsNumber();
		if (!checkAddress(startingAddress, noOfCoils, READ_DISCRETE_INPUT))
			return;

		uint8_t pos = 0;
		bool data[noOfCoils];
        for (uint8_t i = 0; i < noOfCoils; i++) {
            data[pos++] = (*readFunctions[READ_DISCRETE_INPUT])(startingAddress + i);
        }

        response.processFC2(data, noOfCoils);
	}
    
	void readHoldingRegisters() {
        uint8_t startingAddress = req.getAddress();
        uint8_t noOfCoils = req.getCoilsNumber();
		if (!checkAddress(startingAddress, noOfCoils, READ_HOLDING_REGISTER))
			return;

        uint8_t pos = 0;
        uint16_t data[noOfCoils];
        for (uint8_t i = 0; i < noOfCoils; i++) {
            data[pos++] = (*readFunctions[READ_HOLDING_REGISTER])(startingAddress + i);
        }

        response.processFC3(data, noOfCoils);
    }
	
	void readInputRegisters() {
		uint8_t startingAddress = req.getAddress();
        uint8_t noOfCoils = req.getCoilsNumber();
        if (!checkAddress(startingAddress, noOfCoils, READ_INPUT_REGISTER))
            return;

        uint8_t pos = 0;
        uint16_t data[noOfCoils];
        for (uint8_t i = 0; i < noOfCoils; i++) {
            data[pos++] = (*readFunctions[READ_INPUT_REGISTER])(startingAddress + i);
        }

        response.processFC3(data, noOfCoils);
	}
	
	void writeSingleCoil() {
		uint8_t startingAddress = req.getAddress();
#ifdef MODBUS_DEBUG
		Serial.print("writeSingleCoil(");
		Serial.print(startingAddress);
		Serial.println(")");
#endif
        if (!checkAddress(startingAddress, 1, WRITE_COIL))
            return;
		(*writeCoil)(startingAddress, req.getValue() != 0);
        //response.processFC5();
    }
    
	void writeSingleRegister() {
		uint8_t startingAddress = req.getAddress();
        if (!checkAddress(startingAddress, 1, WRITE_REGISTER))
            return;
		(*writeRegister)(startingAddress, req.getValue());
        //response.processFC6();
    }
	
	void writeMultipleCoils() {
		uint16_t startingAddress = req.getAddress();
        uint16_t noOfCoils = req.getCoilsNumber();
        if (!checkAddress(startingAddress, noOfCoils, WRITE_COIL))
            return;

		uint8_t n = 0;
        uint16_t noOfBytes = req.getBytesNumber();
		for (uint8_t i = 1; i <= noOfBytes; i++) {
			uint8_t b = req.getData(i);
			for (uint8_t j = 0; j < 4; j++) {
				(*writeCoil)(startingAddress + n, bitRead(b, j) != 0);
				if (++n == noOfCoils)
					break;
			}
		}

        response.processFC15(startingAddress, noOfCoils);
	}
	
	void writeMultipleRegister() {
		uint16_t startingAddress = req.getAddress();
        uint16_t noOfCoils = req.getCoilsNumber();
        if (!checkAddress(startingAddress, noOfCoils, WRITE_REGISTER))
            return;

		uint8_t n = 1;
        uint16_t noOfBytes = req.getBytesNumber();
/*
	Serial.println("--writeMultipleRegister--");
	Serial.print("noOfCoils=");
	Serial.println(noOfCoils);
	Serial.print("noOfBytes=");
	Serial.println(noOfBytes);
*/
		for (uint8_t i = 0; i < noOfCoils; i++) {
			uint16_t b = toUint16(req.getData(n), req.getData(n + 1));
/*
	Serial.print("write[");
	Serial.print(startingAddress + i);
	Serial.print("]=");
	Serial.print(req.getData(n));
	Serial.print(" ");
	Serial.println(req.getData(n + 1));
*/
			(*writeRegister)(startingAddress + i, b);
			n += 2;
		}
	Serial.println("--");

        response.processFC15(startingAddress, noOfCoils);
	}
}