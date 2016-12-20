/*
 * FILE:	or_os_helper.c
 * AUTHOR:	Gyanranjan Hazarika - gyanranjanh@gmail.com
 * 
 * Copyright (c) 2015-2016, Gyanranjan Hazarika
 * All rights reserved.
 *
 */
#include "mqttypes.h"
#include "mqtt_os_helper.h"

/* --------------------------------------------------------------------------
 * Initialise the mutex so that it can be used to lock
 * areas of code */
bool mqtt_mutex_create(mqttMutexHandle *mutex)
{
    bool result = FALSE;

    if (mutex == NULL)
    {
        goto OUT;;
    }

#ifdef __linux__
    *mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));

    if (pthread_mutex_init(*mutex, NULL) == 0)
    {
        result = TRUE;
    }
#else
    *mutex = semcreate(1);
    result = TRUE;
#endif


OUT:
    return result;
}

/* --------------------------------------------------------------------------
 * Destroys the mutex so that the associate resources are freed
 */
void mqtt_mutex_destroy(mqttMutexHandle mutex)
{
    if(mutex != NULL) {
#ifdef __linux__
        pthread_mutex_destroy(mutex);
        free(mutex);
#else
        (void)semdelete(mutex);
#endif
    }
}

/* --------------------------------------------------------------------------
 * Marks the code following this function as critical. This means no other
 * context that uses the same critical section handle may execute the code
 */
bool mqtt_mutex_lock(mqttMutexHandle mutex)
{
    bool result = FALSE;

    if (mutex == NULL)
    {
        goto OUT;
    }

#ifdef __linux__
    if (pthread_mutex_lock(mutex) == 0)
    {
        result = TRUE;
    }
#else
    mutex_acquire(mutex);
    result = TRUE;
#endif


OUT:
    return result;
}

/* --------------------------------------------------------------------------
 * Marks the end of the critical section - many execution contexts may
 * execute the code after this call.
 */
bool mqtt_mutex_unlock(mqttMutexHandle mutex)
{
    bool result = FALSE;

    if (mutex == NULL)
    {
        goto OUT;
    }

#ifdef __linux__
    if (pthread_mutex_unlock(mutex) == 0)
    {
        result = TRUE;
    }
#else
    mutex_release(mutex);
    result = TRUE;
#endif


OUT:
    return result;
}

bool mqtt_sema_create(mqttSemaHandle *sema) 
{
    bool result = FALSE;


    if (sema == NULL)
    {
        goto OUT;;
    }

#ifdef __linux__
    if(!sem_init(sema, 0, 1))
    {
        result = TRUE;
    }
#endif

OUT:
    return result;
}

bool mqtt_sema_wait(mqttSemaHandle *sema)
{
    bool result = FALSE;


    if (sema == NULL)
    {
        goto OUT;;
    }

#ifdef __linux__
    if(!sem_wait(sema))
    {
        result = TRUE;
    }
#endif

OUT:
    return result;
}

bool mqtt_sema_release(mqttSemaHandle *sema)
{
    bool result = FALSE;

    if (sema == NULL)
    {
        goto OUT;;
    }

#ifdef __linux__
    if(!sem_post(sema))
    {
        result = TRUE;
    }
#endif

OUT:
    return result;
}

bool mqtt_sema_destroy(mqttSemaHandle *sema) 
{
    bool result = FALSE;

    if (sema == NULL)
    {
        goto OUT;;
    }

#ifdef __linux__
    if(!sem_destroy(sema))
    {
        result = TRUE;
    }
#endif


OUT:
    return result;
}

#ifndef __linux__
void *malloc(uint32 size) {
    return (void *)getmem(size);
}
#endif