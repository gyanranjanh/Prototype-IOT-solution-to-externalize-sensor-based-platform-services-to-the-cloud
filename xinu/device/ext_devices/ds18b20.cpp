#include <xinu_cpp.h>
#include <onewiredefs.h>

const int timing[] = {6, 64, 60, 10, 9, 55, 0, 480, 70, 410};
//const int timing[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int CONVERSION_TIME[] = {94, 188, 375, 750};
const bool useCRC = false;

Ds18b20::Ds18b20(pinname_t p, bool t) : Thermometer(p, t) {
#ifdef TMP_DEBUG
	kprintf("Ds18b20>>%u\n", p);
#endif
	init(0);
	setResolution(tenBit);
}

int32 Ds18b20::read() {
	byte data[THERMOM_SCRATCHPAD_SIZE];
    int realTemp = -999;

    writeByte(SKIP_ROM);
    writeByte(CONVERT);     // issue Convert command
    
    sleepms(CONVERSION_TIME[resolution]);


    if (readAndValidateData(data))    // issue Read Scratchpad commmand and get data
    {
        realTemp = calculateTemperature(data);
    }
    
    return realTemp; 
}

bool Ds18b20::readAndValidateData(byte* data)
{
    bool dataOk = true;
    
    writeByte(SKIP_ROM);
    writeByte(READSCRATCH);    // read Scratchpad

    kprintf("read = ");
    for (int i = 0; i < THERMOM_SCRATCHPAD_SIZE; i++) 
    {               
        // we need all bytes which includes CRC check byte
        data[i] = readByte();
        kprintf("%x ", (int)data[i]);
    }
    kprintf("\r\n");

    // Check CRC is valid if you want to
    if (useCRC && !(crc8(data, THERMOM_CRC_BYTE) == data[THERMOM_CRC_BYTE]))  
    {  
        // CRC failed
        kprintf("CRC FAILED... \r\n");
        dataOk = false;
    }
    
    return dataOk;
}

byte Ds18b20::crc8(byte* addr, byte len)
{
    byte i, j;
    byte crc = 0;
    
    for (i = 0; i < len; i++) 
    {
        byte inbyte = addr[i];
        for (j = 0; j < 8; j++) 
        {
            byte mix = (crc ^ inbyte) & 0x01;
            crc >>= 1;
            if (mix) crc ^= 0x8C;
            inbyte >>= 1;
        }
    }
    
    return crc;
}

int Ds18b20::calculateTemperature(void* data) {
    bool signBit = false;
    if (((byte *)data)[TEMPERATURE_MSB] & 0x80) signBit = true;
        
    int read_temp = (((byte *)data)[TEMPERATURE_MSB] << 8) + ((byte *)data)[TEMPERATURE_LSB];
    if (signBit)
    {
        read_temp = (read_temp ^ 0xFFFF) + 1;    // two's complement
        read_temp *= -1;
    }
    
    int resolution = (((byte *)data)[CONFIG_REG_BYTE] & 0x60) >> 5; // mask off bits 6,5 and move to 1,0
    switch (resolution)
    {
        case nineBit:    						// 0.5 deg C increments
            read_temp &= 0xFFF8;                // bits 2,1,0 are undefined
            kprintf("9 bit resolution ...\n");
            break;
        case tenBit:     						// 0.25 deg C increments
            read_temp &= 0xFFFC;                // bits 1,0 are undefined
            kprintf("10 bit resolution ...\n");
            break;
        case elevenBit:  						// 0.125 deg C increments
            read_temp &= 0xFFFE;                // bit 0 is undefined
            kprintf("11 bit resolution ...\n");
            break;
        case twelveBit:  						// 0.0625 deg C increments
            kprintf("12 bit resolution ...\n");
            break;
    }
    int realTemp = (int)read_temp/16 ;
                 
    kprintf("TEMP_READ/REAL TEMP: %d \n", realTemp); 
               
    return (int)realTemp;
}

