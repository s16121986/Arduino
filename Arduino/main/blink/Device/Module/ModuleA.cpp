namespace Device{
	class ModuleA : public Module{
	protected:
	public:
		PinA A0;
		ModuleA() : Module () {}
		ModuleA(uint8_t id, uint8_t mode) : Module(id, mode) {
			A0.init(id, mode);
		};
		void write(int data) { A0.write(data); }
		uint16_t read() { return A0.read(); }
	};
}