namespace ModbusClientUtil{

	enum MESSAGE {
		ID = 0, //!< ID field
		FUNC, //!< Function code position
		ADD_HI, //!< Address high byte
		ADD_LO, //!< Address low byte
		NB_HI, //!< Number of coils or registers high byte
		NB_LO, //!< Number of coils or registers low byte
		BYTE_CNT  //!< byte counter
	};

	enum EXCEPTIONS {
		ILLEGAL_FUNCTION = 1,     //� ������� ��� ������� ������������ ��� �������
		ILLEGAL_DATA_ADDRESS = 2, //��������� � ������� ����� �� ����������
		ILLEGAL_DATA_VALUE = 3,   //�������� ������ �������, �������� ���������� ���� � ������� �� ������������� ����������.
		//����������: �������� �� ��������, ��� ������ �� ������� � ���, ��� ���� �������� �������� ������������ ��� ���������, � ������ �������������� ������ ��� ������ ������� �������.
		SLAVE_DEVICE_FAILURE = 4, //��������� ��������������� ������ �� ���������� ��� ���������� ����������� ��������
		ACKNOWLEDGE = 5,          //������ ������, �����������, �� ���������� ��������� ����� �������; ���������� ��������� �������.
		SLAVE_DEVICE_BUSY = 6,    //���������� ������ ���������� ����������� �������.
		NEGATIVE_ACKNOWLEDGE = 7, //���������� �� ����� ��������� ������, ���������� �������� �� ���������� �������������� ��������������� ����������. ��������, ��������� ���. ������������.
		MEMORY_PARITY_ERROR = 8   //������ �������� ��� ��������� � ���������� ������ ����������.
	};

	enum FUNCTIONS {
		FUNCTION_READ_COILS = 0x01, //������ �������� ���������� ��������� ������
		FUNCTION_READ_DISCRETE_INPUTS = 0x02, //������ �������� ���������� ���������� ������
		FUNCTION_READ_HOLDING_REGISTERS = 0x03, //������ �������� ���������� ��������� ��������
		FUNCTION_READ_INPUT_REGISTERS = 0x04, //������ �������� ���������� ��������� �����
		FUNCTION_WRITE_SINGLE_COIL = 0x05, //������ ������ �������� ������
		FUNCTION_WRITE_SINGLE_REGISTER = 0x06, //������ ������ �������� ��������
		FUNCTION_WRITE_MULTIPLE_COILS = 0x0F, //������ ���������� ��������� ������
		FUNCTION_WRITE_MULTIPLE_REGISTER = 0x10, //������ ���������� ��������� ��������
	};

	uint16_t CRC16(uint8_t* buf, uint8_t len) {
		uint16_t temp, temp2, flag;
		temp = 0xFFFF;
		for (uint8_t i = 0; i < len; i++) {
			temp = temp ^ buf[i];
			for (uint8_t j = 1; j <= 8; j++) {
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
		// the returned value is already swapped
		// crcLo byte is first & crcHi byte is last
		return temp;
	}

	uint16_t toUint16(uint8_t highByte, uint8_t lowByte) { return (highByte << 8) | lowByte; }

	uint16_t toUint16(bool value) {
		if (value)
			return 0xFF00;
		else
			return 0x0000;
	}

	void addUint16Value(uint8_t* frame, uint16_t value, uint8_t pos) {
		frame[pos] = value >> 8; // split crc into 2 bytes
		frame[pos + 1] = value & 0xFF;
	}

	void addCRC(uint8_t* frame, uint8_t pos) {
		uint16_t crc = CRC16(frame, pos);
		/*Serial.print("TX crc=");
		Serial.println(crc, HEX);*/
		addUint16Value(frame, crc, pos);
	}

	uint16_t msFromUint16(uint16_t value) { return value * 10; }

	uint16_t secFromUint16(uint16_t value) { return value * 1000; }

	uint16_t msToUint16(uint16_t value) { return value / 10; }

	uint16_t secToUint16(uint16_t value) { return value / 1000; }
}