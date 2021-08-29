namespace Device{
	class PWM : public ModuleD{
	public:
		PWM() {}
		PWM(int id) : ModuleD(id, OUTPUT) {
			dimmingIncrement = 1;
			brightness = PWM_MAX_VALUE;
			flagOn = false;
			
			delayBrightness = 0;
			lastBrightness = 0;
			brightnessDelayTime = 0;
			dimmingFlag = false;
			setPWMMode(true);
		}
		void init() {
			ModuleD::init();
			(*devicePin).setData(brightness);
		}
		bool isOn() { return flagOn; }
		void listen() {
			if (offTimer.onExpire())
				off();
#ifdef PWM_DELAY
			if (brightnessDelayTime > 0) {
				if (CURRENT_TIME - brightnessDelayTime >= PWM_DELAY) {
					//Serial.println(delayBrightness);
					write(delayBrightness);
					brightnessDelayTime = 0;
	#ifdef DEBUG_PORT
			Serial.print("pwm[]");
			Serial.print("set ");
			Serial.print("pwm set ");
			Serial.println(delayBrightness);
	#endif
				} else {
	#ifdef DEBUG_PORT
			Serial.print("pwm inc ");
			Serial.println(lastBrightness + (delayBrightness - lastBrightness) * (float)(CURRENT_TIME - brightnessDelayTime) / PWM_DELAY);
	#endif
					//Serial.println(lastBrightness + (delayBrightness - lastBrightness) * (float)(currentTime - brightnessDelayTime) / brightnessDelay);
					write(lastBrightness + (delayBrightness - lastBrightness) * (float)(CURRENT_TIME - brightnessDelayTime) / PWM_DELAY);
				}
			}
#endif
			/* else if (dimmingFlag) {
				listenBrightness += dimmingIncrement * 5;
				if (listenBrightness <= 0) {
					listenBrightness = 0;
					dimmingIncrement = 1;
				} else if (listenBrightness >= PWM_MAX_VALUE) {
					listenBrightness = PWM_MAX_VALUE;
					dimmingIncrement = -1;
				}
				write(listenBrightness);
			} */
		}
		void on(uint8_t brightness, unsigned long msec = 0) {
			if (!isEnabled())
				return;
			if (brightness == 0) { //off
				if (!isOn())
					return;
				offTimer.stop();
				//lastBrightness = this->brightness;
				flagOn = false;
				//delayBrightness = 0;
				write(0);
			} else { //on
				offTimer.toggle(msec);
				if (isOn()) {
					if (this->brightness == brightness)
						return;
					//lastBrightness = this->brightness;
				} else {
					//lastBrightness = 0;
					flagOn = true;
				}
				//delayBrightness = brightness;
				setBrightness(brightness);
				write(brightness);
				//brightnessDelayTime = CURRENT_TIME;
			}
			if (isOn()) (*devicePin).high(); else (*devicePin).low();
			state();
	#ifdef DEBUG_PORT
			printUniqueId();
			Serial.print(".brightness=");
			Serial.println(brightness);
			//Serial.print(speed);
	#endif
		}
		void on() { on(brightness); }
		void off() { on(0); }
		void toggle() { if (isOn()) off(); else on(); }
		void setBrightness(uint8_t brightness) {
			//Если включено и яркость не настроена вручную обновляем освещение
			//if (isOn()) on(brightness); // else lastBrightness = brightness;
			this->brightness = brightness;
			(*devicePin).setData(brightness);
		}
		uint8_t getBrightness() { return brightness; }
		void setDimming(bool flag) {
			dimmingFlag = flag;
			if (flag) {
				lastBrightness = brightness;
			}
			brightnessDelayTime = 0;
		}
		bool isDimming() { return dimmingFlag; }
		/*void setEEPROMAddress(byte addr) {
			EEPROMAddress = addr;
			brightnessCache = EEPROM.read(EEPROMAddress);
		}*/
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
			Serial.print("Pwm");
		}
#endif
	protected:
		uint8_t brightness;                //Текущее значене яркости
		bool flagOn;
		uint8_t lastBrightness;
		uint8_t delayBrightness;                //Текущее значене яркости
		
		unsigned long int brightnessDelayTime;

		Timeout offTimer;
		bool dimmingFlag;
		int8_t dimmingIncrement;
	};
}
