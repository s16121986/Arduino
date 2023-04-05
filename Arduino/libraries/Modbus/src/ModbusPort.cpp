using namespace ModbusClientUtil;

AltSoftSerial altSerial;

ModbusClient::ModbusClient() {
	flush();
}

ModbusClient::ModbusClient(uint8_t _slaveID, uint8_t _TxEnablePin, uint8_t u8serno) {
	ModbusClient();
	setup(_slaveID, _TxEnablePin, u8serno);
}

ModbusClient::ModbusClient(uint8_t _slaveID, uint8_t _TxEnablePin) {
	ModbusClient();
	setup(_slaveID, _TxEnablePin, 0);
}

void ModbusClient::setup(uint8_t _slaveID, uint8_t _TxEnablePin, uint8_t u8serno) {
	switch (u8serno) {
#if defined(UBRR1H)
	case 1:
		port = &Serial1;
		break;
#endif

#if defined(UBRR2H)
	case 2:
		port = &Serial2;
		break;
#endif

#if defined(UBRR3H)
	case 3:
		port = &Serial3;
		break;
#endif
	case 0:
	default:
		port = &Serial;
		break;
	}

	slaveID = _slaveID;
	TxEnablePin = _TxEnablePin;
	//Request req();
	errorCount = 0;

	pinMode(TxEnablePin, OUTPUT);
	digitalWrite(TxEnablePin, LOW);
}

void ModbusClient::setup(SoftwareSerial& serial, uint8_t _slaveID) {
	setup(_slaveID, 0, -1);
	port = &serial;
}

void ModbusClient::setup(uint8_t _slaveID, uint8_t _TxEnablePin) { setup(_slaveID, _TxEnablePin, 0); }

void ModbusClient::begin(long baud) {
	//static_cast<HardwareSerial*>(port)->begin(baud);
	//static_cast<SoftwareSerial*>(port)->begin(baud);
	//port->begin(baud);
	// Modbus states that a baud rate higher than 19200 must use a fixed 750 us
	// for inter character time out and 1.75 ms for a frame delay.
	// For baud rates below 19200 the timeing is more critical and has to be calculated.
	// E.g. 9600 baud in a 10 bit packet is 960 characters per second
	// In milliseconds this will be 960characters per 1000ms. So for 1 character
	// 1000ms/960characters is 1.04167ms per character and finaly modbus states an
	// intercharacter must be 1.5T or 1.5 times longer than a normal character and thus
	// 1.5T = 1.04167ms * 1.5 = 1.5625ms. A frame delay is 3.5T.
	// Added experimental low latency delays. This makes the implementation
	// non-standard but practically it works with all major modbus master implementations.

	/*if (baud == 1000000 && _lowLatency) {
		T1_5 = 1;
		T3_5 = 10;
	} else if (baud >= 115200 && _lowLatency){
		T1_5 = 75;
		T3_5 = 175;
	} else */if (baud > 19200) {
		T1_5 = 750;
		T3_5 = 1750;
	}
	else {
		T1_5 = 15000000 / baud; // 1T * 1.5 = T1.5
		T3_5 = 35000000 / baud; // 1T * 3.5 = T3.5
	}

	while (port->read() >= 0);
#ifdef MODBUS_DEBUG
	Serial.println("port started");
#endif
}

bool ModbusClient::error(uint8_t code = 0) {
	errorCount++;
#ifdef MODBUS_DEBUG
	/*Serial.print("error [");
	Serial.print(code);
	Serial.print("]");
	Serial.println();*/
#endif
	return false;
}

bool ModbusClient::available() {
	if (disabledFlag || !port->available()) {
		return false;
	}

	disabledFlag = true;
	//uint8_t buffer[MODBUS_BUFFER_SIZE];
	uint8_t overflow = 0;
	bufferSize = 0;

#ifdef MODBUS_DEBUG
	Serial.print("Modbus.available: ");
#endif
	while (port->available()) {
		// The maximum number of bytes is limited to the serial buffer size of 128 bytes
		// If more bytes is received than the MODBUS_BUFFER_SIZE the overflow flag will be set and the
		// serial buffer will be red untill all the data is cleared from the receive buffer.
		if (overflow)
			port->read();
		else {
			if (bufferSize >= MODBUS_BUFFER_SIZE)
				overflow = 1;
			buffer[bufferSize] = port->read();
#ifdef MODBUS_DEBUG
			/**/Serial.print("[");
			Serial.print(buffer[bufferSize]);
			Serial.print("]");
#endif
			bufferSize++;
		}
		delayMicroseconds(T1_5); // inter character time out
	}
#ifdef MODBUS_DEBUG
	Serial.println();
#endif

	disabledFlag = false;

	// If an overflow occurred increment the errorCount
	// variable and return to the main sketch without
	// responding to the request i.e. force a timeout
	if (overflow)
		return error(1);
	else if (bufferSize <= 6)
		return error(2); // corrupted packet
	else if (buffer[0] > 0 && buffer[0] != slaveID)
		return false; //do nothing, slaveID not match

	//validate request
	uint16_t crc = toUint16(buffer[bufferSize - 2], buffer[bufferSize - 1]); // combine the crc Low & High bytes
	if (CRC16(buffer, bufferSize - 2) != crc) {
		//sendException(ILLEGAL_DATA_VALUE); // corrupted packet
		return false;
	}
	//else if (disabled)
	//	return exception(SLAVE_DEVICE_BUSY);

#ifdef MODBUS_DEBUG
	Serial.print("RX: ");
	for (uint8_t i = 0; i < bufferSize; i++) {
		Serial.print("[");
		Serial.print(buffer[i], HEX);
		Serial.print("]");/**/
	}
	Serial.println();
#endif

	regAdd = toUint16(buffer[ADD_HI], buffer[ADD_LO]); // combine the starting address bytes
	coilsNo = toUint16(buffer[NB_HI], buffer[NB_LO]);

	//Serial.println("request recived");
	//default response

	return true;
}

