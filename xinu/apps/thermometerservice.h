#ifndef __THERMOMETER_SERVICE_H__
#define __THERMOMETER_SERVICE_H__

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_NO_ACTIVE_NOTIFIER (10)

#define THERMOMETER_ID_TMP36   (0X01)
#define THERMOMETER_ID_DS18B20 (0X02)


static inline void thermometerNameHash(uint8* sensorname, uint8 *sensorhash) 
{
#ifdef TMP_DEBUG
    printf("thermometerNameHash>>: %s\n", sensorname);
#endif
    if (!strncmp((const char *)sensorname, "DS18B20", 7))
    {  
        *sensorhash = THERMOMETER_ID_DS18B20; 
    }  
    else if (!strncmp((const char *)sensorname, "TMP36_1", 7)) {  
        *sensorhash = THERMOMETER_ID_TMP36; 
    }
#ifdef TMP_DEBUG
    printf("thermometerNameHash>>: %d\n", *sensorhash);
#endif
}

class ThermometerNotifyData {
    public:
        uint8 sensorname[SERVICE_SENSOR_NAME_MAX_LEN];
        uint8 sensornameLength;
        void *txQ;
};

class ThermometerNotifier {
    public:
        Thermometer *t;
        ThermometerNotifier *next;
};

extern ThermometerNotifier *activeNotifier;

static inline void add_to_active_notifier(ThermometerNotifier *t) {
    ThermometerNotifier *tn = activeNotifier;
    ThermometerNotifier *prev = NULL;
    bool found = false;

    if (t != NULL) {
        ThermometerNotifyData *tdata = (ThermometerNotifyData *)t->t->data;
        if(tn) {
            while (tn) {
                prev = tn;
                if (strncmp((const char *)((ThermometerNotifyData *)(tn->t->data))->sensorname, 
                            (const char *) tdata->sensorname, 
                            tdata->sensornameLength)) {
                    found = true;
                    break;
                }
                tn = tn->next;
            }
            if (!found) {
                prev->next = t;
            } else {
                freemem((char *)t, sizeof(ThermometerNotifier));
            }
        } else {
            activeNotifier = tn;
        }
    }
}

static inline ThermometerNotifier *get_active_notifier(
                                            uint8 *sensorname,
                                            uint8 sensornameLen) {
    ThermometerNotifier *found = NULL;
    ThermometerNotifier *tn = activeNotifier;

    while (tn) {
        if (strncmp((const char *)(((ThermometerNotifyData *)(tn->t->data))->sensorname),
                                (const char *)sensorname, sensornameLen)) {
            found = tn;
            break;
        }
        tn = tn->next;
    }
    return found;
}

static inline void delete_active_notifier(Thermometer *t) {
    ThermometerNotifier *tn = activeNotifier;
    ThermometerNotifier *prev = NULL;

    while (tn) {
        prev = tn;
        if (tn->t == t) {
            break;
        }
        tn = tn->next;
    }

    if (tn) {
        prev->next = tn->next;
        freemem((char *)tn, sizeof(ThermometerNotifier));
    }
}

void thermometerServiceFn();

#ifdef __cplusplus
} //end extern "C"
#endif

#endif

