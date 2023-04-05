class ReadListener{
private:
	uint16_t addressMap[MODBUS_LISTENER_SIZE];
	uint16_t(*funcMap[MODBUS_LISTENER_SIZE])();
	uint8_t size;
public:
	ReadListener() {
		size = 0;
	}

	uint16_t handle(uint16_t addr) {
		for (uint8_t i = 0; i < size; i++) {
			if (addressMap[i] != addr)
				continue;

			return (*funcMap[i])();
		}

		return 0;
	}

	void bind(uint16_t addr, uint16_t (*func)()) {
        addressMap[size] = addr;
        funcMap[size] = func;
        size++;
    }

    bool find(uint16_t addr) {
        for (uint8_t i = 0; i < size; i++) {
            if (addressMap[i] == addr)
                return true;
        }

        return false;
    }

    bool find(uint16_t addr, uint8_t count) {
        for (uint8_t i = 0; i < count; i++) {
            if (false == find(addr + i))
                return false;
        }

        return true;
    }
};