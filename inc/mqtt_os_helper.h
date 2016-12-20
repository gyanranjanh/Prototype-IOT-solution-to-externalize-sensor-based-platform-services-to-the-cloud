/*
 * FILE:	mqtt_os_helper.h
 * AUTHOR:	Gyanranjan Hazarika - gyanranjanh@gmail.com
 * 
 * Copyright (c) 2015-2016, Gyanranjan Hazarika
 * All rights reserved.
 *
 */

#ifndef __MQTT_OS_HELPER_H__
#define __MQTT_OS_HELPER_H__


bool mqtt_mutex_create(mqttMutexHandle* mutex);
void mqtt_mutex_destroy(mqttMutexHandle mutex);
bool mqtt_mutex_lock(mqttMutexHandle mutex);
bool mqtt_mutex_unlock(mqttMutexHandle mutex);
bool mqtt_sema_create(mqttSemaHandle *sema);
bool mqtt_sema_wait(mqttSemaHandle *sema);
bool mqtt_sema_release(mqttSemaHandle *sema);
bool mqtt_sema_destroy(mqttSemaHandle *sema);
#ifndef __linux__
void *malloc(uint32 size);
#endif

#endif /* __MQTT_OS_HELPER_H__ */

