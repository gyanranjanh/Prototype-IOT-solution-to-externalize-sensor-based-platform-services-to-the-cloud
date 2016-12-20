#include <xinu_cpp.h>
#include <services.h>
#include <mqttcliapi.h>
#include <serviceutil.h>
#include <thermometerservice.h>

#ifdef __linux__
#define freemem(a, b) \
{ \
	free(a); \
}
#endif

ThermometerNotifier *activeNotifier = NULL;

void notifycb(void *data) {
	Tmp36 *t = (Tmp36 *)data;
	ThermometerNotifyData *tdata = (ThermometerNotifyData *)t->data;
	int32 temp;
	char *payload;
	uint8 payloadL = SERVICE_STD_PAYLOAD_LEN+SERVICE_SENSOR_NAME_FLD_LEN+tdata->sensornameLength;
	uint8 payloadOffset = SERVICE_SENSOR_NAME_FLD_LEN+tdata->sensornameLength;
	uint32 exprdTime = clktime - t->start;

	kprintf("notifycb>>%d\n", tdata->sensornameLength);

	if (exprdTime < t->duration) {
		temp = t->read();
		kprintf("temp is %d C\n", temp);

		payload = (char *)getmem(payloadL);
		if (payload) {
			serialize_and_copy_sensor_name(payload, tdata->sensorname, tdata->sensornameLength);
			SERVICES_COPY_INT32_TO_LITTLE_ENDIAN(temp, payload+payloadOffset);
			build_pkt_and_enqueue(MQTT_PUBLISH_MESSAGE, 
								MQTT_TOPIC_CUR_TEMP_ANALOG, payloadL, (uint8 *)payload, (mqttQ_t *)tdata->txQ);
		} else {
			kprintf("mem-full\n");
		}
	} else {
		cancel_timer(t->timer);
		delete_active_notifier(t);
		freemem((char *)tdata, sizeof(ThermometerNotifyData));
		freemem((char *)t, sizeof(Tmp36));
	}
}

