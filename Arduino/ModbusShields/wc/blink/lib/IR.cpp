//KGS-812A
namespace IR{
	using namespace HoldingRegisters;

	bool disabledFlag = false;
	bool detectedFlag = false;
	bool currentState = false;
	bool eventDetected = false;
	bool eventLeft = false;
	unsigned long int barrierTime = 0;
	unsigned long int eventDelayTime = 0;

	void reset();

	bool disabled() { return disabledFlag; }
	void disable() { disabledFlag = true; }
	void enable() { disabledFlag = false; }
	bool getState() { return digitalRead(PIN_IR); }
	bool isBarrier() { return currentState == true; }
	bool isFree() { return currentState == false; }
	bool onDetect() {
		if (disabledFlag || !eventDetected)
			return false;
		eventDetected = false;
		return true;
	}
	bool onLeft() {
		if (disabledFlag || !eventLeft)
			return false;
		eventLeft = false;
		return true;
	}
	unsigned long int getBarrierTime() { return barrierTime; }
	void reset() {
		detectedFlag = false;
		currentState = false;
		eventDetected = false;
		eventLeft = false;
		barrierTime = 0;
		eventDelayTime = 0;
	}
	void listen() {
		if (disabledFlag || eventLeft)//
			return;

		bool state = getState();

		if (currentState == state) {
			if (!isBarrier())
				return;
			//если был более Х секунд запуск таймера до смыва
			if (eventDelayTime > 0 && CURRENT_TIME - eventDelayTime > registersValues[IR_EVENT_TIMEOUT]) {
                eventLeft = true;
               // barrierTime = 0;
                //eventDelayTime = 0;
#ifdef DEBUG_PORT
            	Serial.println("ir left event");
#endif
            } else if (barrierTime > 0 && !detectedFlag && CURRENT_TIME - barrierTime > registersValues[FLUSH_HALF_TIME]) {
                detectedFlag = true;
                eventDetected = true;
#ifdef DEBUG_PORT
                Serial.println("ir detected");
#endif
            }
		} else {
			currentState = state;

			//Если обнаружен человек запуск таймера
			if (isBarrier()) {
#ifdef DEBUG_PORT
            	Serial.println("Barrier");
#endif
                eventDelayTime = 0;
                if (0 == barrierTime)
                    barrierTime = CURRENT_TIME;
            //Если человек ушел
            } else if (barrierTime > 0 && CURRENT_TIME - barrierTime > registersValues[FLUSH_HALF_TIME]) {
                if (registersValues[IR_EVENT_TIMEOUT] == 0)
                    eventLeft = true;
                else {
                    eventDelayTime = CURRENT_TIME; //если был более Х секунд запуск таймера до смыва
#ifdef DEBUG_PORT
            	Serial.println("ir event timer");
#endif
				}
            } else {
                reset(); //если был менее Х секунд сброс
#ifdef DEBUG_PORT
            	Serial.println("ir reset");
#endif
			}
		}
	}
}