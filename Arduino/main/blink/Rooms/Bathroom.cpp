
namespace Room{ namespace BT{
	using namespace Device;

	Button mainSwitch(M_BT_SWITCH);    //Выключатель
    Button fanSwitch(M_BT_SWITCH_FAN);    //Выключатель
    MotionSensor motionSensor(M_BT_MOTION);    //Выключатель
    LeakSensor leakSensor(M_BT_LEAK);    //Выключатель

	void listen() {
		mainSwitch.listen();
		fanSwitch.listen();
		motionSensor.listen();
		leakSensor.listen();
	}

	void loop() {
		mainSwitch.onEvents();
		fanSwitch.onPress();
		motionSensor.onHigh();
		leakSensor.onChange();
	}

	void trigger(uint8_t target, uint8_t action, uint8_t data) {
		switch (target) {
			case M_BT_SWITCH:            mainSwitch.trigger(action, data); break;
			case M_BT_SWITCH_FAN:        fanSwitch.trigger(action, data); break;
			case M_BT_MOTION:            motionSensor.trigger(action, data); break;
			case M_BT_LEAK:              leakSensor.trigger(action, data); break;
		}
	}
	
} }