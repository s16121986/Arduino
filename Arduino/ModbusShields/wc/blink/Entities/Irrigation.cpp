
namespace Irrigation{
	bool state = false;

	void rotate() {

	}

	void start() {
		digitalWrite(PIN_FL_IRRIGATION, HIGH);
	}

	void stop() {
		digitalWrite(PIN_FL_IRRIGATION, LOW);
	}
}