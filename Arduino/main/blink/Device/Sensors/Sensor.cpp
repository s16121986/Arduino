namespace Device{
	class Sensor : public ModuleD{
	public:
		Sensor(int pin): ModuleD(pin, INPUT) {
			lastState = 0;
			currentState = 0;
			changed = false;
		}
		void listen() {
			if (!isEnabled())
				return;
			currentState = getState();
			if (currentState != lastState) {
				changed = true;
				lastState = currentState;
			}
		}
		bool onChange() {
			if (changed) {
				changed = false;
				return !event(EVENT_SENSOR_STATE, isHigh());
			}
			return false;
		}
	protected:
		bool lastState;
		bool currentState;
		bool changed;
	};
}