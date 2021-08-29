namespace Modbus{
	Request::Request () {
		broadcastFlag = false;
	}
	bool Request::setBuffer(uint8_t _buffer[MODBUS_BUFFER_SIZE], uint8_t _bufferSize) {
		//unsigned char buffer = sizeof(frame);
		//*buffer = &_buffer;
		bufferSize = _bufferSize;
		//Serial.println("Modbus input: ");
		//Serial.println(ID);

		for (uint8_t i = 0; i < _bufferSize; i++) {
			buffer[i] = _buffer[i];
		}

		/*Serial.print("ID=");
		Serial.print(buffer[ID]);
		Serial.print(";");*/
        if (buffer[ID] == 0)
            broadcastFlag = true;
        else if (buffer[ID] != slaveID) {
			//Serial.println("slaveID!=");
            return false;
        } else
	        broadcastFlag = false;

#ifdef MODBUS_DEBUG
		Serial.print("RX: ");
		for (uint8_t i = 0; i < _bufferSize; i++) {
			Serial.print("[");
			Serial.print(buffer[i], HEX);
			Serial.print("]");/**/
		}
		Serial.println();
#endif

        regAdd = toUint16(buffer[ADD_HI], buffer[ADD_LO]); // combine the starting address bytes
        coilsNo = toUint16(buffer[NB_HI], buffer[NB_LO]); // combine the number of register bytes

		/*Serial.print("function=");
		Serial.print(buffer[FUNC]);
		Serial.print(";");

		Serial.print("regAdd=");
		Serial.print(regAdd);
		Serial.print(";");

		Serial.print("coilsNo=");
		Serial.print(coilsNo);
		Serial.print(";");

		Serial.println(" ");*/

		return true;
	}
	bool Request::validate() {
        uint16_t crc = toUint16(buffer[bufferSize - 2], buffer[bufferSize - 1]); // combine the crc Low & High bytes
		return CRC16(buffer, bufferSize - 2) == crc;
	}
	bool Request::isBroadcast() { return broadcastFlag; };
	uint8_t Request::getFunction() { return buffer[FUNC]; }
	uint16_t Request::getAddress() { return regAdd; }
	uint16_t Request::getCoilsNumber() { return coilsNo; }
	uint8_t Request::getBytesNumber() { return buffer[BYTE_CNT]; }
	uint16_t Request::getValue() { return coilsNo; }
	uint16_t Request::getData(uint8_t pos) { return buffer[BYTE_CNT + pos]; }
}