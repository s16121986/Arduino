namespace Modbus{
	unsigned int calculateCRC(byte bufferSize) {
		unsigned int temp, temp2, flag;
		temp = 0xFFFF;
		for (unsigned char i = 0; i < bufferSize; i++) {
			temp = temp ^ frame[i];
			for (unsigned char j = 1; j <= 8; j++) {
				flag = temp & 0x0001;
				temp >>= 1;
				if (flag)
					temp ^= 0xA001;
			}
		}
		// Reverse byte order.
		temp2 = temp >> 8;
		temp = (temp << 8) | temp2;
		temp &= 0xFFFF;
		return temp; // the returned value is already swopped - crcLo byte is first & crcHi byte is last
	}

	unsigned int CRC16(unsigned char *buf, unsigned int len) {
		unsigned int crc = 0xFFFF;
		for (int pos = 0; pos < len; pos++) {
			crc ^= (unsigned int)buf[pos];        // XOR byte into least sig. byte of crc
			for (int i = 8; i != 0; i--) {        // Loop over each bit
				if ((crc & 0x0001) != 0) {        // If the LSB is set
					crc >>= 1;                    // Shift right and XOR 0xA001
					crc ^= 0xA001;
				} else                            // Else LSB is not set
					crc >>= 1;                    // Just shift right
			}
		}

		return crc;
	}
}