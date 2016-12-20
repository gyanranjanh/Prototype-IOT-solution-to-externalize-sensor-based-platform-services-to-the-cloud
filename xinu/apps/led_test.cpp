#include <xinu_cpp.h>
#include <led_test.h>


void led_test() {
	Led led(P44);
	led.write(1);
	kprintf("led status %d\n", led.read());
	sleep(2);
	led.write(0);
	sleep(2);
	kprintf("led status %d\n", led.read());
	led.deinit();
}
