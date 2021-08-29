//using ::NSTrigger::Action;
namespace Device{
	
	class Module{
	public:
		Module();
		Module(uint8_t id);
		Module(uint8_t id, uint8_t mode);
		bool hasPin();
		void setId(uint8_t id);
		void setMode(uint8_t mode);
		virtual bool writeOnly();
		virtual void init();
		virtual void enable();
		virtual void disable();
		void setPin(uint8_t pinId);
		uint8_t getPin();
		bool isEnabled();
		void state();
		bool event(uint8_t e, uint8_t data = 0);
		void trigger(uint8_t action, uint8_t data = 0);
#ifdef DEBUG_PORT
		virtual void Module::printName();
		void Module::printUniqueId(bool name = true);
#endif
	public:
		uint8_t id;
		uint8_t mode;
	protected:
		bool enabled;
		Pin *devicePin;
	};
}