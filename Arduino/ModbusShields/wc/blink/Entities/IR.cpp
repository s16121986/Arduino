//KGS-812A
#define IR_EVENT_LEFT_TIME 20000
#define IR_EVENT_TIMEOUT 500

namespace IR{
	bool disabledFlag = false;

	//bool detectedFlag = false;

	bool currentState = false;

	//bool eventDetected = false;

	volatile bool eventLeft = false;

	unsigned long int barrierTime = 0;

	unsigned long int eventDelayTime = 0;

	void reset();

	bool disabled() { return disabledFlag; }

	void disable() { disabledFlag = true; }

	void enable() { disabledFlag = false; }

	bool getState() { return digitalRead(PIN_WC_IR); }

	bool isBarrier() { return currentState == true; }

	bool isFree() { return currentState == false; }

	/*bool onDetect() {
		if (disabledFlag || !eventDetected)
			return false;
		eventDetected = false;
		return true;
	}*/

	bool onLeft() {
		if (disabledFlag || !eventLeft)
			return false;
#ifdef DEBUG_PORT
            	Serial.println("ir onLeft()");
#endif
		eventLeft = false;
		return true;
	}

	unsigned long int getBarrierTime() { return barrierTime; }

	void reset() {
		//detectedFlag = false;
		currentState = false;
		//eventDetected = false;
		eventLeft = false;
		barrierTime = 0;
		eventDelayTime = 0;
#ifdef DEBUG_PORT
            	Serial.println("ir reset()");
#endif
	}

	void listen() {
		if (disabledFlag || eventLeft)//
			return;

		bool state = getState();

		if (currentState == state) {
			if (isBarrier())
				return;
			//если был более Х секунд запуск таймера до смыва
			if (eventDelayTime > 0 && CURRENT_TIME - eventDelayTime > IR_EVENT_TIMEOUT) {
                eventLeft = true;
               // barrierTime = 0;
                //eventDelayTime = 0;
            }/* else if (barrierTime > 0 && !detectedFlag && CURRENT_TIME - barrierTime > getFlushHalfTime()) {
                detectedFlag = true;
                eventDetected = true;
#ifdef DEBUG_PORT
                Serial.println("ir detected");
#endif
            }*/
		} else {
			currentState = state;

			//Если обнаружен человек запуск таймера
			if (isBarrier()) {
#ifdef DEBUG_PORT
            	Serial.print("Barrier (");
            	Serial.print(IR_EVENT_LEFT_TIME);
            	Serial.println(")");
#endif
                eventDelayTime = 0;
                if (0 == barrierTime)
                    barrierTime = CURRENT_TIME;
            //Если человек ушел
            } else if (barrierTime > 0 && CURRENT_TIME - barrierTime > IR_EVENT_LEFT_TIME) {
#ifdef DEBUG_PORT
				Serial.println("ir event timer");
#endif
#ifdef IR_EVENT_TIMEOUT
                eventDelayTime = CURRENT_TIME; //если был более Х секунд запуск таймера до смыва
#else
                eventLeft = true;
#endif
            } else {
#ifdef DEBUG_PORT
            	Serial.print("ir reset (");
            	Serial.print(CURRENT_TIME - barrierTime);
            	Serial.println(")");
#endif
                reset(); //если был менее Х секунд сброс
			}
		}
	}
}