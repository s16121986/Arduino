#include "../Entities/IR.cpp"
#include "../Entities/Servo.cpp"
#include "../Entities/Button.cpp"

#define INACTIVE_DELAY 4000 //
#define RESET_TIMEOUT 1000 //

namespace WC{

	uint16_t FLUSH_HALF_DELAY = 1000;
    uint16_t FLUSH_FULL_DELAY = 2500;
    uint16_t FLUSH_HALF_TIME = 20000;
    uint16_t FLUSH_FULL_TIME = 60000;
    uint16_t IR_MODE = 1;
    uint16_t SOUND_MODE = 0;

	bool disabledFlag = false;

	bool busyFlag = false;

	Button btn(PIN_WC_SWITCH);

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
	}

	void busy(bool flag) { busyFlag = flag; }

	bool isEnabled() { return !disabledFlag; }

	void disable() { disabledFlag = true; }

	void enable() { disabledFlag = false; }

	void setEnabled(bool flag) {
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

	void flush() { flush(FLUSH_HALF_DELAY); }

	void halfFlush() { flush(FLUSH_HALF_DELAY); }

	void fullFlush() { flush(FLUSH_FULL_DELAY); }

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
			delay(FLUSH_HALF_DELAY - 200);
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

	void listen() {
		if (disabledFlag)
			return;

		btn.listen();
		if (IR_MODE)
			IR::listen();
	}

	void loop() {
		if (disabledFlag || busyFlag || processBtn())
			return;

		if (IR_MODE) {
			if (IR::onLeft()) {
				IR::disable();
                if (IR::getBarrierTime() > FLUSH_HALF_TIME)
                    fullFlush();
                else
                    halfFlush();
				IR::enable();
            }/* else if (SOUND_MODE && IR::onDetect())
                tone(PIN_SOUND, SOUND_MODE * 100, 700);*/
		}
	}
}