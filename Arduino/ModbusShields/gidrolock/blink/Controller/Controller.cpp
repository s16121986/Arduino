unsigned long int GIDROLOCK_OPEN_DELAY = 15000; //Время поворота шарового крана на 90 градусов 15 sec

#include "../Entities/Gidrolock.cpp"

namespace Controller{

	Gidrolock motorCold(PIN_COLD_POWER, PIN_COLD_CTRL);

	Gidrolock motorHot(PIN_HOT_POWER, PIN_HOT_CTRL);

	volatile bool alarmFlag = false;

	bool isAlarm() {
		Serial.print("isAlarm ");
		Serial.print(alarmFlag);
		Serial.println();
		return alarmFlag;
	}

	void alarm() {
#ifdef DEBUG_PORT
		Serial.println("alarm");
#endif
		alarmFlag = true;
		motorCold.close();
		motorHot.close();
	}

	void open() {
#ifdef DEBUG_PORT
		Serial.println("open");
#endif
		alarmFlag = false;
        motorCold.open();
        motorHot.open();
	}

	void alarmState(bool flag) {
		if (flag)
			alarm();
		else
			open();
	}

	void listen() {
		motorCold.listen();
		motorHot.listen();

		if (alarmFlag)
			return;

		if (digitalRead(PIN_BT_LEAK) == HIGH || digitalRead(PIN_KH_LEAK) == HIGH)
			alarm();
	}
}