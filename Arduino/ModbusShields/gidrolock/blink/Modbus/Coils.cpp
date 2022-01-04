namespace Modbus{ namespace Coils{
	//using namespace WC;

	enum REGISTERS{
        ALARM_STATE,

        GIDROLOCK_HOT_STATE,
        GIDROLOCK_COLD_STATE,

        REGISTERS_SIZE
    };

    bool getHotState() { return Controller::motorHot.getState(); }

    bool getColdState() { return Controller::motorCold.getState(); }

    void setHotState(bool flag) { return Controller::motorHot.setState(flag); }

    void setColdState(bool flag) { return Controller::motorCold.setState(flag); }

	void setup() {
		modbusListener.registerCoilRead(ALARM_STATE, Controller::alarmState);
		modbusListener.registerCoilRead(GIDROLOCK_HOT_STATE, getHotState);
		modbusListener.registerCoilRead(GIDROLOCK_COLD_STATE, getColdState);
		modbusListener.registerCoilWrite(GIDROLOCK_HOT_STATE, setHotState);
		modbusListener.registerCoilWrite(GIDROLOCK_COLD_STATE, setColdState);
	}

} }