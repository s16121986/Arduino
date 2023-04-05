#include "../Entities/Irrigation.cpp"

namespace Flowers{
	uint8_t LIGHT_BRIGHTNESS = 255;

	bool busyFlag = false;
	bool lightState = false;

	void irrigate(uint16_t timeout) {
		if (busyFlag)
    		return;

    #ifdef DEBUG_PORT
        Serial.print("irrigation (");
        Serial.print(timeout);
        Serial.println(")");
    #endif
        busyFlag = true;
        Irrigation::start();
        delay(timeout);
        Irrigation::stop();

        busyFlag = false;
	}

	uint16_t getSoilHumidity() {
		return 0;
	}

	void lightOn() {
		lightState = true;
		if (LIGHT_BRIGHTNESS >= 255)
			digitalWrite(PIN_FL_LIGHT, HIGH);
		else
			analogWrite(PIN_FL_LIGHT, LIGHT_BRIGHTNESS);
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