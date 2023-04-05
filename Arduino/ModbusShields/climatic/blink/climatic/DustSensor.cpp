/*
Sharp Optical Dust Sensor GP2Y1010AU0F
*/
namespace DustSensor{
    float void read(){
		digitalWrite(PIN_DUST_LED, LOW); // power on the LED
		delayMicroseconds(280);

		float voMeasured = analogRead(PIN_DUST_MEASURE); // read the dust value

		delayMicroseconds(40);
		digitalWrite(PIN_DUST_LED, HIGH); // turn the LED off

		// 0 - 5V mapped to 0 - 1023 integer values
		// recover voltage
		float calcVoltage = voMeasured * (5.0 / 1024.0);

		// linear eqaution taken from http://www.howmuchsnow.com/arduino/airquality/
		// Chris Nafis (c) 2012
		float dustDensity = 170 * calcVoltage - 0.1;

		return dustDensity; // unit: ug/m3
    }
}