#include <xinu_cpp.h>
#include <ledservice.h>
#include <services.h>
#include <mqttcliapi.h>
#include <serviceutil.h>

void ledServiceFn() {
	msgListEnt_t* msg = NULL;
	mqttQ_t *rxQ, *txQ;
	uint8 sensorhash = 0;
	uint8 sensornameLength = 0;
	uint8 sensorname[SERVICE_SENSOR_NAME_MAX_LEN] = {0};

	kprintf("ledServiceFn>>\n");
	mqtt_regiter_service(MQTT_SERVICE_BASIC_LED, &rxQ, &txQ);

	build_pkt_and_enqueue(MQTT_SUBSCRIBE_MESSAGE, MQTT_TOPIC_LED, 0, NULL, txQ);
	build_pkt_and_enqueue(MQTT_SUBSCRIBE_MESSAGE, MQTT_TOPIC_GET_LED_STATUS, 0, NULL, txQ);

	while (TRUE) {
		msg = mqtt_dequeue(rxQ);
		if (msg) {
			sensorhash = 0;
			memset(sensorname, 0, SERVICE_SENSOR_NAME_MAX_LEN);
	  	    derive_sensor_name_from_payload(ledNameHash, msg,
	  						sensorname, &sensornameLength, &sensorhash);
			if(sensornameLength) {
#ifdef LED_TEST
				kprintf("%s %d %u\n", sensorname, sensorhash, msg->protoh.topic);
#endif
				switch (sensorhash) {
					case LED_ID_LED1:
					{
						Led led(P44);
						switch (msg->protoh.topic) {
							case MQTT_TOPIC_LED:
							{
								uint8 on = false;
								if (msg->protoh.length > SERVICE_SENSOR_NAME_FLD_LEN+sensornameLength) {
									on = SERVICES_GET_UINT8_FROM_LITTLE_ENDIAN
											(msg->payload+SERVICE_SENSOR_NAME_FLD_LEN+sensornameLength);
									if (on) {
										led.write(true);
									}
									else {
										led.write(false);
									}
								}
							}
							break;
							case MQTT_TOPIC_GET_LED_STATUS:
							{
									char *payload;
									uint8 payloadL = SERVICE_STD_PAYLOAD_LEN+SERVICE_SENSOR_NAME_FLD_LEN+sensornameLength;
									uint8 payloadOffset = SERVICE_SENSOR_NAME_FLD_LEN+sensornameLength;
									uint8 _status = false;

									_status = (uint8)(led.read() & 0x000000ff);
#ifdef LED_TEST
									printf("led status: %u\n", _status);
#endif
									payload = (char *)getmem(payloadL);
									if (payload) {
										serialize_and_copy_sensor_name(payload, sensorname, sensornameLength);
										SERVICES_COPY_UINT8_TO_LITTLE_ENDIAN(_status, payload+payloadOffset);
										build_pkt_and_enqueue(MQTT_PUBLISH_MESSAGE, 
														MQTT_TOPIC_LED_STATUS, payloadL, (uint8 *)payload, txQ);
									}
							}
							break;
							default:
								kprintf("unsupported led service\n");
						}
					}
					break;
					case LED_ID_LED2:
					{
						Led led(P45);
						switch (msg->protoh.topic) {
							case MQTT_TOPIC_LED:
							{
								uint8 on = false;
								if (msg->protoh.length > SERVICE_SENSOR_NAME_FLD_LEN+sensornameLength) {
									on = SERVICES_GET_UINT8_FROM_LITTLE_ENDIAN
											(msg->payload+SERVICE_SENSOR_NAME_FLD_LEN+sensornameLength);
									if (on) {
										led.write(true);
									}
									else {
										led.write(false);
									}
								}
							}
							break;
							case MQTT_TOPIC_GET_LED_STATUS:
							{
								char *payload;
								uint8 payloadL = SERVICE_STD_PAYLOAD_LEN+SERVICE_SENSOR_NAME_FLD_LEN+sensornameLength;
								uint8 payloadOffset = SERVICE_SENSOR_NAME_FLD_LEN+sensornameLength;
								uint8 _status = false;

								_status = (uint8)(led.read() & 0x000000ff);
#ifdef LED_TEST
								kprintf("led status: %u\n", _status);
#endif
								led.deinit();
								payload = (char *)getmem(payloadL);
								if (payload) {
									serialize_and_copy_sensor_name(payload, sensorname, sensornameLength);
									SERVICES_COPY_UINT8_TO_LITTLE_ENDIAN(_status, payload+payloadOffset);
									build_pkt_and_enqueue(MQTT_PUBLISH_MESSAGE, 
													MQTT_TOPIC_LED_STATUS, payloadL, (uint8 *)payload, txQ);
								}
							}
							break;
							default:
								kprintf("unsupported led service\n");
						}
					}
					break;
					default:
						kprintf("unknown led\n");
				}
			}
			freemem((char *)msg->payload, msg->protoh.length);
			freemem((char *)msg, sizeof(msgListEnt_t));
			msg = NULL;
		} else {
			kprintf("led service going to sleep..\n");
			receiveMsg();
			kprintf("led service woke up\n");
		}
	}
}

