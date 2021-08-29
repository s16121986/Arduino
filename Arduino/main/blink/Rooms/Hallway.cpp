
namespace Room{ namespace HL{
	using namespace Device;

	Button mainSwitch(M_HL_SWITCH);        //Выключатель
    Button hallSwitch(M_HL_SWITCH_HALL);   //Выключатель
    MotionSensor motionSensor(M_HL_MOTION);    //Выключатель

	void listen() {
		mainSwitch.listen();
		hallSwitch.listen();
		motionSensor.listen();
	}

	void loop() {
		mainSwitch.onHold();
		mainSwitch.onRelease();

		hallSwitch.onEvents();
		motionSensor.onHigh();
	}

	void trigger(uint8_t target, uint8_t action, uint8_t data) {
		switch (target) {
			case M_HL_SWITCH:            mainSwitch.trigger(action, data); break;
			case M_HL_SWITCH_HALL:       hallSwitch.trigger(action, data); break;
			case M_HL_MOTION:            motionSensor.trigger(action, data); break;
		}
	}
	
} }