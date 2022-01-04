#define ANGLE_DOWN 400
#define ANGLE_UP 1600

namespace Servo{
	bool upFlag = false;

	void rotate(unsigned short int angle) {
    	for (unsigned char i = 0; i < 40; i++) {
    	    digitalWrite(PIN_WC_SERVO, HIGH);
    		delayMicroseconds(angle);
    	    digitalWrite(PIN_WC_SERVO, LOW);
    		delay(15);
    	}
	}
	void down() {
#ifdef DEBUG_PORT
		Serial.println("servo down");
#endif
		rotate(ANGLE_DOWN);
		upFlag = false;
		//digitalWrite(PIN_TEST, LOW);
	}
	void up() {
		//digitalWrite(PIN_TEST, HIGH);
#ifdef DEBUG_PORT
		Serial.println("servo up");
#endif
		upFlag = true;
		rotate(ANGLE_UP);
	}
}