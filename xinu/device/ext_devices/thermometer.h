#ifndef __THERMOMETER_H__
#define __THERMOMETER_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    IDLE_STATE,
    NOTIFY_STATE,
} state_t;

class Thermometer : public Device {
    private:
        bool type;        // analog or digital
        void *notifycb;
        state_t state;
    public :
        time start;       // start time (sec)
        time duration;    // duration (sec)
        timerid timer;
        void *data;

        Thermometer(pinname_t pin, bool type);
        virtual bool init(bool inout);
        virtual int32 read();
        virtual void write(bool data);
        virtual void notify(uint32 interval, uint32 duration, 
                            void (*notifycb)(void *data), void *data);
        virtual void deinit();
        virtual int calculateTemperature(void* data) = 0;
};

#ifdef __cplusplus
} //end extern "C"
#endif

#endif
