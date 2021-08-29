namespace Device{
	class Debug : public ModuleD{
	public:
		Debug(uint8_t id) : ModuleD(id, INPUT) {
			debouncedFlag = false;
		}
		void listen() {
			if (!isHigh())
				return;
			
			if (debouncedFlag) {
				debouncedFlag = false;
				event(EVENT_BUTTON_HOLD);
#ifdef DEBUG_PORT
				Serial.print("pin (");
				Serial.print(id);
				Serial.print(":");
				Serial.print(getPin());
				Serial.println(") high");
#endif
			} else {
				debouncedFlag = true;
			}
		}
	protected:
		bool debouncedFlag;
	};
}