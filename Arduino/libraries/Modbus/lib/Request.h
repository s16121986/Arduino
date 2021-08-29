namespace Modbus{

	enum MESSAGE {
        ID = 0, //!< ID field
        FUNC, //!< Function code position
        ADD_HI, //!< Address high byte
        ADD_LO, //!< Address low byte
        NB_HI, //!< Number of coils or registers high byte
        NB_LO, //!< Number of coils or registers low byte
        BYTE_CNT  //!< byte counter
    };

	class Request{
	public:
		Request();
		bool available();
		bool Request::setBuffer(uint8_t _buffer[MODBUS_BUFFER_SIZE], uint8_t _bufferSize);
		bool validate();
        bool isBroadcast();
        uint8_t getFunction();
        uint16_t getAddress();
        uint16_t getCoilsNumber();
		uint8_t getBytesNumber();
		uint16_t getValue();
		uint16_t getData(uint8_t pos);
    private:
		uint8_t buffer[MODBUS_BUFFER_SIZE];
		uint8_t bufferSize;
        uint16_t regAdd;
        uint16_t coilsNo;
        bool broadcastFlag;
	};
}