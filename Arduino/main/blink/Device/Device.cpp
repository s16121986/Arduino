
#include "Pin/Pin.h"
#include "Module/Module.h"
#include "Device.h"

#include "Pin/functions.cpp"
#include "Pin/Pin.cpp"

#ifdef DEBUG_MODE
#include "Pins/uno.h"
#else
#include "Pins/mega.h"
#endif

#include "Module/Module.cpp"
#include "Module/ModuleD.cpp"
#include "Module/ModuleA.cpp"
#include "Modules/Relay.cpp"
#include "Modules/PWM.cpp"
#include "Modules/Ping.cpp"
#include "Modules/PWMD.cpp"
#include "Sensors/Button.cpp"
#include "Sensors/Sensor.cpp"
#include "Sensors/DoorSensor.cpp"
#include "Sensors/MotionSensor.cpp"
#include "Sensors/LeakSensor.cpp"

#include "../Rooms/Storage.cpp"
#include "../Rooms/Hallway.cpp"
#include "../Rooms/Bathroom.cpp"
#include "../Rooms/Kitchen.cpp"
#include "../Rooms/Balcony.cpp"
#include "../Rooms/Bedroom.cpp"
	
namespace Device{
	using namespace Room;
	
	bool hasPin(uint8_t pin) {
		for (uint8_t i = 0; i < _pins_count; i++) {
			if (pins[i].pin == pin)
				return true;
		}
		return false;
	}
	
	Pin& getPin(uint8_t pin) {
		for (uint8_t i = 0; i < _pins_count; i++) {
			if (pins[i].pin == pin)
				return pins[i];
		}
	}
	
	void setup() {
		delay(1000);
		Device::sendEvent(T_HOME, EVENT_INIT); //pins request
	}

	void listen() {
		ST::listen();
		HL::listen();
		BT::listen();
		KH::listen();
		BL::listen();
		BD::listen();
	}

	void loop() {
		ST::loop();
		HL::loop();
		BT::loop();
		KH::loop();
		BL::loop();
		BD::loop();
	}

	void trigger(uint8_t target, uint8_t action, uint8_t data) {
		ST::trigger(target, action, data);
		HL::trigger(target, action, data);
		BT::trigger(target, action, data);
		KH::trigger(target, action, data);
		BL::trigger(target, action, data);
		BD::trigger(target, action, data);
	}
	
	void state() {
		for (uint8_t i = 0; i < _pins_count; i++) {
			if (pins[i].isEnabled())
				pins[i].state();
		}
	}
	
	void debug(uint8_t action, uint8_t data) {
		switch (data) {
			case 0:
				uint8_t buf[5];
				buf[0] = IO_STATE_BIT;
				buf[1] = 17;
				buf[2] = 4;
				buf[3] = 19;
				buf[4] = IO_STOP_BIT;
				Serial.write(buf, 5);
				break;
		}
	}
	
	
}