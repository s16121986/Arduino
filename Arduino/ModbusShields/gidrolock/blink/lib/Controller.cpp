namespace Controller{
	enum MOTORS{
		MOTOR_COLD,
		MOTOR_HOT,
		//
		MOTORS_SIZE
	};

	Gidrolock &getMotor(uint8_t address) {
		switch (address) {
            case MOTOR_COLD:
                return motorCold;
            case MOTOR_HOT:
                return motorHot;
        }
	}
	bool getMotorState (uint8_t address) {
		Gidrolock &motor = getMotor(address);
		return motor.getState();
	}
	void openMotor(uint8_t address) {
		Gidrolock &motor = getMotor(address);
		motor.open();
	}
	void closeMotor(uint8_t address) {
		Gidrolock &motor = getMotor(address);
		motor.close();
	}
	void listen() {
		motorCold.listen();
		motorHot.listen();
	}
}