#include "mqttypes.h"
#include "mqttclient.h"
#include "mqttutil.h"

#ifndef __linux__
/*
 * Ascii internet address interpretation routine.
 * The value returned is in network order.
 */
uint32 inet_addr(cp)
	register const char *cp;
{
	struct in_addr val;

	if (inet_aton(cp, &val))
		return (val.s_addr);
	return (IP_ALLZEROS);
}

/* 
 * Check whether "cp" is a valid ascii representation
 * of an Internet address and convert to a binary address.
 * Returns 1 if the address is valid, 0 if not.
 * This replaces inet_addr, the return value from which
 * cannot distinguish between failure and a local broadcast address.
 */
int
inet_aton(cp, addr)
	register const char *cp;
	struct in_addr *addr;
{
	register uint32 val;
	register int base, n;
	register char c;
	uint32 parts[4];
	register uint32 *pp = parts;

	c = *cp;
	for (;;) {
		/*
		 * Collect number up to ``.''.
		 * Values are specified as for C:
		 * 0x=hex, 0=octal, isdigit=decimal.
		 */
		if (!isdigit(c))
			return (0);
		val = 0; base = 10;
		if (c == '0') {
			c = *++cp;
			if (c == 'x' || c == 'X')
				base = 16, c = *++cp;
			else
				base = 8;
		}
		for (;;) {
			if (isascii(c) && isdigit(c)) {
				val = (val * base) + (c - '0');
				c = *++cp;
			} else if (base == 16 && isascii(c) && isxdigit(c)) {
				val = (val << 4) |
					(c + 10 - (islower(c) ? 'a' : 'A'));
				c = *++cp;
			} else
				break;
		}
		if (c == '.') {
			/*
			 * Internet format:
			 *	a.b.c.d
			 *	a.b.c	(with c treated as 16 bits)
			 *	a.b	(with b treated as 24 bits)
			 */
			if (pp >= parts + 3)
				return (0);
			*pp++ = val;
			c = *++cp;
		} else
			break;
	}
	/*
	 * Check for trailing characters.
	 */
	if (c != '\0' && (!isascii(c) || !isspace(c)))
		return (0);
	/*
	 * Concoct the address according to
	 * the number of parts specified.
	 */
	n = pp - parts + 1;
	switch (n) {

	case 0:
		return (0);		/* initial nondigit */

	case 1:				/* a -- 32 bits */
		break;

	case 2:				/* a.b -- 8.24 bits */
		if (val > 0xffffff)
			return (0);
		val |= parts[0] << 24;
		break;

	case 3:				/* a.b.c -- 8.8.16 bits */
		if (val > 0xffff)
			return (0);
		val |= (parts[0] << 24) | (parts[1] << 16);
		break;

	case 4:				/* a.b.c.d -- 8.8.8.8 bits */
		if (val > 0xff)
			return (0);
		val |= (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8);
		break;
	}
#if 0
    char	str[128];
    uint32 ipaddr = val;
    sprintf(str, "%d.%d.%d.%d",
        (ipaddr>>24)&0xff, (ipaddr>>16)&0xff,
        (ipaddr>>8)&0xff,        ipaddr&0xff);
    kprintf("IP address  %s   (0x%08x)\n", str, ipaddr);
#endif
	if (addr)
        addr->s_addr = val;
		/* addr->s_addr = htonl(val); */
	return (1);
}

#endif



void mqtt_enqueue(mqttQ_t *mqttq, msgListEnt_t *newmsg) {
    msgListEnt_t *curr = NULL;

#ifdef MQTT_TEST
    kprintf("mqtt_enqueue>>\n");
#endif
    mqtt_mutex_lock(mqttq->qMutex);
	curr = mqttq->msgQhead;

	/* find last element in list */
	if(curr == NULL)
	{
		mqttq->msgQhead = newmsg;
	} else {
		while(curr->next != NULL)
		{
			curr = curr->next;
		}
        /* add new message to list */
    	curr->next = newmsg;
    }
    mqtt_mutex_unlock(mqttq->qMutex);
}

msgListEnt_t* mqtt_dequeue(mqttQ_t *mqttq) {
    msgListEnt_t *curr = NULL;

    mqtt_mutex_lock(mqttq->qMutex);
	/* remove message from Q */
    if (mqttq->msgQhead != NULL) {
        
#ifdef MQTT_TEST
        kprintf("mqtt_dequeue>>\n");
#endif
    	curr = mqttq->msgQhead;
    	mqttq->msgQhead = mqttq->msgQhead->next;
    }
    mqtt_mutex_unlock(mqttq->qMutex);
    return curr;
}

void mqtt_regiter_service (mqttService_t svcid,
                           mqttQ_t **rxQ,
                           mqttQ_t **txQ) {
    mqttRegistdSvcs_t *node = NULL, *temp = NULL;

#ifndef __linux__
    kprintf("mqtt_regiter_service>>\n");
#endif

    node = (mqttRegistdSvcs_t *)malloc(sizeof(mqttRegistdSvcs_t));
    MQTT_ASSERT((node != NULL));

    node->svc.svc = svcid;
#ifndef __linux__
    node->svc.svcQ.qOwner = getpid();
#endif
    mqtt_mutex_create(&node->svc.svcQ.qMutex);
    node->svc.svcQ.msgQhead = NULL;

    if(mqttCtx.svcListH == NULL) {
        mqttCtx.svcListH = node;
    } else {
        temp = mqttCtx.svcListH;
        while (temp->next != NULL) temp = temp->next;
        temp->next = node;
    }

    *rxQ = &node->svc.svcQ;
    *txQ = &mqttCtx.txQ;
}

void mqtt_deregister_service (mqttService_t svcid) {
    mqttRegistdSvcs_t *node = NULL;
    msgListEnt_t *msg = NULL, *temp = NULL;

#ifndef __linux__
        kprintf("mqtt_deregister_service>>\n");
#endif

    if (mqttCtx.svcListH) {
        node = mqttCtx.svcListH;
        while (node != NULL) {
            if (node->svc.svc == svcid) {
                break;
            }
            node = node->next;
        }
        mqtt_mutex_destroy(node->svc.svcQ.qMutex);
        msg = node->svc.svcQ.msgQhead;
        while (msg != NULL) {
            temp = msg->next;
#ifdef __linux__
            free(msg->payload);
            free(msg);
#else
            freemem(msg->payload, msg->protoh.length);
            freemem(msg, sizeof(msgListEnt_t));
#endif
            msg = temp;
        }
    }
}

mqttServiceNode_t* mqtt_get_sevice_node(mqttService_t svcid) {
    mqttRegistdSvcs_t *node = NULL;
    mqttServiceNode_t *svc = NULL;

    node = mqttCtx.svcListH;
    while (node != NULL) {
        if (node->svc.svc == svcid) {
            break;
        }
        node = node->next;
    }

    if (node) {
        svc = &node->svc;
    }

    return svc;
}

