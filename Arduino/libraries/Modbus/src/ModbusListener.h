using rfunc = uint16_t (*)();

#include "./Listener/WriteListener.cpp"
#include "./Listener/ReadListener.cpp"

class ModbusListener {
private:

public:
	ModbusClient* modbus;
	ReadListener readCoilListener;
	ReadListener readDiscreteInputListener;
	ReadListener readHoldingRegisterListener;
	ReadListener readInputRegisterListener;
	WriteListener<bool> writeCoilListener;
	WriteListener<uint16_t> writeRegisterListener;

	ModbusListener(ModbusClient& modbus);

	void registerCoilRead(uint16_t reqAdd, uint16_t (*f)());

	void registerDiscreteInputRead(uint16_t reqAdd, uint16_t (*f)());

	void registerHoldingRegisterRead(uint16_t reqAdd, uint16_t (*f)());

	void registerInputRegisterRead(uint16_t reqAdd, uint16_t (*f)());

	void registerCoilWrite(uint16_t reqAdd, void (*f)(bool flag));

	void registerHoldingRegisterWrite(uint16_t reqAdd, void (*f)(uint16_t value));

	void readCoils();

	void readDiscreteInputs();

	void readHoldingRegisters();

	void readInputRegisters();

	void writeSingleCoil();

	void writeSingleRegister();

	void writeMultipleCoils();

	void writeMultipleRegister();
	
	void run();
};