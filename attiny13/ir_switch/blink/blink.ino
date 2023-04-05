//#include <util/delay.h>
//#include <avr/io.h>
#include <avr/wdt.h> // здесь организована работа с ватчдогом
#include <avr/sleep.h> // здесь описаны режимы сна
#include <avr/interrupt.h>

#define MODE_DOOR 1
#define MODE_SWITCH 2

#define PIN_LED 1<<0 // PB0
#define PIN_INT 1<<1 // PB1 (INT0)
#define PIN_VCC 1<<2 // PB2

bool currentState;
uint8_t mode;
uint8_t skip;

bool isBarrier() {
	PORTB |= PIN_LED;                 // Включаем светодиод
	delay(5);
	bool state = PINB & PIN_INT;
	PORTB &= ~PIN_LED;                // Выключаем светодиод
	return state;
}
void toggleVcc() {
	currentState = !(PORTB & PIN_VCC); //инвертивуем включенное состояние
	if (currentState)
		PORTB |= PIN_VCC;
	else
		PORTB &= ~PIN_VCC;
}
void setMode(uint8_t m) {
	mode = m;
	if (mode == MODE_DOOR)
		wdt_enable(WDTO_500MS);
	else
		wdt_enable(WDTO_30MS);
	WDTCR |= 1<<WDTIE; // разрешаем прерывания по ватчдогу. Иначе будет резет.
}
void switchState() {
	if (skip) {
		skip--;
	} else if (isBarrier()) {
		 // Переключаем питание при смене состояния
		skip = 20;
		toggleVcc();
	}
}
void doorState() {
	bool state = !isBarrier();
	if (currentState != state)     // Переключаем питание при смене состояния
		toggleVcc();
}

ISR (WDT_vect) {
	if (mode == MODE_DOOR)
		doorState();
	else
		switchState();

	WDTCR |= 1<<WDTIE; // разрешаем прерывания по ватчдогу. Иначе будет резет.
	//sleep_mode();
}

//ISR(INT0_vect) { state = HIGH; }     // Срабатывает при открытии двери

void setup() {
	currentState = false;
	skip = 0;

	DDRB |= PIN_LED;                  // pin output
	DDRB |= PIN_VCC;                  // pin output
	PORTB &= ~PIN_LED;
	PORTB &= ~PIN_VCC;
	//инициализация ватчдога
	wdt_reset(); // сбрасываем

	ADCSRA &= ~(1 << ADEN); // Disable ADC
	ACSR = (1<<ACD); // Disable the analog comparator

	//MCUSR &= ~(1 << WDRF);
	// Start the WDT Config change sequence.
	//WDTCR |= (1 << WDCE) | (1 << WDE);

	setMode(MODE_DOOR);
	sei(); // разрешаем прерывания
	set_sleep_mode(SLEEP_MODE_PWR_DOWN); // если спать - то на полную
	while(1) {
		sleep_mode();
	}
}
void loop() {

}