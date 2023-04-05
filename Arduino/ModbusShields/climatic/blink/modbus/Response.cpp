namespace Modbus{
	class Response{
	public:
		Response(unsigned char function) {
			frame[0] = slaveID;
			frame[1] = function;
		}
		void sendData(unsigned char *data) {
			frame[2] = sizeof(data);
			unsigned char pos = 3;
			for (int i = 0; i < frame[2]; i++) {
                frame[pos++] = data[i];
            }
            // only the first 6 bytes are used for CRC calculation
            unsigned int crc16 = CRC16(frame, pos);
            frame[pos++] = crc16 >> 8; // split crc into 2 bytes
            frame[pos] = crc16 & 0xFF;
            send(6);
		}
		void sendData(unsigned char data) {
			unsigned char multipleData[1];
			multipleData[0] = data
			sendData(multipleData);
		}
		void sendException(unsigned char code) {
			frame[1] |= 0x80;
            frame[2] = code;
            unsigned int crc16 = CRC16(frame, 3); // ID, function + 0x80, exception code == 3 bytes
            frame[3] = crc16 >> 8;
            frame[4] = crc16 & 0xFF;
            send(5); // exception response is always 5 bytes ID, function + 0x80, exception code, 2 bytes crc
		}
	private:
		void send(unsigned char bufferSize) {
			digitalWrite(TxEnablePin, HIGH);

			for (unsigned char i = 0; i < bufferSize; i++)
				Serial.write(frame[i]);

			Serial.flush();

			// allow a frame delay to indicate end of transmission
			delayMicroseconds(T3_5);

			digitalWrite(TxEnablePin, LOW);
        }
		unsigned char frame[6];
	}
}