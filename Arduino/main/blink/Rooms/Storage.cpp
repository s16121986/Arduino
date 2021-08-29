
namespace Room{ namespace ST{
	using namespace Device;

	Button mainSwitch(M_ST_SWITCH);
	//PWMD vent(M_ST_VENTILATION);
	DoorSensor doorSensor(M_ST_DOOR);    //Выключатель

	void listen() {
		mainSwitch.listen();
		doorSensor.listen();
	}

	void loop() {
		mainSwitch.onPress();
		doorSensor.onChange();
	}

	void trigger(uint8_t target, uint8_t action, uint8_t data) {
		switch (target) {
			case M_ST_SWITCH:            mainSwitch.trigger(action, data); break;
			case M_ST_DOOR:              doorSensor.trigger(action, data); break;
			//case M_ST_VENTILATION:       ST::vent.trigger(action, data); break;
		}
	}
} }