class Gidrolock {
public:
	Gidrolock(unsigned char _pinPower, unsigned char _pinControl) {
		pinPower = _pinPower;
		pinControl = _pinControl;
		state = true;
		disabledFlag = false;
		powerTime = 0;
		pinMode(pinPower, OUTPUT);
		pinMode(pinControl, OUTPUT);
	}

	bool getState() { return state; }

	void disabled() { return disabledFlag; }

	void stop() {
		powerOff();
		powerTime = 0;
		disabledFlag = false;
#ifdef DEBUG_PORT
		Serial.println("power off");
#endif
	}

	void open() {
		if (disabledFlag)
			return stop();
#ifdef DEBUG_PORT
		Serial.print("open start (");
		Serial.print(GIDROLOCK_OPEN_DELAY);
		Serial.println(")");
#endif
		power(false);
		//control(true);
		//powerOn();
	}

	void close() {
		if (disabledFlag)
			return stop();
#ifdef DEBUG_PORT
		Serial.print("close start (");
		Serial.print(GIDROLOCK_OPEN_DELAY);
		Serial.println(")");
#endif
		power(true);
		//control(false);
		//powerOff();
	}

	void setState(bool flag) {
		if (flag)
			open();
		else
			close();
	}

	void listen() {
		if (0 == powerTime)
			return;
		if (CURRENT_TIME - powerTime > GIDROLOCK_OPEN_DELAY)
			stop();
	}
private:
	/*
	Соединение черного провода с синим проводом (GND): закрытие шарового электропривода.
	Отсоединение черного провода от синего провода (GND): открытие шарового электропривода.
	 */
	void control(bool flag) { digitalWrite(pinControl, flag); }

	void powerOn() { digitalWrite(pinPower, HIGH); }

	void powerOff() { digitalWrite(pinPower, LOW); }

	void power() {
		disabledFlag = true;
		powerOn();
		powerTime = CURRENT_TIME;
		//delay(GIDROLOCK_OPEN_DELAY);
		//powerOff();
		//disabledFlag = false;
	}

	void power(bool pinFlag) {
		state = !pinFlag;
		control(pinFlag);
		power();
	}

	unsigned long int powerTime;

	unsigned char pinPower;

	unsigned char pinControl;

	bool state;

	bool disabledFlag;
};