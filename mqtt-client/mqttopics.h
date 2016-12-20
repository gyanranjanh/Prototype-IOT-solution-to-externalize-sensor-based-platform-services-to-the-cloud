#ifndef __MQTT_TOPIC_H__
#define __MQTT_TOPIC_H__

#ifdef __cplusplus
extern "C" {
#endif


#define MQTT_PUBLISH_MESSAGE     (1)
#define MQTT_SUBSCRIBE_MESSAGE   (2)
#define MQTT_UNSUBSCRIBE_MESSAGE (3)

typedef enum {
    MQTT_TOPIC_GET_TEMP_ANALOG = 0x01,
    MQTT_TOPIC_GET_TEMP_DIGITAL,
    MQTT_TOPIC_NOTIFY_TEMP_ANALOG,
    MQTT_TOPIC_NOTIFY_TEMP_DIGITAL,
    MQTT_TOPIC_CUR_TEMP_ANALOG,
    MQTT_TOPIC_CUR_TEMP_DIGITAL,
    MQTT_TOPIC_LED,
    MQTT_TOPIC_GET_LED_STATUS,
    MQTT_TOPIC_LED_STATUS,
} mqttTopic_t;

typedef enum {
    MQTT_SERVICE_THERMOMETER = 0x00,
    MQTT_SERVICE_BASIC_LED,
    MQTT_SERVICE_UNKNOWN,
    NO_MQTT_SERVICES = MQTT_SERVICE_UNKNOWN,
} mqttService_t;

static inline uint16 mqtt_get_service_id(uint16 topic) {
    uint16 srvc; 
    switch (topic) {
        case MQTT_TOPIC_GET_TEMP_ANALOG:
        case MQTT_TOPIC_NOTIFY_TEMP_ANALOG:
        case MQTT_TOPIC_GET_TEMP_DIGITAL:
        case MQTT_TOPIC_NOTIFY_TEMP_DIGITAL:
            srvc = MQTT_SERVICE_THERMOMETER;
            break;
        case MQTT_TOPIC_LED:
        case MQTT_TOPIC_GET_LED_STATUS:
            srvc = MQTT_SERVICE_BASIC_LED;
            break;
        default:
            srvc = MQTT_SERVICE_UNKNOWN;
    }
    return srvc;
}

#ifdef __cplusplus
} //end extern "C"
#endif


#endif
