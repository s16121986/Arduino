namespace Modbus{
	class Request{
	public:
		Request () {
			function = 0;
			broadcastFlag = false;
		}
		bool available() {
			if (!Serial.available())
				return false;

			function = 0;
			broadcastFlag = false;

			unsigned char frame[BUFFER_SIZE];
			unsigned char overflow = 0;
			unsigned char buffer = 0;

			while (Serial.available()) {
	            // The maximum number of bytes is limited to the serial buffer size of 128 bytes
	            // If more bytes is received than the BUFFER_SIZE the overflow flag will be set and the
	            // serial buffer will be red untill all the data is cleared from the receive buffer.
	            if (overflow)
	                Serial.read();
	            else {
	                if (buffer == BUFFER_SIZE)
	                    overflow = 1;
	                frame[buffer] = Serial.read();
	                buffer++;
	            }
	            delayMicroseconds(T1_5); // inter character time out
	        }

			// If an overflow occurred increment the errorCount
			// variable and return to the main sketch without
			// responding to the request i.e. force a timeout
			if (overflow)
				return exception();
			else if (buffer <= 6)
				return exception(); // corrupted packet

			unsigned char id = frame[0];

			if (id == 0)
				broadcastFlag = true;
			else if (id != slaveID)
				return false;

			unsigned int crc = ((frame[buffer - 2] << 8) | frame[buffer - 1]); // combine the crc Low & High bytes
			if (calculateCRC(buffer - 2) != crc) // if the calculated crc matches the recieved crc continue
				return exception(); // checksum failed

			function = frame[1];
			startingAddress = ((frame[2] << 8) | frame[3]); // combine the starting address bytes
			no_of_registers = ((frame[4] << 8) | frame[5]); // combine the number of register bytes

			return true;
		}
		bool isException() { return errorCode > 0; }
		unsigned char getException() { return errorCode; }
		bool isBroadcast() { return broadcastFlag; };
		unsigned char getFunction() { return function; }
		unsigned char getAddress() { return startingAddress; }
		unsigned char getAddressCount() { return no_of_registers; }
		unsigned int getMaxData() { return startingAddress + no_of_registers; }
	private:
		bool exception(unsigned char code) {
			errorCode = code;
			return true;
		}
		bool exception() { return exception(1); }

		unsigned char function;
		unsigned int startingAddress;
		unsigned int no_of_registers;
		bool broadcastFlag;

		unsigned char errorCode;
	}
}










function = frame[1];
unsigned int startingAddress = ((frame[2] << 8) | frame[3]); // combine the starting address bytes
unsigned int no_of_registers = ((frame[4] << 8) | frame[5]); // combine the number of register bytes
unsigned int maxData = startingAddress + no_of_registers;
unsigned char index;
unsigned char address;
unsigned int crc16;

// broadcasting is not supported for function 3
if (!broadcastFlag && (function == 3))
switch (function) {
	case 3:
		return readHoldingRegisters();
	case 6:
		return writeSingleRegister();
	case 16:
		return writeMultipleRegister();
	default:
		return exceptionResponse(1); // exception 1 ILLEGAL FUNCTION
}

return errorCount;