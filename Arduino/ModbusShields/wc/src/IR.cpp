namespace IR{
	ir;

	bool humanDetected = false;
	unsigned long actionTime;

	void listen() { ir.listen(); };
	void loop() {
		if (!ir.onHigh())
			return;

		humanDetected = true;

		disableListen();
		//start timer
	};
	void reset() {
		humanDetected = false;
		actionTime = 0;


	}
}