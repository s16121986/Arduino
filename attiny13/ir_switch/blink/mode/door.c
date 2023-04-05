#define POWER_MODE
#define STATE_OPEN 0
#define PIN_LED 1<<0 // PB0
#define PIN_INT 1<<1 // PB1 (INT0)
#define PIN_VCC 1<<2 // PB2

bool currentState;

void blink() {
	PORTB |= PIN_LED;                 // Включаем светодиод
	delay(5);
	bool state = STATE_OPEN == (PINB & PIN_INT);
	PORTB &= ~PIN_LED;                // Выключаем светодиод
	if (currentState != state) {     // Переключаем питание при смене состояния
		currentState = state;
		if (currentState)
			PORTB |= PIN_VCC;
		else
			PORTB &= ~PIN_VCC;
	}
}

#ifdef POWER_MODE
ISR (WDT_vect) {
	blink();
	WDTCR |= 1<<WDTIE; // разрешаем прерывания по ватчдогу. Иначе будет резет.
	//sleep_mode();
}
#endif

//ISR(INT0_vect) { state = HIGH; }     // Срабатывает при открытии двери

void setup() {
	DDRB |= PIN_LED;                  // pin output
	DDRB |= PIN_VCC;                  // pin output
#ifdef POWER_MODE
	//инициализация ватчдога
	wdt_reset(); // сбрасываем
	
	ADCSRA &= ~(1 << ADEN); // Disable ADC
	ACSR = (1<<ACD); // Disable the analog comparator
	
	//MCUSR &= ~(1 << WDRF);
	// Start the WDT Config change sequence.
	//WDTCR |= (1 << WDCE) | (1 << WDE);
	
	wdt_enable(WDTO_500MS); // разрешаем ватчдог 0.5 сек
	WDTCR |= 1<<WDTIE; // разрешаем прерывания по ватчдогу. Иначе будет резет.
	sei(); // разрешаем прерывания
	set_sleep_mode(SLEEP_MODE_PWR_DOWN); // если спать - то на полную
	while(1) {
		sleep_mode();
	}
#endif
}
void loop() {
#ifndef POWER_MODE
	blink();
	delay(500);                      //Таймаут цикла
#endif
}