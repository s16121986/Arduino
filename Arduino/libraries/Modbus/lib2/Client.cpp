namespace Modbus{ class Client {
public:
	Client() {}

	void setup(uint8_t _slaveID, uint8_t _TxEnablePin, uint8_t u8serno) {
		switch(u8serno) {
#if defined(UBRR1H)
			case 1:
				port = &Serial1;
				break;
#endif

#if defined(UBRR2H)
			case 2:
				port = &Serial2;
				break;
#endif

#if defined(UBRR3H)
			case 3:
				port = &Serial3;
				break;
#endif
			case 0:
			default:
				port = &Serial;
				break;
		}

		slaveID = _slaveID;
		TxEnablePin = _TxEnablePin;
		//Request req();
		errorCount = 0;

        pinMode(TxEnablePin, OUTPUT);
        digitalWrite(TxEnablePin, LOW);
    }

	void setup(uint8_t _slaveID, uint8_t _TxEnablePin) { setup(_slaveID, _TxEnablePin, 0); }

	void begin(long baud) {
		static_cast<HardwareSerial*>(port)->begin(baud);
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

		while(port->read() >= 0);
	}

    Response &getResponse() { return response; }

    void sendException(uint8_t errorCode) {
        errorCount++; // each call to exceptionResponse() will increment the errorCount
        response.setException(errorCode);
        sendResponse();
    }

    void sendResponse() {
        if (req.isBroadcast())
            return;// don't respond if its a broadcast message
        response.send();
    }

    //void setDisabled(bool flag) { disabledFlag = flag; }
    bool available() {
        if (disabledFlag || !port->available())
            return false;

        disabledFlag = true;
        uint8_t buffer[MODBUS_BUFFER_SIZE];
        uint8_t overflow = 0;
        uint8_t bufferSize = 0;

#ifdef MODBUS_DEBUG
        Serial.print("Modbus.available: ");
#endif
        while (port->available()) {
            // The maximum number of bytes is limited to the serial buffer size of 128 bytes
            // If more bytes is received than the MODBUS_BUFFER_SIZE the overflow flag will be set and the
            // serial buffer will be red untill all the data is cleared from the receive buffer.
            if (overflow)
                port->read();
            else {
                if (bufferSize >= MODBUS_BUFFER_SIZE)
                    overflow = 1;
                buffer[bufferSize] = port->read();
#ifdef MODBUS_DEBUG
                //Serial.print("input[");
                //Serial.print(bufferSize);
                //Serial.print("]=");
                Serial.print("[");
                Serial.print(buffer[bufferSize]);
                Serial.print("]");/**/
#endif
                bufferSize++;
            }
            delayMicroseconds(T1_5); // inter character time out
        }
#ifdef MODBUS_DEBUG
        Serial.println();
#endif

        disabledFlag = false;

        // If an overflow occurred increment the errorCount
        // variable and return to the main sketch without
        // responding to the request i.e. force a timeout
        if (overflow)
            return error();
        else if (bufferSize <= 6)
            return error(); // corrupted packet

        if (!req.setBuffer(buffer, bufferSize))
            return false; // slaveId not match
        else if (!req.validate())
            return exception(ILLEGAL_DATA_VALUE); // corrupted packet
        //else if (disabled)
        //	return exception(SLAVE_DEVICE_BUSY);

        //Serial.println("request recived");
        response.setRequest(req);

        return true;
    }

    bool error() {
        errorCount++;
        return false;
    }

    bool exception(uint8_t code) {
#ifdef DEBUG_PORT
        Serial.print("exception: ");
        Serial.println(code);
#endif
        sendException(code);
        processingFlag = false;
        return false;
    }

    bool exception() { return exception(1); }

    bool isProcessing() { return processingFlag; }

    void processRequest() {
        processingFlag = true;
        uint16_t reqFunc = req.getFunction();
#ifdef MODBUS_DEBUG
        Serial.print("req.function=");
        Serial.println(reqFunc);
#endif
        if (!checkFunction(reqFunc)) {
            Serial.println("function not registered");
            exception(ILLEGAL_FUNCTION);
            return;
        }

        switch (reqFunc) {
            case FUNCTION_READ_COILS:
                readCoils();
                break;
            case FUNCTION_READ_DISCRETE_INPUTS:
                readDiscreteInputs();
                break;
            case FUNCTION_READ_HOLDING_REGISTERS:
                readHoldingRegisters();
                break;
            case FUNCTION_READ_INPUT_REGISTERS:
                readHoldingRegisters();
                break;
            case FUNCTION_WRITE_SINGLE_COIL:
                writeSingleCoil();
                break;
            case FUNCTION_WRITE_MULTIPLE_COILS:
                writeMultipleCoils();
                break;
            case FUNCTION_WRITE_SINGLE_REGISTER:
                writeSingleRegister();
                break;
            case FUNCTION_WRITE_MULTIPLE_REGISTER:
                writeMultipleRegister();
                break;
            default:
                response.setException(ILLEGAL_FUNCTION);
        }

        sendResponse();
        processingFlag = false;
    }

    void loop() {
        if (!available())
            return;

        processRequest();
/*
ILLEGAL_FUNCTION = 1,     //В запросе был передан недопустимый код функции
ILLEGAL_DATA_ADDRESS = 2, //Указанный в запросе адрес не существует
ILLEGAL_DATA_VALUE = 3,   //Неверный формат запроса, например количество байт в запросе не соответствует ожидаемому.
                          //Примечание: несмотря на название, эта ошибка не говорит о том, что само значение регистра неправильное или ошибочное, и должна использоваться только для ошибок формата запроса.
SLAVE_DEVICE_FAILURE = 4, //Произошла невосстановимая ошибка на устройстве при выполнении запрошенной операции
ACKNOWLEDGE = 5,          //Запрос принят, выполняется, но выполнение потребует много времени; необходимо увеличить таймаут.
SLAVE_DEVICE_BUSY = 6,    //Устройство занято обработкой предыдущего запроса.
NEGATIVE_ACKNOWLEDGE = 7, //Устройство не может выполнить запрос, необходимо получить от устройства дополнительную диагностическую информацию. Возможно, требуется тех. обслуживание.
MEMORY_PARITY_ERROR = 8   //Ошибка четности при обращении к внутренней памяти устройства.

FUNCTION_READ_COILS = 0x01, //Чтение значений нескольких регистров флагов
FUNCTION_READ_DISCRETE_INPUTS = 0x02, //Чтение значений нескольких дискретных входов
FUNCTION_READ_HOLDING_REGISTERS = 0x03, //Чтение значений нескольких регистров хранения
FUNCTION_READ_INPUT_REGISTERS = 0x04, //Чтение значений нескольких регистров ввода
FUNCTION_WRITE_SINGLE_COIL = 0x05, //Запись одного регистра флагов
FUNCTION_WRITE_SINGLE_REGISTER = 0x06, //Запись одного регистра хранения
FUNCTION_WRITE_MULTIPLE_COILS = 0x0F, //Запись нескольких регистров флагов
FUNCTION_WRITE_MULTIPLE_REGISTER = 0x10 //Запись нескольких регистров хранения
*/

    }

    uint16_t CRC16(uint8_t *buf, uint8_t len) {
        uint16_t temp, temp2, flag;
        temp = 0xFFFF;
        for (uint8_t i = 0; i < len; i++) {
            temp = temp ^ buf[i];
            for (uint8_t j = 1; j <= 8; j++)  {
                flag = temp & 0x0001;
                temp >>=1;
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
private:

};


}






