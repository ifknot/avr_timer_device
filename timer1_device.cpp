/*
 * timer1_device.cpp
 *
 * Created: 4/19/2020 1:42:24 PM
 *  Author: ifkno
 */ 

#include "timer1_device.h"

ISR(TIMER1_OVF_vect) {
	cli();
	chrono::timer1_device::__process_interrupt();
	TCNT1 =	chrono::timer1_device::__overflow();
	sei();
}

namespace chrono {
	
	uint64_t timer1_device::tick_count	= 0;
	uint16_t timer1_device::overflow	= 0xFFFF; // i.e. 1 tick to overflow

	timer1_device::timer1_device() {
		cli();
		TCCR1B = prescale_stop; 
		TCCR1A = 0x00;
		TIMSK1 |= 1<<TOIE1;		//Enable Timer 1 Overflow Interrupt
		sei();					// Enable global interrupts by setting global interrupt enable bit in SREG
	}
	
}


