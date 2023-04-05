#include <avr/wdt.h>            // библиотека для стандартных watchdog функций
#include <avr/interrupt.h>      // библиотека для работы с прерываниями
#include <avr/sleep.h>          // библиотека для режима сна
#include <avr/power.h>          // библиотека для управления режимами питания

namespace PowerSafe{
	void watchdogOn(unsigned char watchdogTimeout) {
		cli();                           // запретить прерывания для изменения регистров
		MCUSR = 0;                       // сбрасывание флагов status register
		                               // перевести таймер в режим interrupt-only:
		WDTCSR |= 0b00011000;            // Выставляем биты WDCE и WDE для входа в режим конфигурирования,
		WDTCSR =  0b01000000; // выставляем биты WDIE: разрешение прерывания
		                               // clr WDE: сбрасывание запрещено
		                               // и выставляем интервал задержки. Все битами

		//wdt_enable(WDTO_1S); //устанавливаем таймер на 1 секунду
		switch (watchdogTimeout) {
			case 0: //  16 мс:  0b000000
				WDTCSR |= 0b000000;
				break;
			case 1: //  500 мс: 0b000101
				WDTCSR |= 00b000101;
				break;
			case 2: //  1 сек:  0b000110
				WDTCSR |= 00b000110;
				break;
			case 3: //  2 сек: 0b000111
				WDTCSR |= 00b000111;
				break;
			case 4: //  4 сек: 0b100000
				WDTCSR |= 00b100000;
				break;
			case 5: //  8 сек: 0b100001
				WDTCSR |= 00b100001;
				break;
		}

		sei();                           // разрешаем прерывания
		//WDTCSR |= (1 << WDIE); //разрешаем прерывание
	}
	void sleep() {
		// The ATmega328 has five different sleep states.
		// See the ATmega 328 datasheet for more information.
		// SLEEP_MODE_IDLE -the least power savings
		// SLEEP_MODE_ADC
		// SLEEP_MODE_PWR_SAVE
		// SLEEP_MODE_STANDBY
		// SLEEP_MODE_PWR_DOWN -the most power savings
		// I am using the deepest sleep mode from which a
		// watchdog timer interrupt can wake the ATMega328

		//Установить в сон с самым низким потреблением. Разбудить при этом сможет внешнее прерывание или watchdog
		set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Set sleep mode.

		sleep_mode(); // разрешить сон и погрузиться в режим сна
		// CPU is now asleep and program execution completely halts!
		// Once awake, execution will resume at this point if the
		// watchdog is configured for resume rather than restart

		// When awake, disable sleep mode
		sleep_disable();
	}
	void setup() {
		// The following saves some extra power by disabling some
        // peripherals I am not using.

        // Disable the ADC by setting the ADEN bit (bit 7) to zero.
        ADCSRA = ADCSRA & B01111111;
        //выключить ADC пока спит
        //power_adc_disable();

        // Disable the analog comparator by setting the ACD bit
        // (bit 7) to one.
        ACSR = B10000000;

        // Disable digital input buffers on all analog input pins
        // by setting bits 0-5 to one.
        DIDR0 = DIDR0 | B00111111;
	}
	void setup(unsigned char watchdogTimeout) {
		watchdogOn(watchdogTimeout); // Turn on the watch dog timer.

		setup();
	}
}