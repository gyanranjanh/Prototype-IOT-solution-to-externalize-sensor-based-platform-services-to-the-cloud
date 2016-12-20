#ifndef SNATCH59_ONEWIREDEFS_H
#define SNATCH59_ONEWIREDEFS_H
#ifdef __cplusplus
extern "C" {
#endif

// device ids
#define DS18B20_ID        0x28
#define DS18S20_ID        0x10

#define ALARM_CONFIG_SIZE 3
#define THERMOM_SCRATCHPAD_SIZE    9
#define THERMOM_CRC_BYTE  8
#define ADDRESS_SIZE      8
#define ADDRESS_CRC_BYTE  7

// One Wire command codes
#define OVERDRIVE_SKIP    0x3C
// ROM commands
#define SEARCH_ROM        0xF0
#define READ_ROM          0x33
#define MATCH_ROM         0x55
#define SKIP_ROM          0xCC
#define ALARM_SEARCH      0xEC
// Functions Commnds
#define CONVERT           0x44
#define WRITESCRATCH      0x4E
#define READSCRATCH       0xBE
#define COPYSCRATCH       0x48
#define RECALLE2          0xB8
#define READPOWERSUPPLY   0xB4

// temperature read resolutions
enum eResolution {nineBit = 0, tenBit, elevenBit, twelveBit};
extern int CONVERSION_TIME[4];    // milli-seconds

// DS18B20/DS18S20 related
#define TEMPERATURE_LSB    0
#define TEMPERATURE_MSB    1
#define HIGH_ALARM_BYTE    2
#define LOW_ALARM_BYTE     3
#define CONFIG_REG_BYTE    4
#define CONFIG_READ_END    5
#define COUNT_REMAIN_BYTE  6
#define COUNT_PER_DEG_BYTE 7

#ifdef __cplusplus
} //end extern "C"
#endif

#endif

