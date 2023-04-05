class PinD{
public:
	PinD() { pwmMode = false; }
	void init(uint8_t id, uint8_t mode) {
		this->id = id;
		this->mode = mode;
#ifdef PINOUT_UNO
		if (id <= 7) {
			_preg(id, PORTD, PIND, DDRD);
		} else if (id <= 13) {
			_preg((id - 8), PORTB, PINB, DDRB);
		}
#elif PINOUT_MEGA
		switch (id) {
			case 0:_preg(0, PORTE, PINE, DDRE);break;
			case 1:_preg(1, PORTE, PINE, DDRE);break;
			case 2:_preg(4, PORTE, PINE, DDRE);break;
			case 3:_preg(5, PORTE, PINE, DDRE);break;
			case 4:_preg(5, PORTG, PING, DDRG);break;
			case 5:_preg(3, PORTE, PINE, DDRE);break;
			case 6:
			case 7:
			case 8:
			case 9:_preg(id - 3, PORTH, PINH, DDRH);break;
			case 10:
			case 11:
			case 12:
			case 13:_preg(id - 6, PORTB, PINB, DDRB);break;
			case 14:_preg(1, PORTJ, PINJ, DDRJ);break;
			case 15:_preg(0, PORTJ, PINJ, DDRJ);break;
			case 16:_preg(1, PORTH, PINH, DDRH);break;
			case 17:_preg(0, PORTH, PINH, DDRH);break;
			case 18:_preg(3, PORTD, PIND, DDRD);break;
			case 19:_preg(2, PORTD, PIND, DDRD);break;
			case 20:_preg(1, PORTD, PIND, DDRD);break;
			case 21:_preg(0, PORTD, PIND, DDRD);break;
			case 22:
			case 23:
			case 24:
			case 25:
			case 26:
			case 27:
			case 28:
			case 29:_preg(37 - id, PORTB, PINB, DDRB);break;
			case 30:
			case 31:
			case 32:
			case 33:
			case 34:
			case 35:
			case 36:
			case 37:_preg(4, PORTB, PINB, DDRB);break;
			case 38:_preg(7, PORTD, PIND, DDRD);break;
			case 39:
			case 40:
			case 41:_preg(41 - id, PORTB, PINB, DDRB);break;
			case 42:
			case 43:
			case 44:
			case 45:
			case 46:
			case 47:
			case 48:
			case 49:_preg(49 - id, PORTL, PINL, DDRL);break;
			case 50:
			case 51:
			case 52:
			case 53:_preg(53 - id, PORTB, PINB, DDRB);break;
		}
#else
		pinMode(id, mode);
#endif
		
		/*if (id <= 7) {
			poud = &PORTD;
			pind = &PIND;
			shift = id;
			switch (id) {
				case 0:pin = 1;break;   //B00000001
				case 1:pin = 2;break;   //B00000010
				case 2:pin = 4;break;   //B00000100
				case 3:pin = 8;break;   //B00001000
				case 4:pin = 16;break;  //B00010000
				case 5:pin = 32;break;  //B00100000
				case 6:pin = 64;break;  //B01000000
				case 7:pin = 128;break; //B10000000
			}
			if (mode == OUTPUT) DDRD |= pin;
		} else if (id <= 13) {
			poud = &PORTB;
			pind = &PINB;
			switch (id) {
				case 8:pin = 1;break;    //B00000001
				case 9:pin = 2;break;    //B00000010
				case 10:pin = 4;break;   //B00000100
				case 11:pin = 8;break;   //B00001000
				case 12:pin = 16;break;  //B00010000
				case 13:pin = 32;break;  //B00100000
			}
			if (mode == OUTPUT) DDRB |= pin;
		}*/
		state = LOW;
	}
	void unset() {
		id = 0;
	}
	uint16_t read() {
		if (mode == INPUT) {
			if (pwmMode) {
				state = analogRead(id);
			} else {
#if defined (PINOUT_UNO) || defined(PINOUT_MEGA)
				state = (*pind & pin ? HIGH : LOW)
#else
				state = digitalRead(id);
#endif
			}
		}
		return state;
	}
	void write(uint8_t s) {
/*#ifdef DEBUG_PORT
		Serial.print("pin (");
		Serial.print(id);
		Serial.print(") write ");
		Serial.println(s);
#endif*/
		if (mode == OUTPUT) {
			if (pwmMode) {
#if defined (PINOUT_UNO) || defined(PINOUT_MEGA)
				if (s == 0) *poud &= ~pin;
				else if (s >= PWM_MAX_VALUE) *poud |= pin;
#else
				if (s == 0) digitalWrite(id, LOW);
				else if (s >= PWM_MAX_VALUE) digitalWrite(id, HIGH);
#endif
				else analogWrite(id, s);
			} else {
				state = s;
#if defined (PINOUT_UNO) || defined(PINOUT_MEGA)
				if (state == HIGH) *poud |= pin; else *poud &= ~pin;
#else
				digitalWrite(id, state);
#endif
			}
		}
	}
	void setPWMMode(bool flag) { pwmMode = flag; }
private:
#if defined (PINOUT_UNO) || defined(PINOUT_MEGA)
	void _preg(uint8_t portPin, uint8_t volatile &poud, volatile uint8_t &pind, volatile uint8_t &pmode) {
		this->pin = 1<<portPin;
		this->poud = &poud;
		this->pind = &pind;
		//this->pmode = &pmode;
		if (mode == OUTPUT) pmode |= this->pin;
	}
	volatile uint8_t *pmode;
	volatile uint8_t *pind;
	volatile uint8_t *poud;
	uint8_t pin;
#endif
	bool pwmMode;
	uint8_t id;
	uint8_t mode;
	uint8_t state;
};