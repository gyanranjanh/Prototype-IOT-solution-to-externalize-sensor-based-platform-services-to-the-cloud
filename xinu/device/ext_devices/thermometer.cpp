#include <xinu_cpp.h>

Thermometer::Thermometer(pinname_t p, bool t) : type(t), Device(p) {
#ifdef TMP_DEBUG
	kprintf("Thermometer>>%u\n", p);
#endif
	this->state = IDLE_STATE;
}

bool Thermometer::init(bool inout) {
    if (inout) {
        kprintf("can not configure thermometer as out\n");
        return false;
    }
#ifdef TMP_DEBUG
	kprintf("Thermometer init %d\n", type);
#endif
    if (!type) { //anlalog
        if (pinmap[pin].adcpin == BBBIO_ADC_INVALID) {
            kprintf("invalid pin\n");
            return false;
        }
#ifdef TMP_DEBUG		
		kprintf("enabling adc>>>\n");
#endif
        enable_adc(pinmap[pin].adcpin);
    } else {
        if (!pinmap[pin].pp.port && !pinmap[pin].pp.pin) {
            kprintf("invalid pin\n");
            return false;
        } else {
        	kprintf("%u %u\n", pinmap[pin].pp.port, pinmap[pin].pp.pin);
        }
    }

    return true;
}

int Thermometer::read() {
#ifdef TMP_DEBUG	
	kprintf("Thermometer::read()>>%u\n", pin);
#endif
	if (!type) {
	    int voltage = read_adc(pinmap[pin].adcpin);
	    return calculateTemperature((void *)&voltage);
	}
}

void Thermometer::write(bool data) {
	if (!type) {
		kprintf("unable to write data to thermometer");
	}
}

void Thermometer::notify(uint32 interval, uint32 duration, 
                            void (*notifycb)(void *data), void *data) {
    if (duration) {
		if (interval > 0) {
		    this->state    = NOTIFY_STATE;
			this->start    = clktime;
			this->duration = duration;
			this->data     = data;
			this->timer    = create_timer(notifycb, this, interval);
			start_timer(this->timer);
		}
    } else {
    	if (this->state == NOTIFY_STATE) {
			this->state = IDLE_STATE;
			this->duration = 0;
    	}
    }
}

void Thermometer::deinit() {
	if (!type) { //anlalog
		disable_adc(pinmap[pin].adcpin);
	} else {
	
	}
}

