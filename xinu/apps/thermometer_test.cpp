#include <xinu_cpp.h>
#include <thermometer_test.h>
#include <led_test.h>


void thermometer_test() {
	Tmp36 t(P84, 0);
	for (uint32 i = 0; i < 500; i++) {
		kprintf("temp is %d C\n", t.read());
		led_test();
	}
	t.deinit();
}
