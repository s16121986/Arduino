#define M_BT_MAINLIGHT_AUTOTIMEOUT 0
#define M_HL_MAINLIGHT_AUTOTIMEOUT 1
//#define M_BT_MAINLIGHT_AUTOTIMEOUT 0

namespace Memory {
	const uint8_t EMPTY_VALUE = 0xFF;
	
	void set(int EEPROMAddress, uint8_t value) {
		EEPROM.update(EEPROMAddress, value);
	}
	void set(int EEPROMAddress, bool value) {
		uint8_t b = value ? 1 : EMPTY_VALUE;
		set(EEPROMAddress, b);
	}
	void set(int EEPROMAddress, unsigned long int value, int multiplier) {
		uint8_t v = value / multiplier;
		set(EEPROMAddress, v);
	}
	uint8_t get(int EEPROMAddress, uint8_t defaultValue = EMPTY_VALUE) {
		uint8_t v = EEPROM.read(EEPROMAddress);
		return v == EMPTY_VALUE ? defaultValue : v;
	}
	void init(uint8_t &variable, int EEPROMAddress, uint8_t defaultValue = EMPTY_VALUE) {
		variable = get(EEPROMAddress, defaultValue);
	}
	void init (bool &variable, int EEPROMAddress, bool defaultValue = true) {
		uint8_t b = get(EEPROMAddress);
		if (b == EMPTY_VALUE) {
			variable = defaultValue;
		} else {
			variable = b == 1;
		}
	}
	void init (uint8_t &variable, int EEPROMAddress, int defaultValue, int multiplier) {
		uint8_t v = get(EEPROMAddress);
		if (v == EMPTY_VALUE) {
			variable = defaultValue;
		} else {
			variable = multiplier * v;
		}
	}
}
