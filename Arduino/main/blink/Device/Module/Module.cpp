//using ::NSTrigger::Action;
namespace Device{

	Module::Module() {
		id = 0;
		devicePin = 0;
		enabled = false;
	}

	Module::Module(uint8_t id) : Module::Module() { setId(id); }

	Module::Module(uint8_t id, uint8_t mode) : Module::Module(id) {
		setMode(mode);
	}

	bool Module::hasPin() { return devicePin != 0; }

	void Module::setId(uint8_t id) { this->id = id; }

	void Module::setMode(uint8_t mode) { this->mode = mode; }

	bool Module::writeOnly() { return false; }

	void Module::init() {}

	void Module::enable() { enabled = hasPin(); }

	void Module::disable() { enabled = false; }

	void Module::setPin(uint8_t pinId) {
#ifdef DEBUG_PORT
		printUniqueId();
		Serial.print(".setPin(");
		Serial.print(pinId);
		Serial.print("): ");
#endif
		if (pinId == 0) {
			if (!hasPin())
				return;
			disable();
			(*devicePin).unbindModule();
			devicePin = 0;
		} else {
			if (!Device::hasPin(pinId)) {
#ifdef DEBUG_PORT
				Serial.println("pin invalid");
#endif
				return;
			} else if (hasPin() && pinId == (*devicePin).pin) {
#ifdef DEBUG_PORT
				Serial.println("pin already set");
#endif
				return;
			}
			devicePin = &Device::getPin(pinId);
			init();
			(*devicePin).enable();
			(*devicePin).bindModule(this);
			enabled = true;
		}
#ifdef DEBUG_PORT
		Serial.println("success");
#endif
	}

	uint8_t Module::getPin() {
		if (0 == devicePin)
			return 0;
		else
			return (*devicePin).pin;
	}

	bool Module::isEnabled() { return enabled && (*devicePin).isEnabled(); }

	void Module::state() {
#ifdef DEBUG_PORT
		printUniqueId();
		Serial.println(".state();");
#endif
		if (hasPin())
			(*devicePin).state();
	}
	bool Module::event(uint8_t e, uint8_t data = 0) {
#ifdef DEBUG_PORT
		printUniqueId();
		Serial.print(".event(");
		Serial.print(e);
		Serial.print(", ");
		Serial.print(data);
		Serial.println(");");
		return false;
#endif
		if (!hasPin()) // || !::Config::DEVICE_EVENTS
			return false;
		(*devicePin).event(e, data);
		return true;
	}

#ifdef DEBUG_PORT
	void Module::printName() {
		Serial.print("Module");
	}
	void Module::printUniqueId(bool name = true) {
		if (name)
			printName();
		Serial.print("[id=");
		Serial.print(id);
		Serial.print(";pin=");
		Serial.print(getPin());
		Serial.print("]");
	}
#endif

	void Module::trigger(uint8_t action, uint8_t data = 0) {
#ifdef DEBUG_PORT
		printUniqueId();
		Serial.print(".trigger(");
		Serial.print(action);
		Serial.print(", ");
		Serial.print(data);
		Serial.println(");");
#endif
		switch (action) {
			case C_SETPIN:
				setPin(data);
				state();
				break;
			case C_STATE:
				state();
				break;
			case C_PING:
				Device::sendPing(id, hasPin());
				break;
		}
	}
}