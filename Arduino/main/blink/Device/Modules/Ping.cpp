namespace Device{
	class Ping : public ModuleD{
	public:
		Ping(uint8_t id): ModuleD(id, OUTPUT) {
			pwmFlag = false;
			pwmCounter = 0;
			pwmTime = 0;
			pwmDelay = 1;
		}
		void signal(uint8_t code) {
			if (pwmCounter)
				return;
			
#ifdef DEBUG_PORT
			printUniqueId();
			Serial.println(code);
#endif
			pwmFlag = true;
			pwmCounter = code;
			pwmTime = millis();
			D0.write(HIGH);
		}
		void on() { signal(1); }
		void off() { signal (2); }
		void loop() {
			if (0 == pwmTime || (millis() - pwmDelay) <= pwmTime)
				return;
			
			if (pwmFlag) {
				pwmFlag = false;
				D0.write(LOW);
#ifdef DEBUG_PORT
//				Serial.print("ping ");
//				Serial.println(pwmCounter);
#endif
				pwmCounter--;
				if (0 == pwmCounter) {
					pwmTime = 0;
					return;
				}
			} else {
				pwmFlag = true;
				D0.write(HIGH);
			}
			
			pwmTime = millis();
		}
		void trigger(uint8_t action, uint8_t data = 0) {
			switch (action) {
				case C_ON: on(); break;
				case C_OFF: off(); break;
				default: Module::trigger(action, data);
			}
		}
#ifdef DEBUG_PORT
		void printName() {
			Serial.print("Ping");
		}
#endif
	protected:		
		uint8_t pwmDelay;
		bool pwmFlag;
		uint8_t pwmCounter;
		unsigned long int pwmTime;
	};
}