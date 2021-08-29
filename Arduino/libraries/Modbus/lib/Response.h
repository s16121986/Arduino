namespace Modbus{
	class Response{
	public:
		Response();
		void setRequest(Request req);
		void processFC1(uint16_t *data, uint8_t noOfCoils);
		void processFC1(bool *data, uint8_t noOfCoils);
		void processFC2(uint16_t *data, uint8_t noOfCoils);
		void processFC2(bool *data, uint8_t noOfCoils);
		void processFC3(uint16_t *data, uint8_t noOfCoils);
		void processFC4(uint16_t *data, uint8_t noOfCoils);
		void processFC5(uint16_t regAdd, uint16_t reqState);
		void processFC6(uint16_t regAdd, uint16_t reqVal);
		void processFC15(uint16_t firstRegAdd, uint16_t noOfCoils);
		void processFC16(uint16_t firstRegAdd, uint16_t noOfCoils);
        void setException(uint8_t code);
        bool isSent();
        void send();
    private:
        void addCRC(uint8_t pos);
        void addUint16Value(uint16_t value, uint8_t pos);
		uint8_t bufferSize;
		uint8_t frame[MODBUS_BUFFER_SIZE];
	};
}