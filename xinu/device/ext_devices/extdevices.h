#ifndef __EXTDEVICES_H__
#define __EXTDEVICES_H__

#ifdef __cplusplus
extern "C" {
#endif

void __cxa_pure_virtual();

class Device {
    protected:
        pinname_t pin;
    public:
        Device(pinname_t _pin) : pin(_pin){};
        // in => inout = 0
        virtual bool init(bool inout) = 0;
        virtual int32 read() = 0;
        virtual void write(bool data) = 0;
        virtual void notify(uint32 interval, uint32 duration, 
                            void (*notifycb)(void *data), void *data) = 0;
        virtual void deinit() = 0;
};

#ifdef __cplusplus
} //end extern "C"
#endif


#endif