void thermometerServiceFn() {
	msgListEnt_t* msg = NULL;
	mqttQ_t *rxQ, *txQ;
	uint8 sensorhash = 0;
	uint8 sensornameLength = 0;
	uint8 sensorname[SERVICE_SENSOR_NAME_MAX_LEN] = {0};

	kprintf("thermometerServiceFn>>\n");
	mqtt_regiter_service(MQTT_SERVICE_THERMOMETER, &rxQ, &txQ);

	build_pkt_and_enqueue(MQTT_SUBSCRIBE_MESSAGE, MQTT_TOPIC_GET_TEMP_ANALOG, 0, NULL, txQ);
	build_pkt_and_enqueue(MQTT_SUBSCRIBE_MESSAGE, MQTT_TOPIC_GET_TEMP_DIGITAL, 0, NULL, txQ);
	build_pkt_and_enqueue(MQTT_SUBSCRIBE_MESSAGE, MQTT_TOPIC_NOTIFY_TEMP_ANALOG, 0, NULL, txQ);
	build_pkt_and_enqueue(MQTT_SUBSCRIBE_MESSAGE, MQTT_TOPIC_NOTIFY_TEMP_DIGITAL, 0, NULL, txQ);

	while (TRUE) {
		 msg = mqtt_dequeue(rxQ);
		 if (msg) {
			 sensorhash = 0;
			 memset(sensorname, 0, SERVICE_SENSOR_NAME_MAX_LEN);
			 derive_sensor_name_from_payload(thermometerNameHash, msg,
										sensorname, &sensornameLength, &sensorhash);

			 if (sensornameLength) {
#ifdef TMP_DEBUG
			 	 kprintf("%s %d\n", sensorname, sensorhash);
#endif
				 switch (msg->protoh.topic) {
				 	case MQTT_TOPIC_GET_TEMP_ANALOG:
					case MQTT_TOPIC_NOTIFY_TEMP_ANALOG:
					{
						switch (sensorhash) {
							case THERMOMETER_ID_TMP36:
							{
								if (msg->protoh.topic == MQTT_TOPIC_GET_TEMP_ANALOG) {
									Tmp36 t = Tmp36(P84, 0);
									char *payload;
									uint8 payloadL = SERVICE_STD_PAYLOAD_LEN+SERVICE_SENSOR_NAME_FLD_LEN+sensornameLength;
									uint8 payloadOffset = SERVICE_SENSOR_NAME_FLD_LEN+sensornameLength;
									int32 temp = t.read();

									kprintf("temp is %d C\n", temp);
									payload = (char *)getmem(payloadL);
									if (payload) {
										serialize_and_copy_sensor_name(payload, sensorname, sensornameLength);
										SERVICES_COPY_INT32_TO_LITTLE_ENDIAN(temp, payload+payloadOffset);
										build_pkt_and_enqueue(MQTT_PUBLISH_MESSAGE, 
															MQTT_TOPIC_CUR_TEMP_ANALOG, payloadL, (uint8 *)payload, txQ);
									} else {
										kprintf("mem-full\n");
									}
								} else if (msg->protoh.topic == MQTT_TOPIC_NOTIFY_TEMP_ANALOG){
									uint32 interval = 0;
									uint32 duration = 0;

									if (msg->protoh.length > SERVICE_SENSOR_NAME_FLD_LEN+sensornameLength) {
										Tmp36 *tp = NULL;
										ThermometerNotifier *tn = get_active_notifier(sensorname, sensornameLength);
										ThermometerNotifyData *data = NULL;

										interval = SERVICES_GET_UINT32_FROM_LITTLE_ENDIAN
											(msg->payload+SERVICE_SENSOR_NAME_FLD_LEN+sensornameLength);
										duration = SERVICES_GET_UINT32_FROM_LITTLE_ENDIAN
											(msg->payload+SERVICE_SENSOR_NAME_FLD_LEN+sensornameLength+4);
										kprintf("interval: %u duration: %u\n", interval, duration);
										
										if (duration) {
											if (!tn) {
												Tmp36 t = Tmp36(P84, 0);
												tp = (Tmp36 *)malloc(sizeof(Tmp36));
												memcpy(tp, &t, sizeof(Tmp36));
												//add to active notifier list
												tn = (ThermometerNotifier *)malloc(sizeof(ThermometerNotifier));
												tn->t = tp;
												tn->next = NULL;
												add_to_active_notifier(tn);
											} else {
												tp = (Tmp36 *)tn->t;
											}
											
										} else {
											if (tn) {
												tp = (Tmp36 *)tn->t;
											} else {
												kprintf("case4\n");
											}
										}

										if (tp) {
											data = (ThermometerNotifyData *)malloc(sizeof(ThermometerNotifyData));
											strncpy((char *)data->sensorname, (const char *)sensorname, sensornameLength);
											data->sensornameLength = sensornameLength;
											data->txQ = (void *)txQ;
											kprintf("tp: %x\n", tp);
											tp->notify(interval, duration, notifycb, data);
										}
									}
								}
								else {
									kprintf("unsupported thermometer service\n");
								}
							}
							break;
							default:
								kprintf("unknown analog thermometer\n");
						}
					}	
					break;
					case MQTT_TOPIC_GET_TEMP_DIGITAL:
					case MQTT_TOPIC_NOTIFY_TEMP_DIGITAL:
					{
						switch (sensorhash) {
							if (msg->protoh.topic == MQTT_TOPIC_GET_TEMP_DIGITAL) {
								Ds18b20 t = Ds18b20(P45, 0);
								char *payload;
								uint8 payloadL = SERVICE_STD_PAYLOAD_LEN+SERVICE_SENSOR_NAME_FLD_LEN+sensornameLength;
								uint8 payloadOffset = SERVICE_SENSOR_NAME_FLD_LEN+sensornameLength;
								int32 temp = t.read();

								kprintf("temp is %d C\n", temp);
								payload = (char *)getmem(payloadL);
								if (payload) {
									serialize_and_copy_sensor_name(payload, sensorname, sensornameLength);
									SERVICES_COPY_INT32_TO_LITTLE_ENDIAN(temp, payload+payloadOffset);
									build_pkt_and_enqueue(MQTT_PUBLISH_MESSAGE, 
														MQTT_TOPIC_CUR_TEMP_ANALOG, payloadL, (uint8 *)payload, txQ);
								} else {
									kprintf("mem-full\n");
								}
							} else if (msg->protoh.topic == MQTT_TOPIC_NOTIFY_TEMP_DIGITAL){
								uint32 interval = 0;
								uint32 duration = 0;

								if (msg->protoh.length > SERVICE_SENSOR_NAME_FLD_LEN+sensornameLength) {
									Ds18b20 *tp = NULL;
									ThermometerNotifier *tn = get_active_notifier(sensorname, sensornameLength);
									ThermometerNotifyData *data = NULL;

									interval = SERVICES_GET_UINT32_FROM_LITTLE_ENDIAN
										(msg->payload+SERVICE_SENSOR_NAME_FLD_LEN+sensornameLength);
									duration = SERVICES_GET_UINT32_FROM_LITTLE_ENDIAN
										(msg->payload+SERVICE_SENSOR_NAME_FLD_LEN+sensornameLength+4);
									kprintf("interval: %u duration: %u\n", interval, duration);
									
									if (duration) {
										if (!tn) {
											Ds18b20 t = Ds18b20(P45, 1);
											tp = (Ds18b20 *)malloc(sizeof(Ds18b20));
											memcpy(tp, &t, sizeof(Ds18b20));
											//add to active notifier list
											tn = (ThermometerNotifier *)malloc(sizeof(ThermometerNotifier));
											tn->t = tp;
											tn->next = NULL;
											add_to_active_notifier(tn);
										} else {
											if (tn) {
												tp = (Ds18b20 *)tn->t;
											}
										}
										
									} else {
										if (tn) {
											tp = (Ds18b20 *)tn->t;
										}
									}

						
									if (tp) {
										data = (ThermometerNotifyData *)malloc(sizeof(ThermometerNotifyData));
										strncpy((char *)data->sensorname, (const char *)sensorname, sensornameLength);
										data->sensornameLength = sensornameLength;
										data->txQ = (void *)txQ;
										kprintf("tp: %x\n", tp);
										tp->notify(interval, duration, notifycb, data);
									}
								}
							}
							else {
								kprintf("unsupported thermometer service\n");
							}
						}
						break;
					}
					break;
					default:
						kprintf("Unknown topic\n");
				 }
			 }
			 freemem((char *)msg->payload, msg->protoh.length);
			 freemem((char *)msg, sizeof(msgListEnt_t));
			 msg = NULL;
		} else {
			kprintf("thermometer service going to sleep..\n");
			receiveMsg();
			kprintf("thermometer service woke up\n");
		}
	}//while(TRUE)
}

