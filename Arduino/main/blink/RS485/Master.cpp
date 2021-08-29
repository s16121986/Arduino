
namespace Bus{
uint8_t currentSlaveAddress = 0;
	bool waitResponseAddress = 0;
	bool waitingTime;
	bool flagQuiet = false;
	uint8_t queue = [];

	void sendQuiet() {
		send();
	}
	void sendQuiet(uint8_t slaveAddress) {
		send();
	}

	void sendToSlave(uint8_t slaveAddress, uint8_t action, uint8_t data) {
		sendQuiet(slaveAddress);
		flagQuiet = true;
		currentSlaveAddress = slaveAddress;
	}
	void slaveRequest(uint8_t slaveAddress, uint8_t pinAddress) {
		sendToSlave(slaveAddress, pinAddress, BIT_STATE);
	}
	void command() {

	}
	void loop() {
		if (waitingTime)
			return;

		buffer = Serial.read();
		switch (buffer[0]) {
			case 'stop':
				currentSlaveAddress = buffer[1];
				sendToSlave(currentSlaveAddress, BIT_PERMIT);
				waitingTime = CURRENT_TIME;
				break;
			case 'response':
				if (currentSlaveAddress != buffer[1]) {

				}
				waitingTime = 0;
				currentSlaveAddress = 0;

				break;

		}
		if (waitResponseAddress)
	}

}