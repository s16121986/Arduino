#define INACTIVE_DELAY 4000 //
#define RESET_TIMEOUT 1000 //

namespace Controller{
	using namespace HoldingRegisters;

	bool disabledFlag = false;
	bool busyFlag = false;
	unsigned long int lastActiveTime = 0;
	Button btn(PIN_SWITCH);

	void activity(bool flag) {
		if (flag)
			lastActiveTime = CURRENT_TIME;
		else
			lastActiveTime = 0;
	}
	void reset() {
		btn.reset();
        IR::reset();
        busyFlag = false;
#ifdef DEBUG_PORT
        Serial.println("controller reset");
#endif
	}
	void reset(unsigned int timeout) {
		delay(timeout);
        reset();
        activity(true);
	}
	void busy(bool flag) { busyFlag = flag; }
	bool disabled() { return disabledFlag; }
	void disable() { disabledFlag = true; }
	void enable() { disabledFlag = false; }
	void setEnabled(bool flag) {
		//Coils::setFlag(Coils::ENABLED, flag);
#ifdef DEBUG_PORT
        Serial.print("setEnabled (");
        Serial.print(flag);
        Serial.println(")");
#endif
		if (flag)
            enable();
        else
            disable();
	}
	void flush(unsigned int timeout) {
#ifdef DEBUG_PORT
        Serial.print("flush (");
        Serial.print(timeout);
        Serial.println(")");
#endif
    	busyFlag = true;
    	Servo::up();
        delay(timeout);
		Servo::down();

        reset(RESET_TIMEOUT);
	}
	void flush() { flush(registersValues[FLUSH_HALF_DELAY]); }
	void halfFlush() { flush(registersValues[FLUSH_HALF_DELAY]); }
	void fullFlush() { flush(registersValues[FLUSH_FULL_DELAY]); }
	bool processBtn() {
		if (!btn.onPress())
			return false;

    	busyFlag = true;

		Servo::up();
		delay(200);
		if (btn.onRelease()) { // Авто режим (половина бачка)
#ifdef DEBUG_PORT
			Serial.println("btn released");
#endif
			delay(registersValues[FLUSH_HALF_DELAY] - 200);
		} else { //Удержание (отключение при отжатии)
#ifdef DEBUG_PORT
			Serial.println("btn holded");
#endif
			uint8_t i = 0;
			while (btn.isPressed() && i++ < 100)
				delay(50);
		}

		Servo::down();

		reset(RESET_TIMEOUT);

		return true;
	}
	bool isActive() { return IR::getState() || btn.getState(); }
	bool isInactive() { return lastActiveTime > 0 && (CURRENT_TIME - lastActiveTime) > INACTIVE_DELAY; }
	void listen() {
		if (disabledFlag)
			return;

		btn.listen();
#ifdef PIN_IR
		if (registersValues[IR_MODE] > 0)
			IR::listen();
#endif
	}
	void loop() {
		if (disabledFlag || busyFlag || processBtn())
			return;

#ifdef PIN_IR
		if (registersValues[IR_MODE]) {
			if (IR::onLeft()) {
				IR::disable();
                if (IR::getBarrierTime() > registersValues[FLUSH_FULL_TIME])
                    fullFlush();
                else
                    halfFlush();
				IR::enable();
            } else if (registersValues[SOUND_MODE] && IR::onDetect())
                tone(PIN_SOUND, registersValues[SOUND_MODE] * 100, 700);
		}
#endif
	}
}