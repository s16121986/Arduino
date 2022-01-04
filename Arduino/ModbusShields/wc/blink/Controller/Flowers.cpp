#include "../Entities/Irrigation.cpp"

namespace Flowers{
	uint16_t IRRIGATE_DELAY = 12;
	uint8_t LIGHT_BRIGHTNESS = 255;

	bool busyFlag = false;
	bool lightState = false;

	void irrigate() {
		if (busyFlag)
			return;

#ifdef DEBUG_PORT
        Serial.print("irrigation (");
        Serial.print(IRRIGATE_DELAY);
        Serial.println(")");
#endif
    	busyFlag = true;
    	Irrigation::start();
        delay(IRRIGATE_DELAY);
		Irrigation::stop();

		busyFlag = false;
        //reset(RESET_TIMEOUT);
	}

	void irrigate(bool flag) {
		if (flag)
    	    irrigate();
    	else
    	    Irrigation::stop();
	}

	uint16_t getSoilHumidity() {
		return 0;
	}

	void lightOn() {
		lightState = true;
		if (LIGHT_BRIGHTNESS >= 255)
			digitalWrite(PIN_FL_LIGHT, HIGH);
		else
			analogWrite(LIGHT_BRIGHTNESS, LIGHT_BRIGHTNESS);
	}

	void lightOff() {
		lightState = false;
		digitalWrite(PIN_FL_LIGHT, LOW);
	}

	void light(bool flag) {
		if (flag)
			lightOn();
		else
			lightOff();
	}

	void setBrightness(uint16_t b) {
		if (b > 255)
			LIGHT_BRIGHTNESS = 255;
		else if (b == 0)
			LIGHT_BRIGHTNESS = 1;
		else
			LIGHT_BRIGHTNESS = b;

		if (lightState)
			lightOn();
	}
}