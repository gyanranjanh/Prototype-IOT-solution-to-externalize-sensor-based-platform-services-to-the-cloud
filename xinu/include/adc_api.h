#ifndef __ADC_API_H__
#define __ADC_API_H__

#ifdef __cplusplus
extern "C" {
#endif

void enable_adc(bbb_adc_pin_t adc_pin);
int read_adc(bbb_adc_pin_t adc_pin);
void disable_adc(bbb_adc_pin_t adc_pin);

#ifdef __cplusplus
} //end extern "C"
#endif

#endif
