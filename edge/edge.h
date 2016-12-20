#ifndef __EDGE_H__
#define __EDGE_H__

/* #define EDGE_TEST */

#define BUFF_SIZE 1024

#define MQTT_PUBLISH_MESSAGE     (1)
#define MQTT_SUBSCRIBE_MESSAGE   (2)
#define MQTT_UNSUBSCRIBE_MESSAGE (3)


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
typedef struct protoheader
{
	uint32_t  bbbid;
	uint8_t   msgtype;
	uint16_t  topic;
	uint8_t   length;
} mqttProtoheader_t;

typedef struct _msglist_ent{
    mqttMutexHandle mutex;
	mqttProtoheader_t protoh;
	uint8_t  *payload;
	struct _msglist_ent *next;
} msgListEnt_t;


typedef struct {
    int udpSocket;
    struct sockaddr_in serverAddr;
	struct sockaddr_storage senderIP;
} udpContext_t;

typedef struct {
    mqttMutexHandle qMutex;
    uint8_t buffer[BUFF_SIZE];
    bbbid2ip_tuple_t bbbid2IP_map[2048];
    uint32_t BBBscnnct_cnt;
    topic_t topics[MAX_NO_TOPICS];
    msgListEnt_t *msgQhead;
    udpContext_t udpContext;
    pthread_t bkr;
} mqttContext_t;

extern mqttContext_t mqttCtx;
#endif
