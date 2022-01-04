using namespace ModbusClientUtil;

ModbusListener::ModbusListener(ModbusClient* _modbus) {
	modbus = _modbus;
}

void ModbusListener::registerCoilRead(uint16_t reqAdd, rfunc f) { readCoilListener.bind(reqAdd, f); }

void ModbusListener::registerDiscreteInputRead(uint16_t reqAdd, rfunc f) { readDiscreteInputListener.bind(reqAdd, f); }

void ModbusListener::registerHoldingRegisterRead(uint16_t reqAdd, rfunc f) { readHoldingRegisterListener.bind(reqAdd, f); }

void ModbusListener::registerInputRegisterRead(uint16_t reqAdd, rfunc f) { readInputRegisterListener.bind(reqAdd, f); }

void ModbusListener::registerCoilWrite(uint16_t reqAdd, void (*f)(bool flag)) { writeCoilListener.bind(reqAdd, f); }

void ModbusListener::registerHoldingRegisterWrite(uint16_t reqAdd, void (*f)(uint16_t value)) { writeRegisterListener.bind(reqAdd, f); }

void ModbusListener::readCoils() {
	uint8_t startingAddress = modbus->getAddress();
	uint8_t noOfCoils = modbus->getCoilsNumber();

	if (!readCoilListener.find(startingAddress, noOfCoils))
		return;

	uint8_t pos = 0;
	bool data[noOfCoils];
	for (uint8_t i = 0; i < noOfCoils; i++) {
		data[pos++] = readCoilListener.handle(startingAddress + i);
	}

	modbus->sendFC1(data, noOfCoils);
}

void ModbusListener::readDiscreteInputs() {
	uint8_t startingAddress = modbus->getAddress();
	uint8_t noOfCoils = modbus->getCoilsNumber();

	if (!readDiscreteInputListener.find(startingAddress, noOfCoils))
		return;

	uint8_t pos = 0;
	bool data[noOfCoils];
	for (uint8_t i = 0; i < noOfCoils; i++) {
		data[pos++] = readCoilListener.handle(startingAddress + i);
	}

	modbus->sendFC2(data, noOfCoils);
}

void ModbusListener::readHoldingRegisters() {
	uint8_t startingAddress = modbus->getAddress();
	uint8_t noOfCoils = modbus->getCoilsNumber();

	if (!readHoldingRegisterListener.find(startingAddress, noOfCoils))
		return;

	uint8_t pos = 0;
	uint16_t data[noOfCoils];
	for (uint8_t i = 0; i < noOfCoils; i++) {
		data[pos++] = readHoldingRegisterListener.handle(startingAddress + i);
	}

	modbus->sendFC3(data, noOfCoils);
}

void ModbusListener::readInputRegisters() {
	uint8_t startingAddress = modbus->getAddress();
	uint8_t noOfCoils = modbus->getCoilsNumber();

	if (!readInputRegisterListener.find(startingAddress, noOfCoils))
		return;

	uint8_t pos = 0;
	uint16_t data[noOfCoils];
	for (uint8_t i = 0; i < noOfCoils; i++) {
		data[pos++] = readInputRegisterListener.handle(startingAddress + i);
	}

	modbus->sendFC3(data, noOfCoils);
}

void ModbusListener::writeSingleCoil() {
	uint8_t startingAddress = modbus->getAddress();
#ifdef MODBUS_DEBUG
	Serial.print("writeSingleCoil(");
	Serial.print(startingAddress);
	Serial.println(")");
#endif
	if (!writeCoilListener.find(startingAddress))
		return;

	writeCoilListener.handle(startingAddress, modbus->getCoilsNumber() != 0);
	modbus->sendFC5(modbus->getAddress(), modbus->getCoilsNumber());
}

void ModbusListener::writeSingleRegister() {
	uint8_t startingAddress = modbus->getAddress();
	if (!writeRegisterListener.find(startingAddress))
		return;

	writeRegisterListener.handle(startingAddress, modbus->getCoilsNumber());
	modbus->sendFC5(modbus->getAddress(), modbus->getCoilsNumber());
}

void ModbusListener::writeMultipleCoils() {
	uint16_t startingAddress = modbus->getAddress();
	uint16_t noOfCoils = modbus->getCoilsNumber();
	if (!writeCoilListener.find(startingAddress, noOfCoils))
		return;

	uint8_t n = 0;
	uint16_t noOfBytes = modbus->getBytesNumber();
	for (uint8_t i = 1; i <= noOfBytes; i++) {
		uint8_t b = modbus->getByte(i);
		for (uint8_t j = 0; j < 4; j++) {
			writeCoilListener.handle(startingAddress + n, bitRead(b, j) != 0);
			if (++n == noOfCoils)
				break;
		}
	}

	modbus->sendFC15(startingAddress, noOfCoils);
}

void ModbusListener::writeMultipleRegister() {
	uint16_t startingAddress = modbus->getAddress();
	uint16_t noOfCoils = modbus->getCoilsNumber();
	if (!writeRegisterListener.find(startingAddress, noOfCoils))
		return;

	uint8_t n = 1;
	uint16_t noOfBytes = modbus->getBytesNumber();
	for (uint8_t i = 0; i < noOfCoils; i++) {
		uint16_t b = toUint16(modbus->getByte(n), modbus->getByte(n + 1));
		writeRegisterListener.handle(startingAddress + i, b);
		n += 2;
	}

	modbus->sendFC15(startingAddress, noOfCoils);
}

void ModbusListener::run() {
	if (!modbus->available())
		return;

	switch (modbus->getFunction()) {
	case FUNCTION_READ_COILS:
		readCoils();
		break;
	case FUNCTION_READ_DISCRETE_INPUTS:
		readDiscreteInputs();
		break;
	case FUNCTION_READ_HOLDING_REGISTERS:
		readHoldingRegisters();
		break;
	case FUNCTION_READ_INPUT_REGISTERS:
		readHoldingRegisters();
		break;
	case FUNCTION_WRITE_SINGLE_COIL:
		writeSingleCoil();
		break;
	case FUNCTION_WRITE_MULTIPLE_COILS:
		writeMultipleCoils();
		break;
	case FUNCTION_WRITE_SINGLE_REGISTER:
		writeSingleRegister();
		break;
	case FUNCTION_WRITE_MULTIPLE_REGISTER:
		writeMultipleRegister();
		break;
	default:
		modbus->sendException(ILLEGAL_FUNCTION);
	}

	if (!modbus->isEmpty())
		modbus->sendException(ILLEGAL_FUNCTION);
}