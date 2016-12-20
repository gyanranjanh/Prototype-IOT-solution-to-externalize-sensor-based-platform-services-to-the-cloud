#include  <xinu.h>

#define BUFFER_SIZE 100
#define SAMPLE_SIZE 10

// 7 channel adcs
unsigned int buffer[7][BUFFER_SIZE];

void enable_adc(bbb_adc_pin_t adc_pin) {
    memset(buffer[adc_pin], 0, BUFFER_SIZE);
    BBBIO_ADCTSC_module_ctrl(BBBIO_ADC_WORK_MODE_BUSY_POLLING, 160);
    BBBIO_ADCTSC_channel_ctrl(adc_pin, BBBIO_ADC_STEP_MODE_SW_CONTINUOUS, 0, 1, \
                              BBBIO_ADC_STEP_AVG_8, buffer[adc_pin], BUFFER_SIZE);
}

int read_adc(bbb_adc_pin_t adc_pin) {
    unsigned int i ,j;
    unsigned int sample;
    unsigned int avg = 0;

    /* kprintf("read adc>>%d\n", adc_pin); */
    BBBIO_ADCTSC_channel_enable(adc_pin);
    avg = 0;
    for(i = 0 ; i < 1 ; i++) {
        BBBIO_ADCTSC_work(SAMPLE_SIZE);

        for(j = 0 ; j < SAMPLE_SIZE ; j++) {
            sample = buffer[adc_pin][j];
            avg += sample;
        }
        /* sleepms(100); */
    }
    BBBIO_ADCTSC_channel_disable(adc_pin);

    return (int) (avg/10);
}

void disable_adc(bbb_adc_pin_t adc_pin) {
    /* BBBIO_ADCTSC_channel_disable(adc_pin); */
}


