#define MODBUS_BUFFER_SIZE 128

namespace Modbus{ class Client{
public:
	void sendResponse();

	bool exception(uint8_t code);

	bool exception();

	bool error();
	//void setDisabled(bool flag);

	void sendException(uint8_t errorCode);
private:
	Stream *port;
	Request req;
	Response response;
	// frame[] is used to recieve and transmit packages.
	// The maximum serial ring buffer size is 128
	uint8_t slaveID;
	uint8_t TxEnablePin;
	unsigned int T1_5; // inter character time out
	unsigned int T3_5; // frame delay
	unsigned int errorCount;

	static void init_crc16_table( void );
	static bool crc_tab16_init = false;
	static uint16_t crc_tab16[256];
	bool disabledFlag = false;
	bool processingFlag = false;

	void readCoils();
	void readDiscreteInputs();
	void readHoldingRegisters();
	void readInputRegisters();
	void writeSingleCoil();
	void writeSingleRegister();
	void writeMultipleCoils();
	void writeMultipleRegister();

	uint8_t registersCount[6];
	bool registeredFunctions[6] = {false, false, false, false, false, false};
	uint16_t (*readFunctions[READ_FUNCTIONS_SIZE])(uint16_t reqAdd);
	void (*writeCoil)(uint16_t reqAdd, bool flag);
	void (*writeRegister)(uint16_t reqAdd, uint16_t value);

}; }