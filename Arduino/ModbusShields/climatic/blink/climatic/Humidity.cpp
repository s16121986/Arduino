namespace Climatic{
	unsigned int readHumidity() {
    		float t = dht.readHumidity(); //Измеряем влажность
		Serial.print("hum=");
		Serial.println(t);
            if (isnan(t)) {  // Проверка. Если не удается считать показания, выводится «Ошибка считывания», и программа завершает работу
                //Serial.println("Ошибка считывания");
                return 0;
            }
    		return t;
    	}
}