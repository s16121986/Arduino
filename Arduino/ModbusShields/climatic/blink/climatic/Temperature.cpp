namespace Climatic{
	unsigned int readTemperature() {
		//float h = dht.readHumidity(); //Измеряем влажность
        float t = dht.readTemperature(); //Измеряем температуру
		Serial.print("temp=");
		Serial.println(t);
        if (isnan(t)) {  // Проверка. Если не удается считать показания, выводится «Ошибка считывания», и программа завершает работу
            //Serial.println("Ошибка считывания");
            return 0;
        }
		return t;
	}
}