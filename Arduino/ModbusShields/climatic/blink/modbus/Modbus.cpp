#define BUFFER_SIZE 128

#include "./Modbus.h"
#include "./util.cpp"

namespace Modbus{
	void setup(long baud, unsigned char _slaveID, unsigned char _TxEnablePin) {
		slaveID = _slaveID;
		TxEnablePin = _TxEnablePin;
		//Request req();
		errorCount = 0;

		Serial.begin(baud);
        pinMode(TxEnablePin, OUTPUT);
        digitalWrite(TxEnablePin, LOW);
        // Modbus states that a baud rate higher than 19200 must use a fixed 750 us
        // for inter character time out and 1.75 ms for a frame delay.
        // For baud rates below 19200 the timeing is more critical and has to be calculated.
        // E.g. 9600 baud in a 10 bit packet is 960 characters per second
        // In milliseconds this will be 960characters per 1000ms. So for 1 character
        // 1000ms/960characters is 1.04167ms per character and finaly modbus states an
        // intercharacter must be 1.5T or 1.5 times longer than a normal character and thus
        // 1.5T = 1.04167ms * 1.5 = 1.5625ms. A frame delay is 3.5T.
        // Added experimental low latency delays. This makes the implementation
        // non-standard but practically it works with all major modbus master implementations.

        /*if (baud == 1000000 && _lowLatency) {
            T1_5 = 1;
            T3_5 = 10;
        } else if (baud >= 115200 && _lowLatency){
            T1_5 = 75;
            T3_5 = 175;
        } else */if (baud > 19200) {
            T1_5 = 750;
            T3_5 = 1750;
        } else {
            T1_5 = 15000000/baud; // 1T * 1.5 = T1.5
            T3_5 = 35000000/baud; // 1T * 3.5 = T3.5
        }
	}
	void sendData(unsigned char *data) {
		if (req.isBroadcast())
            return;// don't respond if its a broadcast message

        Response response(req.getFunction());
        response.sendData(data);
	}
    void sendException(unsigned char errorCode) {
		errorCount++; // each call to exceptionResponse() will increment the errorCount
		if (req.isBroadcast())
			return;// don't respond if its a broadcast message

		Response response(req.getFunction());
		response.sendException(errorCode);
    }
}






