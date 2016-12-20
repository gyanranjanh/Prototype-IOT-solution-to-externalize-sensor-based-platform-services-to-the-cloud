#ifndef __MQTT_UTIL_H__
#define __MQTT_UTIL_H__

#include "mqttcliapi.h"

mqttServiceNode_t* mqtt_get_sevice_node(mqttService_t svcid);

#ifndef __linux__
/* Internet address. */
struct in_addr {
    uint32_t  s_addr;     /* address in network byte order */
};
uint32 inet_addr(register const char *cp);
int inet_aton(register const char *cp, struct in_addr *addr);
#endif

#endif
