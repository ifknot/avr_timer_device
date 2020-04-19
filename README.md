# avr_timer_device
C++ singleton TIMER1 with convenient class member functions for prescaling start stop tick counter etc.

Combines Meyer's canonical singleton with avrbeginners.net excellent article [Timers](http://www.avrbeginners.net/architecture/timers/timers.html)

## examples (also uses my uart and assert code)
```cpp

auto& uart = singleton::uart_device::instance();
	auto& timer1 = chrono::timer1_device::instance();
	
	timer1.start(timer1.divideby256, 4);
	
	uart.write_hex(timer1.ticks());
	uart.endl();
	uart.write_hex(timer1.ticks());
	uart.endl();
	uart.write_hex(timer1.ticks());
	uart.endl();
	
	auto f1 = timer1.frequency();
	uart.write_dec(f1); 	
	uart.endl();
	
	timer1.stop();
	timer1.reset_tick_count();
	timer1.start(timer1.divideby1024, 1);
	
	uart.write_hex(timer1.ticks());
	uart.endl();
	uart.write_hex(timer1.ticks());
	uart.endl();
	uart.write_hex(timer1.ticks());
	uart.endl();
	
	auto f2 = timer1.frequency();
	uart.write_dec(f2);
  
	REQUIRE(f1 == f2);

```
