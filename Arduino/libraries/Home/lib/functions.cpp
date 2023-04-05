void triacHigh(int pin, unsigned int offDelay) {
	delayMicroseconds(offDelay);
	digitalWrite(pin, HIGH); // triac firing
	delayMicroseconds(10); // triac On propogation delay
	digitalWrite(pin, LOW); // triac Off
	//Симистор выключится по достижении нуля!
}

int pinToInterrupt(int pin) {
	switch (pin) {
		case 2:return 0;
		case 3:return 1;
		case 21:return 2;
		case 20:return 3;
		case 19:return 4;
		case 18:return 5;
	}
	return 0;
}

void emptyFn() {}