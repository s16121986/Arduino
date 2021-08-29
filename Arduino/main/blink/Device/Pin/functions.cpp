namespace Device{

	void send(uint8_t pckg, uint8_t pin, uint8_t param, uint8_t data) {
		Serial.write(pckg);
	#ifdef DEBUG_PORT
			Serial.print("Device::send: ");
			Serial.print(pin);
			Serial.print(" ");
			Serial.print(param);
			Serial.print(" ");
			Serial.print(data);
			Serial.println(";");
	#else
			Serial.write(pin);
			Serial.write(param);// == 0 ? 254 : param
			Serial.write(data);
			Serial.write(IO_STOP_BIT);
	#endif
	}
	void sendState(uint8_t pin, uint8_t state, uint8_t data = 0) { send(IO_STATE_BIT, pin, state, data); }
	void sendEvent(uint8_t pin, uint8_t e, uint8_t data = 0) { send(IO_EVENT_BIT, pin, e, data); }
	void sendData(uint8_t pin, uint8_t data) { sendState(pin, STATE_AVAILABLE | STATE_ENABLED, data); }
	void sendPing(uint8_t pin, uint8_t state = 1) { send(IO_PING_BIT, pin, state, 0); }

}