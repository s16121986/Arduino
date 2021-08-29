
namespace Room{ namespace KH{
	using namespace Device;

	Button mainSwitch(M_KH_SWITCH);

	void listen() {
		mainSwitch.listen();
	}

	void loop() {
		mainSwitch.onEvents();
	}

	void trigger(uint8_t target, uint8_t action, uint8_t data) {
		switch (target) {
			case M_KH_SWITCH:            mainSwitch.trigger(action, data); break;
		}
	}
	
} }