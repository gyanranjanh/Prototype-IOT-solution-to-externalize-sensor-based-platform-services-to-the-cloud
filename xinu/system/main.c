#include <xinu.h>
#include <thermometer_test.h>
#include <mqttcliapi.h>

/* ----------------------------------------------------------- */
int main(void)
{
    kprintf("main>>\n");

	unsigned int sample;

#if 0
    iolib_init();
    led_test();
	iolib_free();
#endif

    iolib_init();
    init_timer_module();
    mqtt_start();

    initialize_services();
    while(TRUE);
    deinitialize_services();
    mqtt_stop();
    iolib_free();
    kprintf("main<<\n");

	return 0;
}


