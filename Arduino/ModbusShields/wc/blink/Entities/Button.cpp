class Button {
public:
	Button(unsigned char _id) {
		id = _id;
		pinMode(id, INPUT);
		currentState = LOW;
		pressedState = HIGH;

		debounceDelay = 60;
		holdDelay = 1200;

		reset();
	}
	bool getState() { return digitalRead(id); }
	bool onPress() {
		if (!eventPressed)
			return false;
		eventPressed = false;
		return true;
	}
	bool onRelease() {
		if (!eventReleased)
			return false;
		eventReleased = false;
		return true;
	}
	bool onHold()  {
		if (!eventHold)
			return false;
		eventHold = false;
		currentPressTime = 0;
		return true;
	}
	bool debounce() {
		if (debounceStartTime == 0) {
			debounceStartTime = CURRENT_TIME;
			return false;
		} else if ((CURRENT_TIME - debounceStartTime) < debounceDelay)
			return false;

		debounceStartTime = 0;

		return true;
	}
	bool isPressed () { return currentState == pressedState; }
	bool isReleased () { return currentState != pressedState; }
	bool isHold () { return holdFlag; }
	void listen() {
		bool state = getState();

		if (currentState == state) {
			debounceStartTime = 0;
			if (!holdFlag && (currentPressTime > 0 && (CURRENT_TIME - currentPressTime) > holdDelay)) {
				holdFlag = true;
				eventHold = true;
				currentPressTime = 0;
			}
		} else if (debounce()) { //state changed
			currentState = state;

            eventPressed = eventPressed || isPressed();
            eventReleased = eventReleased || isReleased();
            eventHold = false;
            holdFlag = false;

            if (eventPressed) {
                currentPressTime = CURRENT_TIME;
#ifdef DEBUG_PORT
				Serial.println("btn pressed");
#endif
            } else {
                currentPressTime = 0;
            }
		}
	}
	void reset() {
		currentPressTime = 0;
		debounceStartTime = 0;
		holdFlag = false;
		eventHold = false;
		eventPressed = false;
		eventReleased = false;
	}
protected:
	unsigned char id;
	bool pressedState; //HIGH == pressed (1) or LOW == pressed (0)
	bool currentState;

	unsigned long int currentPressTime;
	unsigned long int debounceStartTime;
	unsigned int debounceDelay;
	unsigned int holdDelay;

	bool holdFlag;
	bool eventHold;
	bool eventPressed;
	bool eventReleased;
};