namespace Modbus{

	enum EXCEPTIONS{
		ILLEGAL_FUNCTION = 1,     //В запросе был передан недопустимый код функции
		ILLEGAL_DATA_ADDRESS = 2, //Указанный в запросе адрес не существует
		ILLEGAL_DATA_VALUE = 3,   //Неверный формат запроса, например количество байт в запросе не соответствует ожидаемому.
                                  //Примечание: несмотря на название, эта ошибка не говорит о том, что само значение регистра неправильное или ошибочное, и должна использоваться только для ошибок формата запроса.
		SLAVE_DEVICE_FAILURE = 4, //Произошла невосстановимая ошибка на устройстве при выполнении запрошенной операции
		ACKNOWLEDGE = 5,          //Запрос принят, выполняется, но выполнение потребует много времени; необходимо увеличить таймаут.
		SLAVE_DEVICE BUSY = 6,    //Устройство занято обработкой предыдущего запроса.
		NEGATIVE_ACKNOWLEDGE = 7, //Устройство не может выполнить запрос, необходимо получить от устройства дополнительную диагностическую информацию. Возможно, требуется тех. обслуживание.
		MEMORY_PARITY_ERROR = 8   //Ошибка четности при обращении к внутренней памяти устройства.
	}
	enum FUNCTIONS{
		FUNCTION_READ_COILS = 0x01; //Чтение значений нескольких регистров флагов
		FUNCTION_READ_DISCRETE_INPUTS = 0x02; //Чтение значений нескольких дискретных входов
		FUNCTION_READ_HOLDING_REGISTERS = 0x03; //Чтение значений нескольких регистров хранения
		FUNCTION_READ_INPUT_REGISTERS = 0x04; //Чтение значений нескольких регистров ввода
		FUNCTION_WRITE_SINGLE_COIL = 0x05; //Запись одного регистра флагов
		FUNCTION_WRITE_SINGLE_REGISTER = 0x06; //Запись одного регистра хранения
		FUNCTION_WRITE_MULTIPLE_COILS = 0x0F; //Запись нескольких регистров флагов
		FUNCTION_WRITE_MULTIPLE_REGISTER = 0x10; //Запись нескольких регистров хранения
	}

	Request req;
	// frame[] is used to recieve and transmit packages.
	// The maximum serial ring buffer size is 128
	unsigned char slaveID;
	unsigned char TxEnablePin;
	unsigned int T1_5; // inter character time out
	unsigned int T3_5; // frame delay
	unsigned int errorCount;
}