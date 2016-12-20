/* edge.c - */
#include "edgetypes.h"
#include "mqtt_os_helper.h"
#include "events.h"
#include "edge.h"

typedef struct {
    int clientSocket;
    struct sockaddr_storage destAddr;
    socklen_t addr_size;
} udpTxContext_t;


mqttContext_t mqttCtx = {0};

/*----------------------------------------------------------------------*/

static bool is_bbbid_registered(msgListEnt_t *newmsg) {
    bool flag = FALSE;
    uint32_t i;
    
	/* Search if BBB already registered on edge */
	for(i=0; i < mqttCtx.BBBscnnct_cnt;i++)
	{
		if(mqttCtx.bbbid2IP_map[i].bbbid == newmsg->protoh.bbbid)
		{
			flag = TRUE;
		}
			
	}
    return flag;
}

static void mqtt_enqueue(msgListEnt_t *newmsg) {
    msgListEnt_t *curr = NULL;

#ifdef EDGE_TEST
    printf("mqtt_enqueue>>\n");
#endif
    mqtt_mutex_lock(mqttCtx.qMutex);
	curr = mqttCtx.msgQhead;

	/* find last element in list */
	if(curr == NULL)
	{
		mqttCtx.msgQhead = newmsg;
	} else {
		while(curr->next != NULL)
		{
			curr = curr->next;
		}
        /* add new message to list */
		curr->next = newmsg;
    }
    mqtt_mutex_unlock(mqttCtx.qMutex);
}

static msgListEnt_t* mqtt_dequeue() {
    msgListEnt_t *curr = NULL;

    mqtt_mutex_lock(mqttCtx.qMutex);
	/* remove message from Q */
    if (mqttCtx.msgQhead != NULL) {
        
#ifdef EDGE_TEST
        printf("mqtt_dequeue>>\n");
#endif
    	curr = mqttCtx.msgQhead;
    	mqttCtx.msgQhead = mqttCtx.msgQhead->next;
    }
    mqtt_mutex_unlock(mqttCtx.qMutex);
    return curr;
}

void get_client_socket(udpContext_t *udpContext) {
    udpContext->udpSocket = socket(PF_INET, SOCK_DGRAM, 0);
}

void config_addr_setting(udpContext_t *udpContext,
                         uint16_t port,
                         uint8_t *addr) {
    /* Configure settings in address struct */
    udpContext->serverAddr.sin_family      = AF_INET;
    udpContext->serverAddr.sin_port        = htons(port);
#ifdef EDGE_LOCAL_IP
    udpContext->serverAddr.sin_addr.s_addr = inet_addr(addr);
#else
    udpContext->serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
#endif
    memset(udpContext->serverAddr.sin_zero, '\0', 
                             sizeof(udpContext->serverAddr.sin_zero));

    /*Initialize size variable to be used later on*/
    get_client_socket(udpContext);

    /*Bind socket with address struct*/
	bind(udpContext->udpSocket, (struct sockaddr *) &udpContext->serverAddr, 
	                                            sizeof(udpContext->serverAddr));
}

void* listenfunc()
{
	msgListEnt_t *newmsg = NULL;
    uint8_t* addr = (uint8 *)"127.0.0.1";
    uint8_t *bufferh = NULL;
    uint8_t checkLength = 0;
	int recvBytes, i;
    size_t addr_size = 0;
    struct sockaddr_in *sin;

	/* Configure settings in address struct */
    config_addr_setting(&mqttCtx.udpContext, 7894, addr);
    addr_size = sizeof(mqttCtx.udpContext.senderIP);

	while(TRUE)
	{
	    checkLength = 0;
        memset(mqttCtx.buffer, 0, BUFF_SIZE);
		/* Try to receive any incoming UDP datagram. Address and port of 
		requesting client will be stored on senderIP variable */
		recvBytes = recvfrom(mqttCtx.udpContext.udpSocket, mqttCtx.buffer, BUFF_SIZE, 0,
		       (struct sockaddr *)&mqttCtx.udpContext.senderIP, &addr_size);
#ifdef EDGE_TEST
        printf("%d\n", recvBytes);
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

#ifdef EDGE_TEST
		//printing for testing purposes
		printf("%u\n", newmsg->protoh.bbbid);
		printf("%u\n", newmsg->protoh.msgtype);
		printf("%u\n", newmsg->protoh.topic);
		printf("%u\n", newmsg->protoh.length);
        printf("\n");
        for(i = 0; i < newmsg->protoh.length; i++) {
            printf("0x%x\n", newmsg->payload[i]);
        }
#endif

        sin = (struct sockaddr_in *)&mqttCtx.udpContext.senderIP;
        unsigned char *ip = (unsigned char *)&sin->sin_addr.s_addr;
        printf("From:> %d.%d.%d.%d:", ip[0], ip[1], ip[2], ip[3]);
        printf("%u\n", sin->sin_port);

		if(!is_bbbid_registered(newmsg))
		{
#ifdef EDGE_TEST
		    printf("BBBscnnct_cnt %u\n", mqttCtx.BBBscnnct_cnt);
#endif
            sin->sin_port = 55326;
            mqttCtx.udpContext.senderIP = *((struct sockaddr_storage *) sin);
			mqttCtx.bbbid2IP_map[mqttCtx.BBBscnnct_cnt].bbbid = newmsg->protoh.bbbid;
			mqttCtx.bbbid2IP_map[mqttCtx.BBBscnnct_cnt++].IPaddrss = mqttCtx.udpContext.senderIP;
		}

        mqtt_enqueue(newmsg);
	}	
}

