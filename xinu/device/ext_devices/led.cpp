#include <xinu_cpp.h>


Led::Led(pinname_t p):Device(p){
#ifdef LED_TEST
	kprintf("led pin : %u\n", pin);
#endif
	init(1);
}

bool Led::init(bool inout) {
    if (!pinmap[pin].pp.port || !pinmap[pin].pp.pin) {
        kprintf("invalid pin\n");
        return false;
    }

#ifdef LED_TEST
	kprintf("led inout : %u\n", inout);
	kprintf("led port : %u\n", pinmap[pin].pp.port);
	kprintf("led pin : %u\n", pinmap[pin].pp.pin);
#endif

    BBBIO_sys_Enable_GPIO(pinmap[pin].pp.port);
    if (!inout) {
        BBBIO_GPIO_set_dir(pinmap[pin].pp.port, 1 << pinmap[pin].pp.pin, 0);
    }
    else {
        BBBIO_GPIO_set_dir(pinmap[pin].pp.port, 0, 1 << pinmap[pin].pp.pin);
    }
	BBBIO_sys_Disable_GPIO(pinmap[pin].pp.port);

    return true;
}

int Led::read() {
	int _status = 0;
	BBBIO_sys_Enable_GPIO(pinmap[pin].pp.port);
	_status = BBBIO_GPIO_get(pinmap[pin].pp.port, 1 << pinmap[pin].pp.pin);
	BBBIO_sys_Disable_GPIO(pinmap[pin].pp.port);
	_status = (_status & (1 << pinmap[pin].pp.pin)) >> (pinmap[pin].pp.pin);
	return _status;
}

void Led::write(bool data) {
#ifdef LED_TEST
	kprintf("led inout : %u\n", data);
	kprintf("led inout : %x\n", 1 << pinmap[pin].pp.pin);
#endif
	BBBIO_sys_Enable_GPIO(pinmap[pin].pp.port);
    if(data) {
        BBBIO_GPIO_high(pinmap[pin].pp.port, 1 << pinmap[pin].pp.pin);
    }
    else {
        BBBIO_GPIO_low(pinmap[pin].pp.port, 1 << pinmap[pin].pp.pin);
    }
	BBBIO_sys_Disable_GPIO(pinmap[pin].pp.port);
}

void Led::notify(uint32 interval, uint32 duration, 
                            void (*notifycb)(void *data), void *data) {
    kprintf("unable to notify data for led\n");
}

void Led::deinit() {
    BBBIO_sys_Disable_GPIO(pinmap[pin].pp.port);
}

