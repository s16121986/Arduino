
namespace Device{
	Pin::Pin(){
        pin = 0;
        pinState = STATE_AVAILABLE;
        data = 0;
        module = 0;
    }

	Pin::Pin(uint8_t pin) : Pin::Pin() {
		setPin(pin);
	}

	bool Pin::isEmpty() { return this->pin == 0; }

	bool Pin::isEnabled() { return !isEmpty() && (this->pinState & STATE_ENABLED); }

	void Pin::state() {
		if (!isEmpty())
			sendState(pin, pinState, data);
	}

	void Pin::event(uint8_t e, uint8_t data = 0) {
		if (!isEmpty())
			sendEvent(pin, e, data);
	}

	void Pin::setPin(uint8_t pin) { this->pin = pin; }

	void Pin::setState(uint8_t state) {
		if (this->pinState == state)
			return;
		this->pinState = state;
	}

	void Pin::setData(uint8_t data) { this->data = data; }

	void Pin::enable() { setState(pinState | 3); }

	void Pin::disable() { setState(STATE_AVAILABLE); }

	void Pin::high() { setState(7); }

	void Pin::low() {
		if (isHigh())
			setState(3);
	}

	bool Pin::isHigh() { return this->pinState & STATE_ON; }

	bool Pin::isLow() { return !isHigh(); }

	void Pin::unbindModule() {
		if (module != 0)
			module = 0;
	}

	void Pin::bindModule(Module &m) {
		if (module != 0)
			(*module).setPin(0);
		module = &m;
	}

	//void Pin::test();
}
