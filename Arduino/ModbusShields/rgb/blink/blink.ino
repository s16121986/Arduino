//#define ATTINY13 https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json
#include <avr/wdt.h> // здесь организована работа с ватчдогом
#include <avr/sleep.h> // здесь описаны режимы сна
#include <avr/interrupt.h>

//#include "lib/PinIn.cpp"
//#include "lib/PinOut.cpp"

#define PIN_SIG    PA7
#define PIN_RED    PA0
#define PIN_GREEN  PA1
#define PIN_BLUE   PA4

volatile uint8_t pingValue;
volatile uint8_t pingCounter;
volatile uint8_t pingPin;
volatile uint8_t disabled;
volatile uint8_t pinOut[2];

/*ISR(TIM1_OVF_vect) {
	
}*/

ISR (WDT_vect) {
	//sleep_disable();
	if (disabled) {
		if (1 == disabled) {
			//pinOut1.low();
			//pinOut2.low();
			digitalWrite(pinOut[pingValue - 1], LOW);
			pingPin = 0;
			pingValue = 0;
			pingCounter = 0;
		}
		disabled--;
	} else if (pingValue > 0) {
		if (pingCounter > 0) {
			disabled = 4;
			digitalWrite(pinOut[pingValue - 1], HIGH);
		} else
			pingCounter++;
	}
	
	WDTCSR |= (1<<WDIE); // разрешаем прерывания по ватчдогу. Иначе будет резет.
	//sleep_enable();
}

ISR(PCINT0_vect) {
	if (disabled || pingValue > 1)
		return;
	
	if (pingPin) {
		if (!digitalRead(pingPin))
			return;
	} else if (digitalRead(PIN_IN1)) {
		pingPin = PIN_IN1;
		pinOut[0] = PIN_OUT1;
		pinOut[1] = PIN_OUT2;
	} else if (digitalRead(PIN_IN2)) {
		pingPin = PIN_IN2;
		pinOut[0] = PIN_OUT3;
		pinOut[1] = PIN_OUT4;
	} else
		return;
		
	pingValue++;
			
	//pinIn1.listen();
	//pinIn2.listen();
	
	/*if (1 == pinIn1.value)
		digitalWrite(PIN_OUT3, HIGH);
	else if (2 == pinIn1.value)
		digitalWrite(PIN_OUT3, LOW);*/
}
	
void setup() {
	/*DDRA |= PIN_OUT1;
	DDRA |= PIN_OUT2;
	DDRA |= PIN_OUT3;
	DDRA |= PIN_OUT4;*/
	disabled = 0;
	pingPin = 0;
	pingValue = 0;
	pingCounter = 0;
	
	pinMode(PIN_OUT1, OUTPUT);
	pinMode(PIN_OUT2, OUTPUT);
	pinMode(PIN_OUT3, OUTPUT);
	pinMode(PIN_OUT4, OUTPUT);
	
	GIMSK |= (1<<PCIE0);
	PCMSK0 |= (1<<PCINT6) | (1<<PCINT7);
	
	//TCCR0B |= (1<<CS02)|(1<<CS00); // set prescaler to 64 (CLK=1Mz/(1024*510)=2Hz, 0.5s)
	//TCCR0B |= (1<<CS02); // set prescaler to 64 (CLK=1Mz/(256*510)=8Hz, 0.1s)
	//TCCR0B |= (1<<CS01)|(1<<CS00); // set prescaler to 64 (CLK=1Mz/(64*510)=30Hz, 0.03s)
	//TIMSK0 |= (1<<TOIE0); // enable Timer Overflow interrupt
	
	//MCUCR = (1<<ISC01) | (0<<ISC00);
	
	//ADC - Analog to Digital Converter
	ADCSRA &= ~(1 << ADEN); // ADC Control and Status Register A Disable
	ACSR = (1<<ACD); // Analog Comparator Disable
	
	/*set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_enable();
	sei();
	sleep_cpu();
	sleep_disable();
	*/
	
	//инициализация ватчдога
	wdt_reset(); // сбрасываем
	wdt_enable(WDTO_30MS); // разрешаем ватчдог
	MCUSR = 0x00;
	WDTCSR |= (1<<WDIE);
	//WDTCSR |= (1<<WDCE) | (1<<WDE) | (1<<WDIF);
	sei(); // разрешаем прерывания
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	while(1) {
		sleep_mode();
	}
}

void loop() {}
