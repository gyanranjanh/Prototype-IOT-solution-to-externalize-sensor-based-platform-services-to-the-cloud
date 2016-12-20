#ifndef __MQTT_CLIENT_H__
#define __MQTT_CLIENT_H__

#include "mqttypes.h"
#include "mqttopics.h"
#include "mqttcliapi.h"

#define MQTT_GET_UINT8_FROM_LITTLE_ENDIAN(ptr) ((uint8_t) ((uint8_t *) (ptr))[0])
#define MQTT_GET_UINT16_FROM_LITTLE_ENDIAN(ptr) (((uint16_t) ((uint8_t *) (ptr))[0]) \
                                              | ((uint16_t) ((uint8_t *) (ptr))[1]) << 8)
#define MQTT_GET_UINT32_FROM_LITTLE_ENDIAN(ptr) (((uint32_t) ((uint8_t *) (ptr))[0]) \
                                              | ((uint32_t) ((uint8_t *) (ptr))[1]) << 8 | \
                                              ((uint32_t) ((uint8_t *) (ptr))[2]) << 16 \
                                              | ((uint32_t) ((uint8_t *) (ptr))[3]) << 24)

#define MQTT_COPY_UINT8_TO_LITTLE_ENDIAN(uint, ptr)   ((uint8_t *) (ptr))[0] = ((uint8_t)((uint) & 0xFF))
#define MQTT_COPY_UINT16_TO_LITTLE_ENDIAN(uint, ptr)  ((uint8_t *) (ptr))[0] = ((uint8_t)((uint) & 0x00FF)); \
                                                    ((uint8_t *) (ptr))[1] = ((uint8_t) ((uint) >> 8))
#define MQTT_COPY_UINT32_TO_LITTLE_ENDIAN(uint, ptr)  ((uint8_t *) (ptr))[0] = ((uint8_t) ((uint) & 0x000000FF)); \
                                        ((uint8_t *) (ptr))[1] = ((uint8_t) (((uint) >> 8) & 0x000000FF)); \
                                        ((uint8_t *) (ptr))[2] = ((uint8_t) (((uint) >> 16) & 0x000000FF)); \
                                        ((uint8_t *) (ptr))[3] = ((uint8_t) (((uint) >> 24) & 0x000000FF))

#define MQTT_PROTO_HEADER_LEN (8)

typedef struct {
    mqttService_t svc;
    mqttQ_t svcQ;
} mqttServiceNode_t;

typedef struct _mqttRegistdSvcs {
    mqttServiceNode_t svc;
    struct _mqttRegistdSvcs *next;
} mqttRegistdSvcs_t;


typedef struct {
#ifdef __linux__
    int udpSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_in listenAddr;
    struct sockaddr_storage senderIP;
#else
    int32 slot;
    uint32 s_addr;
#endif
} udpContext_t;

typedef struct {
    uint8_t buffer[MQTT_BUFF_SIZE];
    udpContext_t udpContext;
    mqttQ_t txQ;
#ifndef __linux__
    pid32 txter;
#endif
    mqttRegistdSvcs_t *svcListH;
} mqttContext_t;

extern mqttContext_t mqttCtx;
void* mqtt_listener();
void mqtt_rx_agent_init();
void mqtt_rx_agent_deinit();

/* TBD */
/* void send_message_to_broker(udpContext_t *udpContext,
                mqttProtoheader_t *protoh, void *payload); */

#endif