void Ds18b20::setResolution(eResolution resln)
{
    // as the write to the configuration register involves a write to the
    // high and low alarm bytes, need to read these registers first
    // and copy them back on the write
    
    byte read_data[THERMOM_SCRATCHPAD_SIZE];
    byte write_data[ALARM_CONFIG_SIZE];


    if (readAndValidateData(read_data))
    {
        // copy alarm and config data to write data
        for (int k = 2; k < 5; k++)
        {
            write_data[k - 2] = read_data[k];
        }
        int config = write_data[2];
        config &= 0x9F;
        config ^= (resln << 5);
        write_data[2] = config;
        
        writeByte(SKIP_ROM);
        writeByte(WRITESCRATCH);
        for (int k = 0; k < 3; k++)
        {
            writeByte(write_data[k]);
        }
        
        // remember it so we can use the correct delay in reading the temperature
        // for parasitic power
        resolution = resln; 
    }
}


void Ds18b20::writeByte(int data) 
{
    // Loop to write each bit in the byte, LS-bit first
    for (int loop = 0; loop < 8; loop++)
    {
        writeBit(data & 0x01);
        
        // shift the data byte for the next bit
        data >>= 1;
    }
}

//
// Read a byte
//
int Ds18b20::readByte() 
{
    int result = 0;
    
    for (int loop = 0; loop < 8; loop++)
    {
        // shift the result to get it ready for the next bit
        result >>= 1;
        
        // if result is one, then set MS bit
        if (readBit()) result |= 0x80;
    }
    
    return result;
}

void Ds18b20::writeBit(int bit)
{
    bit = bit & 0x01;
    
    if (bit)
    {
        // Write '1' bit
        BBBIO_sys_Enable_GPIO(pinmap[pin].pp.port);
		BBBIO_GPIO_set_dir(pinmap[pin].pp.port, 0, 1 << pinmap[pin].pp.pin);
		BBBIO_GPIO_high(pinmap[pin].pp.port, 1 << pinmap[pin].pp.pin);
		BBBIO_GPIO_low(pinmap[pin].pp.port, 1 << pinmap[pin].pp.pin);
		BBBIO_sys_Disable_GPIO(pinmap[pin].pp.port);
        sleepms(timing[0]);
		
		BBBIO_sys_Enable_GPIO(pinmap[pin].pp.port);
		BBBIO_GPIO_set_dir(pinmap[pin].pp.port, 1 << pinmap[pin].pp.pin, 0);
		BBBIO_sys_Disable_GPIO(pinmap[pin].pp.port);
        sleepms(timing[1]);
    }
    else
    {
        // Write '0' bit
        BBBIO_sys_Enable_GPIO(pinmap[pin].pp.port);
		BBBIO_GPIO_set_dir(pinmap[pin].pp.port, 0, 1 << pinmap[pin].pp.pin);
		BBBIO_GPIO_high(pinmap[pin].pp.port, 1 << pinmap[pin].pp.pin);
		BBBIO_GPIO_low(pinmap[pin].pp.port, 1 << pinmap[pin].pp.pin);
		BBBIO_sys_Disable_GPIO(pinmap[pin].pp.port);
        sleepms(timing[2]);
		
		BBBIO_sys_Enable_GPIO(pinmap[pin].pp.port);
		BBBIO_GPIO_set_dir(pinmap[pin].pp.port, 1 << pinmap[pin].pp.pin, 0);
		BBBIO_sys_Disable_GPIO(pinmap[pin].pp.port);
        sleepms(timing[3]);

    }
}

//
// Read a bit. Port and bit is used to cut lookup time and provide
// more certain timing.
//
int Ds18b20::readBit() 
{
    int result;
    
	BBBIO_sys_Enable_GPIO(pinmap[pin].pp.port);
	BBBIO_GPIO_set_dir(pinmap[pin].pp.port, 0, 1 << pinmap[pin].pp.pin);
	BBBIO_GPIO_high(pinmap[pin].pp.port, 1 << pinmap[pin].pp.pin);
	BBBIO_GPIO_low(pinmap[pin].pp.port, 1 << pinmap[pin].pp.pin);
	BBBIO_sys_Disable_GPIO(pinmap[pin].pp.port);
	sleepms(timing[0]);

	BBBIO_sys_Enable_GPIO(pinmap[pin].pp.port);
	BBBIO_GPIO_set_dir(pinmap[pin].pp.port, 1 << pinmap[pin].pp.pin, 0);
	BBBIO_sys_Disable_GPIO(pinmap[pin].pp.port);
    sleepms(timing[4]);;
    result = BBBIO_GPIO_get(pinmap[pin].pp.port, 1 << pinmap[pin].pp.pin) & 0x01;
    sleepms(timing[5]);
       
    return result;
}

