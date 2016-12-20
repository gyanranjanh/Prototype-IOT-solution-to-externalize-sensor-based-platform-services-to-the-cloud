#include "mqttypes.h"
#include "mqttclient.h"

#ifdef __linux__
static void get_client_socket(udpContext_t *udpContext) {
    udpContext->udpSocket = socket(PF_INET, SOCK_DGRAM, 0);
}

static void config_addr_setting(udpContext_t *udpContext,
                         uint16_t port) {
    /* Configure settings in address struct */

    udpContext->listenAddr.sin_family      = AF_INET;

    udpContext->listenAddr.sin_port        = htons(port);


    udpContext->listenAddr.sin_addr.s_addr = htonl(INADDR_ANY);



    memset(udpContext->listenAddr.sin_zero, '\0', 
                             sizeof(udpContext->listenAddr.sin_zero));

    /*Initialize size variable to be used later on*/
    get_client_socket(udpContext);

    /*Bind socket with address struct*/
    bind(udpContext->udpSocket, (struct sockaddr *) &udpContext->listenAddr, 
	                                            sizeof(udpContext->listenAddr));
}
#endif

void mqtt_rx_agent_init()
{
#ifdef __linux__
    config_addr_setting(&mqttCtx.udpContext, MQTT_UDP_CPORT);
#else
    kprintf("mqtt_rx_agent_init\n");
    mqttCtx.udpContext.slot = udp_register(0, MQTT_UDP_SPORT, MQTT_UDP_CPORT);
	if (mqttCtx.udpContext.slot == SYSERR) {
		kprintf("mqtt_rx_agent_init: cannot register with UDP\n");
	}
#endif
}

void mqtt_rx_agent_deinit()
{
#ifndef __linux__
    udp_release(mqttCtx.udpContext.slot);
#endif
}

void* mqtt_listener()
{
	msgListEnt_t *newmsg = NULL;
    mqttServiceNode_t *svcNode = NULL;

    uint8_t *bufferh = NULL;
    uint8_t checkLength = 0;
	int recvBytes, i;
    uint16 svcid;
#ifdef __linux__
    size_t addr_size = 0;
    addr_size = sizeof(mqttCtx.udpContext.senderIP);
#else
    kprintf("mqtt_listener>>\n");
#endif

	while(TRUE)
	{
	    checkLength = 0;
        memset(mqttCtx.buffer, 0, MQTT_BUFF_SIZE);
		/* Try to receive any incoming UDP datagram. Address and port of 
		requesting client will be stored on senderIP variable */

#ifdef __linux__
		recvBytes = recvfrom(mqttCtx.udpContext.udpSocket, mqttCtx.buffer, MQTT_BUFF_SIZE, 0,
		       (struct sockaddr *)&mqttCtx.udpContext.senderIP, &addr_size);
#else
        kprintf("waiting to receive in sleep mode..\n");
        recvBytes = udp_recv(mqttCtx.udpContext.slot, mqttCtx.buffer, MQTT_BUFF_SIZE, 0x0fffffff);
#endif

#ifndef __linux__
        if (recvBytes != TIMEOUT) {
#endif
#ifdef MQTT_TEST
            printf("recvd bytes: %d\n", recvBytes);
#endif
    		bufferh = mqttCtx.buffer;

    		/* allocate memory for new message */
    		newmsg = (msgListEnt_t *) malloc(sizeof(msgListEnt_t));
            MQTT_ASSERT((newmsg != NULL));
            memset(newmsg, 0, sizeof(msgListEnt_t));
            newmsg->protoh.bbbid   = MQTT_GET_UINT32_FROM_LITTLE_ENDIAN(bufferh);
            bufferh += 4;
            checkLength += 4;
            newmsg->protoh.msgtype = MQTT_GET_UINT8_FROM_LITTLE_ENDIAN(bufferh);
            bufferh += 1;
            checkLength += 1;
            newmsg->protoh.topic   = MQTT_GET_UINT16_FROM_LITTLE_ENDIAN(bufferh);
            bufferh += 2;
            checkLength += 2;
            newmsg->protoh.length  = MQTT_GET_UINT8_FROM_LITTLE_ENDIAN(bufferh);
            bufferh += 1;
            checkLength += 1;
            MQTT_ASSERT((checkLength <= recvBytes));

            if (newmsg->protoh.length) {
                /* allocate mem for payload */
                newmsg->payload = (uint8_t *)malloc(newmsg->protoh.length);
                MQTT_ASSERT((newmsg->payload != NULL));
                memcpy(newmsg->payload, bufferh, newmsg->protoh.length);
           }


    		/* printing for testing purposes */
#ifdef MQTT_TEST
    		printf("bbid %u\n", newmsg->protoh.bbbid);
    		printf("%u\n", newmsg->protoh.msgtype);
    		printf("%u\n", newmsg->protoh.topic);
    		printf("%u\n", newmsg->protoh.length);
            printf("\n");
            for(i = 0; i < newmsg->protoh.length; i++) {
                printf("0x%x\n", newmsg->payload[i]);
            }
#endif

            if (newmsg->protoh.bbbid == MQTT_BBB_ID) {
                svcNode = NULL;
                svcid = mqtt_get_service_id(newmsg->protoh.topic);
#ifdef MQTT_TEST
                kprintf("svcid: %u\n", svcid);
#endif
                if (svcid != MQTT_SERVICE_UNKNOWN) {
                    /* get service node from registered serives list */
                    svcNode = mqtt_get_sevice_node(svcid);
                    if (svcNode != NULL) {
                        mqtt_enqueue(&svcNode->svcQ, newmsg);
#ifndef __linux__
                        sendMsg(svcNode->svcQ.qOwner, TRUE);
#endif
                    }
                }
            }
            else {
                kprintf("msg to bbb %u is dropped\n", newmsg->protoh.bbbid);
            }

            if ((newmsg->protoh.bbbid != MQTT_BBB_ID) || (svcNode == NULL)) {
#ifdef __linux__
                free(newmsg->payload);
                free(newmsg);
#else
                freemem(newmsg->payload, newmsg->protoh.length);
                freemem(newmsg, sizeof(msgListEnt_t));
#endif
            }

#ifndef __linux__
        }
#endif
    }

#ifndef __linux__
    sendMsg(mqttCtx.txter, TRUE);
#endif
}