static void serialize_and_send_message(udpTxContext_t *udpContext,
                            mqttProtoheader_t *protoh,
                            void *payload) {
    uint8_t *frame = NULL, *frameh = NULL;
    uint32_t frameLen = 0, checkLen = 0;

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
    memset(frame, 0, frameLen);
    frameh = frame;
    MQTT_COPY_UINT32_TO_LITTLE_ENDIAN(protoh->bbbid, frameh);
    frameh += 4;
    checkLen += 4;
    MQTT_COPY_UINT8_TO_LITTLE_ENDIAN(protoh->msgtype, frameh);
    frameh += 1;
    checkLen += 1;
    MQTT_COPY_UINT16_TO_LITTLE_ENDIAN(protoh->topic, frameh);
    frameh += 2;
    checkLen += 2;
    MQTT_COPY_UINT8_TO_LITTLE_ENDIAN(protoh->length, frameh);
    frameh += 1;
    checkLen += 1;

    if(payload != NULL) {
        memcpy(frameh, payload + 4, protoh->length);
        checkLen += protoh->length;
    }
    MQTT_ASSERT((frameLen == checkLen));

#ifdef EDGE_TEST
    printf("frameL: %u\n", frameLen);
#endif
    /*Send message to server*/
    sendto(udpContext->clientSocket, frame, frameLen, 0,
            (struct sockaddr *)&udpContext->destAddr, udpContext->addr_size);
    free(frame);
}

void topic_hdlr(topic16 topic, void *data, uint32 size, uint32_t bbbid )
{
	uint32 i;
    struct sockaddr_in *sin = NULL;
	socklen_t addr_size;
    mqttProtoheader_t protoh = {0};
    udpTxContext_t udpContext = {0};

#ifdef EDGE_TEST
    printf("topic_hdlr>> t:%u s: %u b:%u\n", topic, size, bbbid);
#endif

    protoh.bbbid   = MQTT_GET_UINT32_FROM_LITTLE_ENDIAN(data);
    protoh.msgtype = MQTT_PUBLISH_MESSAGE;
    protoh.topic   = topic;
    protoh.length  = size - 4;

	for(i=0;i<mqttCtx.BBBscnnct_cnt;i++)
	{
		if(mqttCtx.bbbid2IP_map[i].bbbid == bbbid)
		{
#ifdef EDGE_TEST
            printf("topic_hdlr>> ip found\n");
            /* printf("BBBscnnct_cnt %u\n", i); */
#endif
            udpContext.destAddr = mqttCtx.bbbid2IP_map[i].IPaddrss;
            break;
		}
	}

    sin = (struct sockaddr_in *)&udpContext.destAddr;
    unsigned char *ip = (unsigned char *)&sin->sin_addr.s_addr;
    printf("To:. %d.%d.%d.%d:", ip[0], ip[1], ip[2], ip[3]);
    printf("%u\n", sin->sin_port);

	/*Create UDP socket*/
    udpContext.clientSocket = mqttCtx.udpContext.udpSocket;

	/*Initialize size variable to be used later on*/
	udpContext.addr_size = sizeof(udpContext.destAddr);

	/*Send message to server*/
    serialize_and_send_message(&udpContext, &protoh, data);
}

void* messageclassifier()
{
	msgListEnt_t *msg = NULL;
	while(1)
	{
	    msg = mqtt_dequeue();
		if(msg != NULL)
		{
			if(msg->protoh.msgtype == MQTT_PUBLISH_MESSAGE)
			{
			    uint8 *payload = NULL;
                payload = (uint8 *) malloc(msg->protoh.length + 4);
                if (payload) {
                    memset(payload, 0, (msg->protoh.length + 4));
                    MQTT_COPY_UINT32_TO_LITTLE_ENDIAN(msg->protoh.bbbid, payload);
                    memcpy(payload+4, msg->payload, msg->protoh.length);
    				publish(msg->protoh.topic, 
                                    payload, msg->protoh.length+4);
                }
			}
			else if(msg->protoh.msgtype == MQTT_SUBSCRIBE_MESSAGE)
			{
				subscribe(msg->protoh.topic, topic_hdlr, 
				msg->protoh.bbbid);
			}
			else if(msg->protoh.msgtype == MQTT_UNSUBSCRIBE_MESSAGE)
			{
				unsubscribe(msg->protoh.topic, msg->protoh.bbbid);
			}
			free(msg->payload);
            free(msg);
            msg = NULL;
		}
	}
}

void mqtt_init() {
	mqttCtx.BBBscnnct_cnt = 0;
    mqtt_mutex_create(&mqttCtx.qMutex);
	init_topics();
}

void mqtt_deinit() {
    mqtt_mutex_destroy(mqttCtx.qMutex);
}

int main()
{
	pthread_t listener, classifier;
    mqtt_init();
	pthread_create(&listener,NULL,listenfunc,NULL);
	pthread_create(&classifier,NULL,messageclassifier,NULL);
	pthread_join(listener,NULL);
	pthread_join(classifier,NULL);
    mqtt_deinit();
    return 0;
}
