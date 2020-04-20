/*
 * timer1.h
 *
 * Created: 4/19/2020 1:26:15 PM
 *  Author: ifkno
 */ 


#ifndef TIMER1_DEVICE_H_
#define TIMER1_DEVICE_H_

#include<avr/io.h>
#include<avr/interrupt.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

ISR(TIMER1_OVF_vect); //interrupt service routine

namespace chrono {
	
	/**
	 * Minimal avr timer (Meyer's canonical singleton implementation)
	 * @note Use -fno-threadsafe-statics 
	 * This disables the mutex around initialization of local static variables which is useless
	 * in single threaded and hence `__cxa_guard_acquire'	is undefined and will raise compile time error
	 */
	class timer1_device {
		
	public:
	
		/**
		 * CS12..10: Clock Select bits; These three bits control the prescaler of timer/counter 1 and the connection to an external clock on Pin T1. 
		 */
		enum prescale_t {	prescale_stop	= 0,
							prescale_none	= (1 << CS10),
							divideby8		= (1 << CS11),
							divideby64		= (1 << CS10) | (1 << CS11),
							divideby256		= (1 << CS12),
							divideby1024	= (1 << CS10) | (1 << CS12),
							fallingedge		= (1 << CS11) | (1 << CS12),
							risingedge		= (1 << CS10) | (1 << CS11) | (1 << CS12),
						};

		inline static timer1_device& instance() {
			static timer1_device t;
			return t;
		}
		
		inline uint64_t ticks() {
			return tick_count;
		}
		
		inline uint32_t frequency() {
			switch(TCCR1B) {
				case prescale_stop:
					return 0;
				case prescale_none:
					return F_CPU;
				case divideby8:
					return (F_CPU >> 3) / (0xFFFF - (overflow + 1));
				case divideby64:
					return (F_CPU >> 5) / (0xFFFF - (overflow + 1));
				case divideby256:
					return (F_CPU >> 8) / (0xFFFF - (overflow + 1));
				case divideby1024:
					return (F_CPU >> 10) / (0xFFFF - (overflow + 1));
				case fallingedge:
				case risingedge:
				default:
					return 0;
			}
		}
		
		inline void start(prescale_t prescale, uint16_t count) {
			overflow = 0xFFFF - count - 1;
			cli();
			TCCR1B = prescale;
			TCNT1 = overflow;	// e.g. prescalar = 1024 and overflow = 0xFFFF gives a tick every 0.064ms
			sei();
		}
		
		inline void stop() {
			cli();
			TCCR1B = 0;
			sei();
		}
		
		inline void reset_tick_count() {
			tick_count = 0;	
		}
		
		inline static void __process_interrupt()  {
			++tick_count;
		}
		
		inline static uint16_t __overflow() {
			return overflow;
		}

		timer1_device(const timer1_device &) = delete;
		
		timer1_device & operator = (const timer1_device &) = delete;
		
		~timer1_device() = default;

	private:

		/**
		 * assuming 16MHz on board crystal and the fuse bits are set appropriately
		 * the highest prescalar of 1024 and shortest count to overflow then every time timer TCNT1 overflows...
		 * timer = CPU Frequency/Prescalar
		 * timer = 16MHz/1024 = 15.625KHz
		 * tick = 1/ 15.625K = 0.000064 secs = 0.064ms = 640us
		 */
		timer1_device();
		
		static uint64_t tick_count; 
		static uint16_t overflow;
		
	};

}

#endif /* TIMER1_H_ */