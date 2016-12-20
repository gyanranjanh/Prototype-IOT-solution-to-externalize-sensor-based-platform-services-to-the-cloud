#ifndef __EDGE_EVENTS_H__
#define __EDGE_EVENTS_H__

typedef void (*topic_handler) (topic16, void *, uint32, uint32_t);

typedef struct _subscriber_tuple {
	uint32_t bbbid;
	topic_handler handler;
}subscriber_tuple_t;

typedef struct _bbbid2ip_tuple {
	uint32_t bbbid;
	struct sockaddr_storage IPaddrss;
}bbbid2ip_tuple_t; 

typedef struct  _subscriber_info {
	uint8 gpid;
	uint8 no_gpsubscbr;
	subscriber_tuple_t bbbid2handler_map[MAX_NO_SUBSCRIBER];
	struct _subscriber_info *next;
}subscriber_info_t;

typedef struct _topic_data {
	uint8 gpid;
	void *data;
	uint32 size;
	struct _topic_data *next;
}topic_data_t;

typedef struct _topic_data_tuple {
	topic16 topic;
	void *data;
	uint32 size;
}topic_data_tuple_t;

typedef struct _topic_info {
	uint8 no_subscbr;
	subscriber_info_t *subinfo_head;
	topic_data_t *topicd_head;
}topic_t;

void init_topics ();
int32 subscribe(topic16 topic, topic_handler handler, uint32_t bbbid);
int32 unsubscribe(topic16 topic, uint32_t bbbid );
int32 publish(topic16 topic, void *data, uint32 size);
void deinit_topics ();

extern topic_t topics[];

#endif
