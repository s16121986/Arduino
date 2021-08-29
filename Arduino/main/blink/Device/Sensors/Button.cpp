namespace Device{
	class Button : public ModuleD {
	public:
		Button() : ModuleD() {}
		Button(uint8_t id) : ModuleD(id, INPUT) {
			currentState = LOW;
			pressedState = HIGH;

			currentPressTime = 0;
			debounceStartTime = 0;
			debounceDelay = 60;
			holdDelay = 1200;

			holdFlag = false;
			eventHold = false;
			eventPressed = false;
			eventReleased = false;
		}
		bool onPress() {
			if (!eventPressed)
				return false;
			eventPressed = false;
			return !event(EVENT_BUTTON_PRESS);
		}
		/*bool onRelease() {
			if (!eventReleased)
				return false;
			eventReleased = false;
			return !event(EVENT_BUTTON_RELEASE);
		}*/
		bool onHold()  {
			if (!eventHold)
				return false;
			eventHold = false;
			currentPressTime = 0;
			return !event(EVENT_BUTTON_HOLD);
		}
		bool onEvents () {
			return onPress() || onHold();
			//onRelease();
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
			if (!isEnabled() || eventPressed || eventHold)
				return;

			bool state = getState();

			if (currentState == state) {
				debounceStartTime = 0;
				if (currentPressTime > 0 && (CURRENT_TIME - currentPressTime) > holdDelay) {
					holdFlag = true;
					eventHold = true;
					currentPressTime = 0;
				}
			} else if (debounce()) { //state changed
				currentState = state;
                eventHold = false;
                holdFlag = false;

				if (isPressed()) {
					currentPressTime = CURRENT_TIME;
#ifdef DEBUG_PORT
                    printUniqueId();
                    Serial.println(" pressed");
#endif
				} else { // is released
					eventPressed = true;
                    currentPressTime = 0;
				}
			}
		}
#ifdef DEBUG_PORT
		void printName() {
			Serial.print("Button");
		}
#endif
	protected:
		bool pressedState; //HIGH == pressed (1) or LOW == pressed (0)
		bool currentState;

		unsigned long int currentPressTime;
		unsigned long int debounceStartTime;
		unsigned int debounceDelay;
		unsigned int holdDelay;

		bool holdFlag;
		bool eventHold;
		bool eventPressed;
	};
}