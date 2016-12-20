#ifndef __MQTT_CLI_API_H__
#define __MQTT_CLI_API_H__

#include "mqttopics.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __linux__
extern int mqtt_start();
extern void mqtt_stop ();
#endif

typedef struct protoheader {
	uint32  bbbid;
	uint8   msgtype;
	uint16  topic;
	uint8   length;
} mqttProtoheader_t;

typedef struct _msglist_ent{
    mqttMutexHandle mutex;
	mqttProtoheader_t protoh;
	uint8  *payload;
	struct _msglist_ent *next;
} msgListEnt_t;

typedef struct {
#ifndef __linux__
    pid32 qOwner; /* service requestor */
#endif
    mqttMutexHandle qMutex;
    msgListEnt_t *msgQhead;
} mqttQ_t;

extern void mqtt_regiter_service (mqttService_t svcid, mqttQ_t **rxQ, mqttQ_t **txQ);
extern void mqtt_deregister_service (mqttService_t svcid);
extern void mqtt_enqueue(mqttQ_t *mqttq, msgListEnt_t *newmsg);
extern msgListEnt_t* mqtt_dequeue(mqttQ_t *mqttq);

#ifdef __cplusplus
} //end extern "C"
#endif

#endif
