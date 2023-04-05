namespace Modbus{ namespace Coils{
	//using namespace WC;

	enum REGISTERS{
        WC_ENABLED,
        WC_HALF_FLUSH,
        WC_FULL_FLUSH,
        FLOWERS_LIGHT,

        REGISTERS_SIZE
    };

	void setup() {
		modbusListener.registerCoilRead(WC_ENABLED, WC::isEnabled);
		modbusListener.registerCoilWrite(WC_ENABLED, WC::setEnabled);
		modbusListener.registerCoilWrite(WC_HALF_FLUSH, WC::halfFlush);
		modbusListener.registerCoilWrite(WC_FULL_FLUSH, WC::fullFlush);
		modbusListener.registerCoilWrite(FLOWERS_LIGHT, Flowers::light);
	}

} }