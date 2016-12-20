#ifndef __DS18B20_H__
#define __DS18B20_H__

#ifdef __cplusplus
extern "C" {
#endif

class Ds18b20 : public Thermometer {
    public:
        Ds18b20(pinname_t pin, bool type);
        virtual int32 read();
        virtual int calculateTemperature(void *data);
    private:
        void setResolution(eResolution resln);
        bool readAndValidateData(byte* data);
        eResolution resolution;
        byte crc8(byte* addr, byte len);
        int readByte();
        void writeByte(int data);
        int readBit();
        void writeBit(int data);
};

#ifdef __cplusplus
} //end extern "C"
#endif

#endif
