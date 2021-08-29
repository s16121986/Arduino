#define INACTIVE_DELAY 4000 //Время полного смыва

namespace Config{
	uint16_t FLUSH_HALF_DELAY = 1000; //Время неполного смыва
	uint16_t FLUSH_FULL_DELAY = 2000; //Время полного смыва
	uint16_t FLUSH_HALF_TIME = 20000; //Время нахождения для неполного смыва
	uint16_t FLUSH_FULL_TIME = 60000; //Время нахождения для полного смыва
	uint16_t IR_EVENT_TIMEOUT = 500; //Пауза перед авто смывом

	bool ENABLED = true;
	bool IR_ENABLED = true;
	uint16_t SOUND_MODE = 0;
}