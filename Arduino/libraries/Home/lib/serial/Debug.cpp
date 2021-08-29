namespace SerialDebug{

	char strToByte(byte c[3], byte l) {
		byte b = 0, i;
		double d;
		for (i = 0; i < l; i++) {
			d = round(pow(10.0, ((double)l - (double)i - 1.0)));
			b +=  d * (c[i] - '0');
		}
		//Serial.println(b);
		return b;
	}
	
	/*void setup(byte bit) {
		triggerBit = bit;
		setup();
	}
	
	void clear() {
		delete buffer;
	}
	
	void copy(uint8_t (&b)[4]) {
		b[0] = buffer[0];
		b[1] = buffer[1];
		b[2] = buffer[2];
		b[3] = buffer[3];
	}
	
	bool read() {
		if (!Serial.available())
			return false;
		
		byte b, l = 0, i = 1;
		byte c[3];
		if (Serial.read() == triggerBit) {
			delay(1);
			clear();
			buffer[0] = triggerBit;
			Serial.print(" ");
			Serial.print(buffer[0]);
			while (Serial.available()) {
				b = Serial.read();
			Serial.print(" ");
			Serial.print(b);
				//Serial.print("//");
				//Serial.println(b);
				if (b == 32 || b == 10) { // "[space] | EOL"
					buffer[i++] = strToByte(c, l);
					l = 0;
				} else {
					c[l++] = b;
				}
				delay(1);
			}
			Serial.println(";");
			
			return true;
		}
		
		//::SerialPort::clear();
			
		return false;
	}*/

	void read (uint8_t (&buffer)[4]) {
		//Serial.println("buffer");
        byte b, l = 0, i = 1;
        byte c[3];

        buffer[0] = Serial.read();
		Serial.print("Serial buffer: ");
		Serial.print((char)buffer[0]);
        delay(1);
        while (Serial.available()) {
            b = Serial.read();
            if (b == 32 || b == 10) { // "[space] | EOL"
                buffer[i++] = strToByte(c, l);
                l = 0;
				Serial.print(" ");
            } else {
                c[l++] = b;
				Serial.print((char)b);
            }
            delay(1);
        }
		Serial.println(";");
	}
	
}