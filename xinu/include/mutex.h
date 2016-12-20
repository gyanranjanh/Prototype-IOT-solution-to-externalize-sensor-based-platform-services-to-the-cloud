/* mutex.h */
#ifdef __cplusplus
extern "C" {
#endif

mutx32 create_mutex();
void destroy_mutex(mutx32 mx);
void mutex_acquire(mutx32 mx);
void mutex_release(mutx32 mx);

#ifdef __cplusplus
} //end extern "C"
#endif

