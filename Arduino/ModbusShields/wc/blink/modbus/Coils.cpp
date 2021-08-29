namespace Coils{
	enum REGISTERS{
        ENABLED,
        HALF_FLUSH,
        FULL_FLUSH,

        REGISTERS_SIZE,
        FLAGS_SIZE = 1
    };

    bool coilsFlags[FLAGS_SIZE] = {
        true //ENABLED
    };

	uint8_t count() { return REGISTERS_SIZE; }
	void setFlag(uint16_t address, bool flag) {
		if (address < FLAGS_SIZE)
			coilsFlags[address] = flag;
	}
	bool hasFlag(uint16_t address) { return address < FLAGS_SIZE; }
	bool getFlag(uint16_t address) { return coilsFlags[address]; }
	bool has(uint16_t address) { return address < REGISTERS_SIZE; }

}