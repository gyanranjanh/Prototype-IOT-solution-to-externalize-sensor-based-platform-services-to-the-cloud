#include <xinu_cpp.h>
#include <digital_thermometer_test.h>
#include <led_test.h>


void digital_thermometer_test() {
	kprintf("digital_thermometer_test>>\n");
	Ds18b20 t(P16, 1);
	for (uint32 i = 0; i < 500; i++) {
		led_test();
		kprintf("temp is %d C\n", t.read());
	}
}

