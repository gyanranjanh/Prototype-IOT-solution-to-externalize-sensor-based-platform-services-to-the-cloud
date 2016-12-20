#ifndef __LED_SERVICE_H__
#define __LED_SERVICE_H__

#ifdef __cplusplus
extern "C" {
#endif

#define LED_ID_LED1 (0x01)
#define LED_ID_LED2 (0x02)
#define LED_ID_LED2 (0x03)

static inline void ledNameHash( uint8* sensorname, uint8 *sensorhash) 
{ 
    if (!strncmp((const char *)sensorname, "LED1", 4)) 
    { 
       *sensorhash = LED_ID_LED1; 
    } else if (!strncmp((const char *)sensorname, "LED3", 4)) { \
       *sensorhash = LED_ID_LED2;
    }
}

void ledServiceFn();

#ifdef __cplusplus
} //end extern "C"
#endif

#endif
