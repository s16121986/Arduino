namespace Modbus{
	Response::Response() {}
	void Response::setRequest(Request req) {
		frame[0] = slaveID;
		frame[1] = req.getFunction();
		switch (frame[1]) {
			case FUNCTION_WRITE_SINGLE_COIL:
			case FUNCTION_WRITE_SINGLE_REGISTER:
				processFC5(req.getAddress(), req.getCoilsNumber());
				break;
			/*case FUNCTION_WRITE_MULTIPLE_COILS:
			case FUNCTION_WRITE_MULTIPLE_REGISTER:
				processFC5(req.getAddress(), req.getCoilsNumber());
				break;*/
		}
	}
	void Response::processFC1(uint16_t *data, uint8_t noOfCoils) {
		frame[2] = sizeof(data);
        uint8_t pos = 3;
        for (uint8_t i = 0; i < noOfCoils; i++) {
            frame[pos++] = data[i];
        }
        bufferSize = pos;
	}
	void Response::processFC1(bool *data, uint8_t noOfCoils) {
		frame[2] = sizeof(data);
        uint8_t pos = 3;
        for (uint8_t i = 0; i < noOfCoils; i++) {
            frame[pos++] = toUint16(data[i]);
        }
        bufferSize = pos;
	}
	void Response::processFC2(uint16_t *data, uint8_t noOfCoils) { processFC1(data, noOfCoils); }
	void Response::processFC2(bool *data, uint8_t noOfCoils) { processFC1(data, noOfCoils); }
	void Response::processFC3(uint16_t *data, uint8_t noOfCoils) {
		frame[2] = sizeof(data);
        uint8_t pos = 3;
        for (uint8_t i = 0; i < noOfCoils; i++) {
            addUint16Value(data[i], pos);
            pos += 2;
        }
        bufferSize = pos;
	}
	void Response::processFC4(uint16_t *data, uint8_t noOfCoils) { processFC3(data, noOfCoils); }
	void Response::processFC5(uint16_t regAdd, uint16_t reqState) {
        addUint16Value(regAdd, 2);
        addUint16Value(reqState, 4);
        bufferSize = 6;
	}
	void Response::processFC6(uint16_t regAdd, uint16_t reqVal) { processFC5(regAdd, reqVal); }
	void Response::processFC15(uint16_t firstRegAdd, uint16_t noOfCoils) {
        addUint16Value(firstRegAdd, 2);
        addUint16Value(noOfCoils, 4);
        bufferSize = 6;
	}
	void Response::processFC16(uint16_t firstRegAdd, uint16_t noOfCoils) { processFC15(firstRegAdd, noOfCoils); }
	void Response::setException(uint8_t code) {
		frame[1] |= 0x80;
        frame[2] = code;
        bufferSize = 3; // exception response is always 5 bytes ID, function + 0x80, exception code, 2 bytes crc
	}
	bool Response::isSent() { return frame[0] == 0; }
	void Response::send() {
		if (frame[0] == 0) //is sent
			return;

        addCRC(bufferSize);
        bufferSize += 2;

#ifdef MODBUS_DEBUG
		Serial.print("TX (buffer=");
		Serial.print(bufferSize);
		Serial.print("): ");
		for (uint8_t i = 0; i < bufferSize; i++) {
			Serial.print("<");
			Serial.print(frame[i], HEX);
			Serial.print(">");
		}
		Serial.println();
#endif

		port->flush();

		digitalWrite(TxEnablePin, HIGH);

		for (uint8_t i = 0; i < bufferSize; i++)
			port->write(frame[i]);

		port->flush();

		frame[0] = 0;

		// allow a frame delay to indicate end of transmission
		delayMicroseconds(T3_5);

		digitalWrite(TxEnablePin, LOW);
    }

	void Response::addCRC(uint8_t pos) {
		uint16_t crc = CRC16(frame, pos);
		/*Serial.print("TX crc=");
		Serial.println(crc, HEX);*/
		addUint16Value(crc, pos);
	}
	void Response::addUint16Value(uint16_t value, uint8_t pos) {
		frame[pos] = value >> 8; // split crc into 2 bytes
        frame[pos + 1] = value & 0xFF;
	}
}