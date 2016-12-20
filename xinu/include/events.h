/* events.h */

#ifdef __cplusplus
extern "C" {
#endif


#define MAX_NO_TOPICS 256
#define MAX_NO_SUBSCRIBER 8

typedef void (*topic_handler) (topic16, void *, uint32);

void init_topics ();
syscall subscribe(topic16 topic, topic_handler handler);
syscall unsubscribe(topic16 topic);
syscall publish(topic16 topic, void *data, uint32 size);
void deinit_topics ();

#ifdef __cplusplus
} //end extern "C"
#endif

