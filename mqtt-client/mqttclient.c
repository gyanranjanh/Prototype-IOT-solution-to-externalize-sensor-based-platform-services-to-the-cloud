#include "mqttypes.h"
#include "mqttclient.h"
#include "mqttcliapi.h"

mqttContext_t mqttCtx = {0};

static void config_addr_setting(udpContext_t *udpContext,
                         uint16_t port,
                         uint8_t *addr) {
    /*Configure settings in address struct*/
#ifdef __linux__
    udpContext->serverAddr.sin_family      = AF_INET;
    udpContext->serverAddr.sin_port        = htons(port);
    udpContext->serverAddr.sin_addr.s_addr = inet_addr(addr);
    memset(udpContext->serverAddr.sin_zero, '\0', 
                             sizeof(udpContext->serverAddr.sin_zero));
#else
    udpContext->s_addr = inet_addr(addr);
#endif
    /*Initialize size variable to be used later on*/
}

void serialize_and_send_message(udpContext_t *udpContext,
                            mqttProtoheader_t *protoh,
                            void *payload) {
    uint8_t *frame = NULL, *frameh = NULL;
    uint32_t frameLen = 0, checkLen = 0;

#ifndef __linux__
#ifdef MQTT_TEST
    kprintf("serialize_and_send_message>>\n");
#endif
#endif

    /* proto header -
    * 1. bbbid (4)
    * 2. msgtype (1)
    * 3. topic (2)
    * 4. length (1)
    */

    /* serialize */
    frameLen = MQTT_PROTO_HEADER_LEN + protoh->length;
    frame = (uint8_t *) malloc(frameLen);
    MQTT_ASSERT((frame != NULL));
    frameh = frame;
    MQTT_COPY_UINT32_TO_LITTLE_ENDIAN(protoh->bbbid, frame);
    frame += 4;
    checkLen += 4;
    MQTT_COPY_UINT8_TO_LITTLE_ENDIAN(protoh->msgtype, frame);
    frame += 1;
    checkLen += 1;
    MQTT_COPY_UINT16_TO_LITTLE_ENDIAN(protoh->topic, frame);
    frame += 2;
    checkLen += 2;
    MQTT_COPY_UINT8_TO_LITTLE_ENDIAN(protoh->length, frame);
    frame += 1;
    checkLen += 1;

    if(payload != NULL) {
        memcpy(frame, payload, protoh->length);
        checkLen += protoh->length;
    }

    MQTT_ASSERT((frameLen == checkLen));

    /*Send message to server*/
#ifdef __linux__
    sendto(udpContext->udpSocket, frameh, frameLen, 0,
        (struct sockaddr *)&udpContext->serverAddr, sizeof(struct sockaddr));
    free(frameh);
#else
#ifdef MQTT_TEST
    char	str[128];
    uint32 ipaddr = udpContext->s_addr;
    sprintf(str, "%d.%d.%d.%d",
        (ipaddr>>24)&0xff, (ipaddr>>16)&0xff,
        (ipaddr>>8)&0xff,        ipaddr&0xff);
    kprintf("Sending to IP %s   (0x%08x)\n", str, ipaddr);
#endif

    udp_sendto(udpContext->slot, udpContext->s_addr, MQTT_UDP_SPORT, frameh, frameLen);
    freemem(frameh, frameLen);
#endif
}

static void* mqtt_txter() {
    msgListEnt_t         *msg = NULL;
    mqttProtoheader_t *protoh = NULL;

#ifndef __linux__
    kprintf("mqtt_txter\n");
#endif

    while (TRUE) {
        msg = mqtt_dequeue(&mqttCtx.txQ);

        if (msg != NULL) {
            msg->protoh.bbbid = MQTT_BBB_ID;
            protoh = &msg->protoh;
            serialize_and_send_message(&mqttCtx.udpContext, protoh, msg->payload);
#ifdef __linux__
            free(msg->payload);
            free(msg);
#else
            freemem(msg->payload, protoh->length);
            freemem(msg, sizeof(msgListEnt_t));
#endif
        }
#ifndef __linux__
        else {
            kprintf("txter going to sleep..\n");
            receiveMsg();
            kprintf("txter woke up\n");
        }
#endif
    }
}

void mqtt_tx_agent_init() {
#ifndef __linux__
    kprintf("mqtt_tx_agent_init\n");
#endif
    mqtt_mutex_create(&mqttCtx.txQ.qMutex);
    mqttCtx.txQ.msgQhead = NULL;
}

void mqtt_tx_agent_deinit() {
    mqtt_mutex_destroy(mqttCtx.txQ.qMutex);
}

#ifdef __linux__
int main(){
#else
int mqtt_start(){
#endif
    struct protoheader protoh;
    char  msg[3] ={0x26};
    pthread_t listener;
    pthread_t txter;

#ifndef __linux__
    kprintf("mqtt_start>>\n");
#endif

    mqtt_rx_agent_init();
    mqtt_tx_agent_init();

    config_addr_setting(&mqttCtx.udpContext, MQTT_UDP_SPORT, MQTT_TARGET_EDGE_ADDR);

#ifdef __linux__
    pthread_create(&listener, NULL, mqtt_listener, NULL);
    pthread_create(&txter, NULL, mqtt_txter, NULL);
#else
    resched_cntl(DEFER_START);
    listener = create(mqtt_listener, MQTT_STD_STK_SIZE, MQTT_LISNR_THD_PRIO, "listener", 0);
    txter = create(mqtt_txter, MQTT_STD_STK_SIZE, MQTT_TXTR_THD_PRIO, "txter", 0);
    resume(listener);
    resume(txter);
    resched_cntl(DEFER_STOP);
    mqttCtx.txter = txter;
    mqttCtx.txQ.qOwner = txter;
#endif


#ifdef __linux__
    pthread_join(listener, NULL);
    mqtt_rx_agent_deinit();
    mqtt_tx_agent_deinit();
#endif

    return 0;
}

#ifndef __linux__
void mqtt_stop ()
{
    mqtt_rx_agent_deinit();
    mqtt_tx_agent_deinit();
}
#endif

