namespace Device{
	class ModuleD : public Module{
	public:
		PinD D0;
		ModuleD() : Module() {}
		ModuleD(uint8_t id, uint8_t mode) : Module(id, mode) {
			highState = HIGH;
		};
		virtual void init() {
			if (hasPin())
				D0.init(getPin(), mode);
		}
		uint8_t getState() { return isEnabled() ? D0.read() : !highState; }
		virtual void setState(uint8_t s) {
			if (getState() == s || !isEnabled())
				return;
			D0.write(s);
			if (isHigh())
				(*devicePin).high();
			else
				(*devicePin).low();
		}
		void setData(uint8_t data) {
			write(data);
			(*devicePin).setData(data);
		}
		bool isHigh() { return getState() == highState; }
		bool isLow() { return getState() != highState; }
		void write(uint8_t data) { D0.write(data); }
		void setPWMMode(bool flag) { D0.setPWMMode(flag); }
		void high() { setState(highState); }
		void low() { setState(!highState); }
	protected:
		bool highState;
	};
}