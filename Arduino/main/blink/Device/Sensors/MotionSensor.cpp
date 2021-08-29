namespace Device{
	class MotionSensor : public ModuleD{
	public:
		MotionSensor(int pin): ModuleD(pin, INPUT) {
			stateFlag = false;
			disableDelay = 0;
		}
		void listen() {
			if (disableDelay > 0)
			    disableDelay--;
			else if (!isEnabled() || stateFlag || !isHigh())
				return;
			else {
				stateFlag = true;
				disableDelay = 250; //250x40ms = 10s
			}
		}
		bool onHigh() {
			if (stateFlag && isHigh()) {
				stateFlag = false;
				return !event(EVENT_MOTION_DETECT);
			}
			return false;
		}
	private:
		bool stateFlag;
		uint8_t disableDelay; // таймер времени включения
	};
}