void ModbusClient::flush() {
	buffer[0] = 0;
	buffer[1] = 0;
}

bool ModbusClient::isBroadcast() { return buffer[ID] == 0; };

bool ModbusClient::isEmpty() { return buffer[1] == 0; };

uint8_t ModbusClient::getId() { return buffer[ID]; }

uint8_t ModbusClient::getFunction() { return buffer[FUNC]; }

uint16_t ModbusClient::getAddress() { return regAdd; }

uint16_t ModbusClient::getCoilsNumber() { return coilsNo; }

uint8_t ModbusClient::getBytesNumber() { return buffer[BYTE_CNT]; }

uint16_t ModbusClient::getByte(uint8_t pos) { return buffer[BYTE_CNT + pos]; }

void ModbusClient::sendResponse(uint8_t* frame, uint8_t frameSize) {
	if (!isBroadcast()) {// don't respond if its a broadcast message
		port->flush();

    	digitalWrite(TxEnablePin, HIGH);

    	addCRC(frame, frameSize);
    	frameSize += 2;

    	for (uint8_t i = 0; i < frameSize; i++)
    		port->write(frame[i]);

    	port->flush();

    	// allow a frame delay to indicate end of transmission
    	delayMicroseconds(T3_5);

    	digitalWrite(TxEnablePin, LOW);

#ifdef MODBUS_DEBUG
    	Serial.print("TX (buffer=");
    	Serial.print(bufferSize);
    	Serial.print("): ");
    	for (uint8_t i = 0; i < frameSize; i++) {
    		Serial.print("<");
    		Serial.print(frame[i], HEX);
    		Serial.print(">");
    	}
    	Serial.println();
#endif
	}

	flush();
}

void ModbusClient::sendException(uint8_t errorCode) {
	errorCount++; // each call to exceptionResponse() will increment the errorCount
	if (isBroadcast()) {
		flush();
		return;
	}

	uint8_t frame[MODBUS_BUFFER_SIZE];
	frame[0] = buffer[0];
	frame[1] = buffer[1] | 0x80;
    frame[2] = errorCode;
	sendResponse(frame, 3); // exception response is always 5 bytes ID, function + 0x80, exception code, 2 bytes crc
}

void ModbusClient::sendException() { sendException(1); }

void ModbusClient::sendFC1(uint16_t* data, uint8_t noOfCoils) {
	uint8_t frame[MODBUS_BUFFER_SIZE];
	frame[0] = buffer[0];
	frame[1] = buffer[1];
	frame[2] = sizeof(data);
	uint8_t pos = 3;
	for (uint8_t i = 0; i < noOfCoils; i++) {
		frame[pos++] = data[i];
	}
	sendResponse(frame, pos);
}

void ModbusClient::sendFC1(bool* data, uint8_t noOfCoils) {
	uint8_t frame[MODBUS_BUFFER_SIZE];
	frame[0] = buffer[0];
	frame[1] = buffer[1];
	frame[2] = sizeof(data);
	uint8_t pos = 3;
	for (uint8_t i = 0; i < noOfCoils; i++) {
		frame[pos++] = toUint16(data[i]);
	}
	sendResponse(frame, pos);
}

void ModbusClient::sendFC2(uint16_t* data, uint8_t noOfCoils) { sendFC1(data, noOfCoils); }

void ModbusClient::sendFC2(bool* data, uint8_t noOfCoils) { sendFC1(data, noOfCoils); }

void ModbusClient::sendFC3(uint16_t* data, uint8_t noOfCoils) {
	uint8_t frame[MODBUS_BUFFER_SIZE];
	frame[0] = buffer[0];
	frame[1] = buffer[1];
	frame[2] = sizeof(data);
	uint8_t pos = 3;
	for (uint8_t i = 0; i < noOfCoils; i++) {
		addUint16Value(frame, data[i], pos);
		pos += 2;
	}
	sendResponse(frame, pos);
}

void ModbusClient::sendFC4(uint16_t* data, uint8_t noOfCoils) { sendFC3(data, noOfCoils); }

void ModbusClient::sendFC5(uint16_t regAdd, uint16_t reqState) {
	uint8_t frame[MODBUS_BUFFER_SIZE];
	frame[0] = buffer[0];
	frame[1] = buffer[1];
	addUint16Value(frame, regAdd, 2);
	addUint16Value(frame, reqState, 4);
	sendResponse(frame, 6);
}

void ModbusClient::sendFC6(uint16_t regAdd, uint16_t reqVal) { sendFC5(regAdd, reqVal); }

void ModbusClient::sendFC15(uint16_t firstRegAdd, uint16_t noOfCoils) {
	uint8_t frame[MODBUS_BUFFER_SIZE];
	frame[0] = buffer[0];
	frame[1] = buffer[1];
	addUint16Value(frame, firstRegAdd, 2);
	addUint16Value(frame, noOfCoils, 4);
	sendResponse(frame, 6);
}

void ModbusClient::sendFC16(uint16_t firstRegAdd, uint16_t noOfCoils) { sendFC15(firstRegAdd, noOfCoils); }