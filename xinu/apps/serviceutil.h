#ifndef __SERVICE_UTIL_H__
#define __SERVICE_UTIL_H__

#ifdef __cplusplus
extern "C" {
#endif

#define SERVICES_GET_UINT8_FROM_LITTLE_ENDIAN(ptr) ((uint8) ((uint8 *) (ptr))[0])
#define SERVICES_GET_UINT16_FROM_LITTLE_ENDIAN(ptr) (((uint16) ((uint8 *) (ptr))[0]) \
                                              | ((uint16) ((uint8 *) (ptr))[1]) << 8)
#define SERVICES_GET_UINT32_FROM_LITTLE_ENDIAN(ptr) (((uint32) ((uint8 *) (ptr))[0]) \
                                              | ((uint32) ((uint8 *) (ptr))[1]) << 8 | \
                                              ((uint32) ((uint8 *) (ptr))[2]) << 16 \
                                              | ((uint32) ((uint8 *) (ptr))[3]) << 24)
#define SERVICES_GET_INT32_FROM_LITTLE_ENDIAN(ptr) (((int32) ((char *) (ptr))[0]) \
                                                  | ((int32) ((char *) (ptr))[1]) << 8 | \
                                                  ((int32) ((char *) (ptr))[2]) << 16 \
                                                  | ((int32) ((char *) (ptr))[3]) << 24)


#define SERVICES_COPY_UINT8_TO_LITTLE_ENDIAN(uint, ptr)   ((uint8 *) (ptr))[0] = ((uint8)((uint) & 0xFF))
#define SERVICES_COPY_UINT16_TO_LITTLE_ENDIAN(uint, ptr)  ((uint8 *) (ptr))[0] = ((uint8)((uint) & 0x00FF)); \
                                                    ((uint8 *) (ptr))[1] = ((uint8) ((uint) >> 8))
#define SERVICES_COPY_UINT32_TO_LITTLE_ENDIAN(uint, ptr)  ((uint8 *) (ptr))[0] = ((uint8) ((uint) & 0x000000FF)); \
                                        ((uint8 *) (ptr))[1] = ((uint8) (((uint) >> 8) & 0x000000FF)); \
                                        ((uint8 *) (ptr))[2] = ((uint8) (((uint) >> 16) & 0x000000FF)); \
                                        ((uint8 *) (ptr))[3] = ((uint8) (((uint) >> 24) & 0x000000FF))
#define SERVICES_COPY_INT32_TO_LITTLE_ENDIAN(uint, ptr)  ((char *) (ptr))[0] = ((char) ((uint) & 0x000000FF)); \
                                            ((char *) (ptr))[1] = ((char) (((uint) >> 8) & 0x000000FF)); \
                                            ((char *) (ptr))[2] = ((char) (((uint) >> 16) & 0x000000FF)); \
                                            ((char *) (ptr))[3] = ((char) (((uint) >> 24) & 0x000000FF))

static inline void derive_sensor_name_from_payload (void hashfn(uint8*, 
                                                    uint8*), msgListEnt_t *msg,
                                                    uint8* sensorname,
                                                    uint8 *sensornameLength,
                                                    uint8 *sensorhash)
{
    if (msg->protoh.length > sizeof(uint8)) {
        *sensornameLength = SERVICES_GET_UINT8_FROM_LITTLE_ENDIAN(msg->payload);
        if(msg->protoh.length > sizeof(uint8) + *sensornameLength) {
            memcpy(sensorname, msg->payload+sizeof(uint8), *sensornameLength);
            hashfn(sensorname, sensorhash);
        }
    }
}

static inline void build_pkt_and_enqueue(uint8 _msgtype, uint16 _topic, \
                                uint8 payloadL, uint8 * payload, mqttQ_t *txQ) {
    msgListEnt_t* msg = NULL;
    msg = (msgListEnt_t *)getmem(sizeof(msgListEnt_t));
    msg->next           = NULL;
    msg->protoh.msgtype = _msgtype;
    msg->protoh.topic   = _topic;
    msg->protoh.length  = payloadL;
    msg->payload        = payload;
    mqtt_enqueue(txQ, msg);
    sendMsg(txQ->qOwner, TRUE);
}

#define serialize_and_copy_sensor_name(payload, sensorname, sensornameLength) \
{ \
    SERVICES_COPY_UINT8_TO_LITTLE_ENDIAN(sensornameLength, payload); \
	memcpy(payload+1, sensorname, sensornameLength); \
}

#ifdef __cplusplus
} //end extern "C"
#endif

#endif
