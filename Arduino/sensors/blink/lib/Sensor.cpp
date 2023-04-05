#define HOLD_DELAY 30 // 1200ms / 40ms
#define DISABLE_DELAY 30 // 1200ms / 40ms

class Sensor {
public:
	Sensor(){}
	Sensor(uint8_t pin) {
		this->pin = pin;

		highState = HIGH;
		debounceDelay = 0;
		holdDelay = 0;
		disableDelay = 0;

		currentState = LOW;
		stateChanged = false;
		holdFlag = false;
		
		pinMode(pin, INPUT);
	}

	void disable(uint8_t delay) {
		disableDelay = delay;
	}
	
	bool debounce() {
		if (debounceDelay == 0) {
			debounceDelay = 1;
			return false;
		}

		debounceDelay--;

		return debounceDelay == 0;
	}

	bool isChanged() {
		if (!stateChanged)
			return false;
		stateChanged = false;
		return true;
	}
	
	bool isHigh () { return currentState == highState; }
	
	bool isLow () { return currentState != highState; }
	
	bool isHold () { return holdFlag; }

	void listen() {
		if (disableDelay > 0) {
			disableDelay--;
			return;
		} else if (stateChanged)
			return;

		bool state = getState();

		if (currentState == state) {
			debounceDelay = 0;
			if (holdFlag || isLow())
				return;

			if (holdDelay > 0)
				holdDelay--;
			else {
				holdFlag = true;
				stateChanged = true;
#ifdef DEBUG_PORT
                Serial.println("holded");
#endif
			}
		} else if (debounce()) { //state changed
			currentState = state;
            //eventHold = false;
            holdFlag = false;
			stateChanged = true;

			if (isHigh()) {
				holdDelay = HOLD_DELAY;
#ifdef DEBUG_PORT
                Serial.println(" pressed");
#endif
			} else {
#ifdef DEBUG_PORT
                Serial.println("released");
#endif
				holdDelay = 0;
				holdFlag = false;
				disable(DISABLE_DELAY);
			}
		}
	}

	uint8_t getState() { return digitalRead(pin); }

	uint8_t pin;
	
protected:
	bool highState; //HIGH == pressed (1) or LOW == pressed (0)

	bool currentState;
	bool stateChanged;
	bool holdFlag;

	uint8_t debounceDelay;
	uint8_t holdDelay;
	uint8_t disableDelay;
};