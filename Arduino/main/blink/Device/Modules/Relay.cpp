namespace Device{
	class Relay : public ModuleD{
	public:
		Relay(uint8_t id): ModuleD(id, OUTPUT) {
			currentState = !highState;
		}
		void disable() {
			Module::disable();
			off();
		}
		void on(unsigned long msec = 0) {
			if (!isEnabled()) return;
			currentState = highState;
			offTimer.toggle(msec);
			setState(currentState);
			state();
		}
		void off() {
			currentState = !highState;
			offTimer.stop();
			setState(currentState);
			state();
		}
		void toggle() {
			if (currentState == highState)
				off();
			else
				on();
		}
		void listen() {
			if (offTimer.onExpire())
				off();
		}
		bool isOn() { return isHigh(); }
		bool isOff() { return !isHigh(); }
		unsigned long getTimeout() { return offTimer.getTimeout(); }
		void trigger(uint8_t action, uint8_t data = 0) {
			switch (action) {
				case C_ON: on(data * 1000); break;
				case C_OFF: off(); break;
				case C_TOGGLE: toggle(); break;
				default: Module::trigger(action, data);
			}
		}
#ifdef DEBUG_PORT
		void printName() {
			Serial.print("Relay");
		}
#endif
	protected:
		Timeout offTimer;
		bool currentState;
	};
}