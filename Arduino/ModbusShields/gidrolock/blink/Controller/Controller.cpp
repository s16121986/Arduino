unsigned long int GIDROLOCK_OPEN_DELAY = 1500; //Время поворота шарового крана на 90 градусов 15 sec

#include "../Entities/Gidrolock.cpp"

namespace Controller{

	Gidrolock motorCold(PIN_COLD_POWER, PIN_COLD_CTRL);

	Gidrolock motorHot(PIN_HOT_POWER, PIN_HOT_CTRL);

	bool alarmState = false;

	bool isAlarm() { return alarmState; }

	void alarm() {
		alarmState = true;
		motorCold.close();
		motorHot.close();
	}

	void d() {
		alarmState = false;
        motorCold.open();
        motorHot.open();
	}

	void alarmState(bool flag) {
		alarmState = flag;
	}

	void loop() {
		if (digitalRead(PIN_BT_LEAK) == HIGH || digitalRead(PIN_KH_LEAK) == HIGH)
			leakAlarm();
	}
}