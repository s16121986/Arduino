

namespace Climatic{
	//uint16_t sensorsRegisters[] = {0, 0, 0, 0};
	//DHT dht(DHTPIN, DHT11);
	Adafruit_BME280 bme;

	enum SENSORS_REGISTERS{
		TEMPERATURE,
		HUMIDITY,
		CARBON_DIOXIDE,
		PRESSURE,
		DUST,
		//
		HOLDING_REGISTERS_SIZE
	};

	void setup();
	void listen();
}