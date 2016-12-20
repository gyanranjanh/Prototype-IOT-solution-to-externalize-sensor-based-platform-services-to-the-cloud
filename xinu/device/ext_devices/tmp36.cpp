#include <xinu_cpp.h>

Tmp36::Tmp36(pinname_t p, bool t) : Thermometer(p, t) {
#ifdef TMP_DEBUG
	kprintf("Tmp36>>%u\n", p);
#endif
	init(0);
}

int Tmp36::calculateTemperature(void* data) {
    return (((*((int *)data)*1800)/4096) - 500)/10;
}
