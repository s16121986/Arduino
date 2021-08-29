
namespace Room{ namespace BD{
	using namespace Device;

	Button mainSwitch(M_BD_SWITCH);  //Выключатель
    Button projectorSwitch(M_BD_PROJECTOR);  //Выключатель
    Relay lightningSwitch(M_BD_LIGHTNING_SWITCH);
    Ping curtains(M_BD_CURTAINS);

	void listen() {
		mainSwitch.listen();
		projectorSwitch.listen();
	}

	void loop() {
		mainSwitch.onEvents();
		projectorSwitch.onPress();
		curtains.loop();
	}

	void trigger(uint8_t target, uint8_t action, uint8_t data) {
		switch (target) {
			case M_BD_SWITCH:            mainSwitch.trigger(action, data); break;
			case M_BD_PROJECTOR:         projectorSwitch.trigger(action, data); break;
			case M_BD_LIGHTNING_SWITCH:  lightningSwitch.trigger(action, data); break;
			case M_BD_CURTAINS:          curtains.trigger(action, data); break;
		}
	}
	
} }