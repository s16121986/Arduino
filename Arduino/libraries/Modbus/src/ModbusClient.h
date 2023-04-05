class ModbusClient {
public:
	ModbusClient();

	void ModbusClient::setup(Stream& port, uint8_t _slaveID);

	void begin(long baud);

	bool error(uint8_t code = 0);

	bool available();

	void flush();

	bool isBroadcast();

	bool isEmpty();

    uint8_t getId();

    uint8_t getFunction();

    uint16_t getAddress();

    uint16_t getCoilsNumber();

    uint8_t getBytesNumber();

    uint16_t getByte(uint8_t pos);

	void sendResponse(uint8_t* frame, uint8_t frameSize);

	void sendException(uint8_t errorCode);

	void sendException();

    void sendFC1(uint16_t* data, uint8_t noOfCoils);

    void sendFC1(bool* data, uint8_t noOfCoils);

    void sendFC2(uint16_t* data, uint8_t noOfCoils);

    void sendFC2(bool* data, uint8_t noOfCoils);

    void sendFC3(uint16_t* data, uint8_t noOfCoils);

    void sendFC4(uint16_t* data, uint8_t noOfCoils);

    void sendFC5(uint16_t regAdd, uint16_t reqState);

    void sendFC6(uint16_t regAdd, uint16_t reqVal);

    void sendFC15(uint16_t firstRegAdd, uint16_t noOfCoils);

    void sendFC16(uint16_t firstRegAdd, uint16_t noOfCoils);

private:
	Stream* port;
	// frame[] is used to recieve and transmit packages.
	// The maximum serial ring buffer size is 128
	uint8_t slaveID;
	uint8_t TxEnablePin;
	unsigned int T1_5; // inter character time out
	unsigned int T3_5; // frame delay
	unsigned int errorCount;
	bool disabledFlag = false;
	//request params
	uint8_t buffer[MODBUS_BUFFER_SIZE];
    uint8_t bufferSize;
    uint16_t regAdd;
    uint16_t coilsNo;
    bool broadcastFlag;
};