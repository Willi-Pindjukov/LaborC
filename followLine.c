#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include "iesmotors.h"
#include "iesusart.h"
#include "iesadc.h"

#define VALUERIGHT 600;
#define VALUEMIDDLE 500;
#define VALUELEFT 500;

typedef enum State {right, left, middle, middleright, middleleft} State;



void drivemiddle(State* state){
	setDutyCycle(PD5, 100);
    setDutyCycle(PD6, 100);
	PORTB |= (1 << PB3);
	PORTD |= (1 << PD7);
	*state = middle;
	}

void driveright(State* state){
	setDutyCycle(PD5, 155);
    setDutyCycle(PD6, 155);
	PORTD |= (1 << PD7);
	PORTB &= ~(1 << PB3);
	*state = right;
	}
	
void driveleft(State* state){
	setDutyCycle(PD5, 155);
    setDutyCycle(PD6, 155);
	PORTB |= (1 << PB3);
	PORTD &= ~(1 << PD7);
	*state = left;
	}



int main(void) { 
	
	USART_init(UBRR_SETTING);
	// Set Data Direction Register C [0|1|2| as input.
    DDRC &= ~((1<<DDC0) | (1<<DDC1) | (1<<DDC2));
    //USART_init(UBRR_SETTING);
    
	//Delete everything on ports B and D
    DDRD = 0;
    DDRB = 0;

    // Set PD5 and PD6 as output (EN[A|B]!)
    DDRD = (1 << DD5) | (1 << DD6);

    // Set PD7 as output (IN1)
    DDRD |= (1 << DD7);

    // Make PWM work on PD[5|6]
    setupTimer0();

    // Set PB0, PB1, and PB3 as output (IN[2|3|4])
    DDRB = (1 << DDB0) | (1 << DDB1) | (1 << DDB3);

    // Set the duty cycles for PD5/PD6
    setDutyCycle(PD5, 155);
    setDutyCycle(PD6, 155);

	// set backword motors on 0 for safety
	PORTB &= ~(1 << PB0);
	PORTB &= ~(1 << PB1);
	
	char right;
	char middle;
	char left;

	// drive with both motors forward (till middle sensor is 0)
	/*while(1) {
		
		char middle = (PINC & (1 << PINC1));
		char right = (PINC & (1 << PINC2));
		char left = (PINC & (1 << PINC0));
		char middleleft = (PINC & (1 << PINC1)) && (PINC & (1 << PINC0));
		char middleright = (PINC & (1 << PINC1)) && (PINC & (1 << PINC2));
		
		while(middle){
			USART_print("Middle sensor\n");
			//Right motor on forward
			PORTB |= (1 << PB3);
			//Left motor on forward    
			PORTD |= (1 << PD7);
			if(PINC & ~(1 << PINC1)){
				PORTB &= ~(1 << PB3);
				PORTD &= ~(1 << PD7);
				break;
				}
		}
		
	
		while(left) {
			USART_print("Left sensor\n");
			PORTB |= (1 << PB3);
			if(PINC & ~(1 << PINC0)){
				PORTB &= ~(1 << PB3);
				break;
				}
			}
	
	
		while(right) {
			USART_print("Right sensor\n");
			PORTD |= (1 << PD7);
			if(PINC & ~(1 << PINC2)){
				PORTD &= ~(1 << PD7);
				break;
				}
			}
*/
				
		
		USART_init(UBRR_SETTING);

		DR_ADC0 &= ~(1 << DP_ADC0);
		DR_ADC1 &= ~(1 << DP_ADC1);
		DR_ADC2 &= ~(1 << DP_ADC2);

		ADC_init();

		unsigned char strbuff[sizeof(ADCMSG) + 15]; // WTF, why + 15? Oo

		// Just to make things clear: You have to be extremely careful with
		// the size of the stringbuffer. Better safe than sorry! But memory
		// as well as time are so so so precious!

		uint16_t adcval0 = 0;
		uint16_t adcval1 = 0;
		uint16_t adcval2 = 0;
		
		State CurrentState;
		while(1) {
			
		
			adcval0 = ADC_read_avg(ADMUX_CHN_ADC0, ADC_AVG_WINDOW);
			adcval1 = ADC_read_avg(ADMUX_CHN_ADC1, ADC_AVG_WINDOW);
			adcval2 = ADC_read_avg(ADMUX_CHN_ADC2, ADC_AVG_WINDOW);
			
			right = ADC_read_avg(ADMUX_CHN_ADC0, ADC_AVG_WINDOW) > VALUERIGHT;
			middle = ADC_read_avg(ADMUX_CHN_ADC1, ADC_AVG_WINDOW) > VALUEMIDDLE;
			left = ADC_read_avg(ADMUX_CHN_ADC2, ADC_AVG_WINDOW) > VALUELEFT;
			
			if(middle){
				drivemiddle(&CurrentState);
			}else if(right){
				driveright(&CurrentState);
			}else if(left){
				driveleft(&CurrentState);
			}else if(CurrentState == middle){
				drivemiddle(&CurrentState);
			}else if(CurrentState == right){
				driveright(&CurrentState);
			}else if(CurrentState == left){
				driveleft(&CurrentState);
			}
			
				
			sprintf(strbuff, ADCMSG, adcval0, adcval1, adcval2);

			USART_print(strbuff);
		}


		
		_delay_ms(1000);
	
	return 0;
}
