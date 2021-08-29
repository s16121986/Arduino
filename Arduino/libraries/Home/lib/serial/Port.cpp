namespace SerialPort{
	
	void setup() {
		Serial.begin(9600);
		while (!Serial);
#ifdef DEBUG_PORT
		Serial.println("serial started");
#endif
	}

	bool isAvailable() {
		return Serial.available();
	}

	bool read(uint8_t (&buffer)[4]) {
		Serial.readBytes(buffer, 4);
	}
	
	void clear() {
		while (Serial.available()) {
			Serial.read();
			delay(1); //wait for the next byte, if after this nothing has arrived it means the text was not part of the same stream entered by the user
		}
	}
	
}