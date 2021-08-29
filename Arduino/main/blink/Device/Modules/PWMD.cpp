namespace Device{
	class PWMD : public Ping{
	public:
		PWMD(int id) : Ping(id) {
			brightness = PWM_MAX_VALUE;
		}
		void init() {
			ModuleD::init();
			(*devicePin).setData(brightness);
		}
		bool isOn() { return flagOn; }
		void signal(uint8_t code) {
			if (pwmCounter)
				return;
			
			pwmFlag = true;
			pwmCounter = code;
			pwmTime = millis() + 10; // long first signal to zerocross delay fix
			D0.write(HIGH);
		}
		void on(uint8_t brightness, unsigned long msec = 0) {
			if (!isEnabled())
				return;
			
			if (brightness == 0) { //off
				//if (!isOn())
				//	return;
				flagOn = false;
				signal(1);
				offTimer.stop();
			} else { //on
				if (isOn()) {
					//if (this->brightness == brightness)
						//return;
				} else {
					flagOn = true;
				}
				setBrightness(brightness);
				signal(1 + map(brightness, 0, PWM_MAX_VALUE, 0, 20));
				offTimer.toggle(msec);
			}
			
			if (isOn()) (*devicePin).high(); else (*devicePin).low();
			state();
			
	#ifdef DEBUG_PORT
			printUniqueId();
			Serial.print(".brightness=");
			Serial.println(brightness);
	#endif
		}
		void on() { on(brightness); }
		void off() { on(0); }
		void toggle() { if (isOn()) off(); else on(); }
		void setBrightness(uint8_t brightness) {
			this->brightness = brightness;
			(*devicePin).setData(brightness);
		}
		uint8_t getBrightness() { return brightness; }
		void listen() {
			if (offTimer.onExpire())
				off();
		}
		void trigger(uint8_t action, uint8_t data = 0) {
			switch (action) {
				case C_ON: on(brightness, data * 1000); break;
				case C_OFF: off(); break;
				case C_TOGGLE: toggle(); break;
				case C_PWM: on(data); break;
				default: Module::trigger(action, data);
			}
		}
#ifdef DEBUG_PORT
		void printName() {
			Serial.print("PwmD");
		}
#endif
	protected:
		uint8_t brightness;                //Текущее значене яркости
		bool flagOn;
		Timeout offTimer;
	};
}
