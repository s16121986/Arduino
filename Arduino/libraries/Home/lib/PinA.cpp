class PinA{
public:
	PinA() {}
	PinA(uint8_t id) { init(id); }
	void init(uint8_t id, uint8_t mode = 0) {
		this->id = id;
		pinMode(id, mode);
		/*if (argPin <= 13) {
			poud = &PORTC;
			pind = &PINC;
			switch (argPin) {
				case 0:pin = 1;break;   //B00000001
				case 1:pin = 2;break;   //B00000010
				case 2:pin = 4;break;   //B00000100
				case 3:pin = 8;break;   //B00001000
				case 4:pin = 16;break;  //B00010000
				case 5:pin = 32;break;  //B00100000
			}
			if (mode == OUTPUT) DDRC |= pin;
		}*/
	}
	uint16_t read()  { return analogRead(id); }
	void write(int data) { analogWrite(id, data); }
private:
	uint8_t id;
};