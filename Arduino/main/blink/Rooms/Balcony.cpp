
namespace Room{ namespace BL{
	using namespace Device;

	Button mainSwitch(M_BL_SWITCH);  //Выключатель

	void listen() {
		mainSwitch.listen();
	}

	void loop() {
		mainSwitch.onEvents();
	}

	void trigger(uint8_t target, uint8_t action, uint8_t data) {
		switch (target) {
			case M_BL_SWITCH:    mainSwitch.trigger(action, data); break;
		}
	}
	
} }