/*
 * FILE:	or_os_helper.c
 * AUTHOR:	Gyanranjan Hazarika - gyanranjanh@gmail.com
 * 
 * Copyright (c) 2015-2016, Gyanranjan Hazarika
 * All rights reserved.
 *
 */
#include "edgetypes.h"
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

    *mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));

    if (pthread_mutex_init(*mutex, NULL) == 0)
    {
        result = TRUE;
    }

OUT:
    return result;
}

/* --------------------------------------------------------------------------
 * Destroys the mutex so that the associate resources are freed
 */
void mqtt_mutex_destroy(mqttMutexHandle mutex)
{
    if(mutex != NULL) {
        pthread_mutex_destroy(mutex);
        free(mutex);
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

    if (pthread_mutex_lock(mutex) == 0)
    {
        result = TRUE;
    }

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

    if (pthread_mutex_unlock(mutex) == 0)
    {
        result = TRUE;
    }

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

    if(!sem_init(sema, 0, 1))
    {
        result = TRUE;
    }

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

    if(!sem_wait(sema))
    {
        result = TRUE;
    }

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

    if(!sem_post(sema))
    {
        result = TRUE;
    }

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

    if(!sem_destroy(sema))
    {
        result = TRUE;
    }

OUT:
    return result;
}

#ifndef __linux__
void *malloc(uint32 size) {
    return (void *)getmem(size);
}
#endif