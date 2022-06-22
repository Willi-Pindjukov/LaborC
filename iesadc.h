

#ifndef IESADC_h
#define IESADC_h

// Average some measurements to reduce probable noise.
#define ADC_AVG_WINDOW 20

// Well hmmm understand this by yourself. Become inspired.
#define ADCMSG "ADC0: %5u\tADC1: %5u\tADC2: %5u\n"

// WTF. Why is there no header here? Always seperate declaration,
// documentation, and implementation! Clean this up if you use this!

#define DR_ADC0 DDRC
#define DP_ADC0 DDC0
#define IR_ADC0 PC0
#define IP_ADC0 PC0

#define DR_ADC1 DDRC
#define DP_ADC1 DDC1
#define IR_ADC1 PC1
#define IP_ADC1 PC1

#define DR_ADC2 DDRC
#define DP_ADC2 DDC2
#define IR_ADC2 PC2
#define IP_ADC2 PC2

// The following constants reflect table 23-4 of the ATMega328p-data-
// sheet. These constants
#define ADMUX_CHN_ADC0 0 // 0000 0000
#define ADMUX_CHN_ADC1 1 // 0000 0001
#define ADMUX_CHN_ADC2 2 // 0000 0010
#define ADMUX_CHN_ADC3 3 // 0000 0011
#define ADMUX_CHN_ALL 7 // 0000 1111


void ADC_init(void);

uint16_t ADC_read_avg(uint8_t channel, uint8_t nsamples);
#endif
