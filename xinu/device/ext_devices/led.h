#ifndef __LED_H__
#define __LED_H__
#ifdef __cplusplus
extern "C" {
#endif

class Led : public Device {
    public :
        Led(pinname_t pin);
        virtual bool init(bool inout);
        virtual int32 read();
        virtual void write(bool data);
        virtual void notify(uint32 interval, uint32 duration, 
                            void (*notifycb)(void *data), void *data);
        virtual void deinit();
};

#ifdef __cplusplus
} //end extern "C"
#endif

#endif
