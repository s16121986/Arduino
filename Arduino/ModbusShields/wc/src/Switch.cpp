namespace IR{
	ir;

	bool holdFlag = false;
	bool pressFlag;
	unsigned long pressTime;
	unsigned long autoCloseTime;

	void listen() { ir.listen(); };
	void loop() {
		if (autoCloseTime != 0) {
			if (CURRENT_TIME - autoCloseTime > closeTimeout) {
				Servo::close();
				autoCloseTime = 0;
			}
			return;
		}
		//onPress
		bool state = isPressed();

		pressTime = CURRENT_TIME;

		if (isPressed() && CURRENT_TIME - pressTime > holdTimeout)
			holdFlag = true;

		//on release
		if (holdFlag)
			Servo::close();
		else {
			Servo::open();
			autoCloseTime = CURRENT_TIME;
		}

		// on hold
		Servo::open();
		humanDetected = true;

		disableListen();
		//start timer
	};

	void reset() {
		humanDetected = false;
		pressTime = 0;
		autoCloseTime = 0;


	}
}