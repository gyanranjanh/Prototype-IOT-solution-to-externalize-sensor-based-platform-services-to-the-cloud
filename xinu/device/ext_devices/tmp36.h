#ifndef __TMP36_H__
#define __TMP36_H__

#ifdef __cplusplus
extern "C" {
#endif

class Tmp36 : public Thermometer {
    public:
        Tmp36(pinname_t pin, bool type);
        virtual int calculateTemperature(void *data);
};

#ifdef __cplusplus
} //end extern "C"
#endif

#endif
