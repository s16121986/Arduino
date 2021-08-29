#define STATE_AVAILABLE     1 // 001
#define STATE_ENABLED       2 // 010
#define STATE_ON            4 // 100


namespace Device{
	class Module;

	class Pin{
	public:
		Pin();
		Pin(uint8_t pin);
		bool isEmpty();
        bool isEnabled();
        void state();
        void event(uint8_t e, uint8_t data = 0);
        void setPin(uint8_t pin);
        //Pin &operator+=( uint8_t value ) { this->pinState |= value;send();return *this; }
        /*Pin &operator|=( uint8_t value ) {
        	setState(this->pinState | value);
        	return *this;
        }
        //State &operator-=( uint8_t value ) { this->pinState ^= value;send();return *this; }
        Pin &operator^=( uint8_t value ) {
        	if (pinState & value)
        		setState(this->pinState ^= value);
        	return *this;
        }
        bool operator==( uint8_t value ) { return this->pinState & value; }
        bool operator!=( uint8_t value ) { return ~this->pinState & value; }*/
        void setState(uint8_t state);
        void setData(uint8_t data);
        void enable();
        void disable();
        void high();
        void low();
        void unbindModule();
        void bindModule(Module &m);
        bool isHigh();
        bool isLow();

		uint8_t pin;
		uint8_t pinState;
		uint8_t data;
		Module *module;
	};
}