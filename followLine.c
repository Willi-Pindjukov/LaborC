#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include "iesmotors.h"
#include "iesusart.h"
#include "iesadc.h"
#include "followLine.h"

typedef enum State {right, left, middle, lowleft, lowmiddle, lowright} State;


// left motor forward:  PORTD |= (1 << PD7);
// left motor backward: PORTB |= (1 << PB0);
// right motor forward: PORTB |= (1 << PB3);
// right motor backward:PORTB |= (1 << PB1); 

void drivemiddle(State* state){
	PORTB &= ~(1 << PB0);
	PORTB &= ~(1 << PB1); 
	setDutyCycle(PD5, 100);
    setDutyCycle(PD6, 100);
	PORTB |= (1 << PB3);
	PORTD |= (1 << PD7);
	*state = middle;
	}

void driveright(State* state){
	PORTB &= ~(1 << PB0);
	PORTB &= ~(1 << PB1); 
	setDutyCycle(PD5, 130);
    setDutyCycle(PD6, 200);
	PORTD |= (1 << PD7);
	PORTB &= ~(1 << PB3);
	PORTB |= (1 << PB1); 
	*state = right;
	}

void drivelowright(State* state){
	PORTB &= ~(1 << PB0);
	PORTB &= ~(1 << PB1); 
	setDutyCycle(PD5, 100);
    setDutyCycle(PD6, 150);
	PORTD |= (1 << PD7);
	PORTB &= ~(1 << PB3);
	PORTB |= (1 << PB1); 
	*state = right;
	}

void driveleft(State* state){
	PORTB &= ~(1 << PB0);
	PORTB &= ~(1 << PB1); 
	setDutyCycle(PD5, 200);
    setDutyCycle(PD6, 130);
	PORTB |= (1 << PB3);
	PORTD &= ~(1 << PD7);
	PORTB |= (1 << PB0);
	*state = left;
	}
	
void drivelowleft(State* state){
	PORTB &= ~(1 << PB0);
	PORTB &= ~(1 << PB1); 
	setDutyCycle(PD5, 100);
    setDutyCycle(PD6, 100);
	PORTB |= (1 << PB3);
	PORTD &= ~(1 << PD7);
	PORTB |= (1 << PB0);
	*state = left;
	}

void stopmotors(State* state){
	PORTB &= ~(1 << PB0);
	PORTB &= ~(1 << PB1); 
    PORTB &= ~(1 << PB3);
    PORTD &= ~(1 << PD7);
    *state = middle;
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
	char lowleft;
	char lowmiddle;
	char lowright;


	USART_init(UBRR_SETTING);

	DR_ADC0 &= ~(1 << DP_ADC0);
	DR_ADC1 &= ~(1 << DP_ADC1);
	DR_ADC2 &= ~(1 << DP_ADC2);

	ADC_init();

	unsigned char strbuff[sizeof(ADCMSG) + 15]; // WTF, why + 15? Oo

	uint16_t adcval0 = 0;
	uint16_t adcval1 = 0;
	uint16_t adcval2 = 0;

	State CurrentState;
	
	//LED------
	// Set Data Direction Register B5 as output.
    //DDRB = (1<<DDB5);
    
	
	while(1) {
		// Set pin/bit 5 to high (VCC) and wait half a sec.
        //PORTB = (1<<PORTB5);
        //_delay_ms(500);
        
        // Set pin/bit 5 to low (GND) and wait half a sec.
        //PORTB &= ~(1<<PORTB5);
        //_delay_ms(500);
        
        
        
        
        
		adcval0 = ADC_read_avg(ADMUX_CHN_ADC0, ADC_AVG_WINDOW);
		adcval1 = ADC_read_avg(ADMUX_CHN_ADC1, ADC_AVG_WINDOW);
		adcval2 = ADC_read_avg(ADMUX_CHN_ADC2, ADC_AVG_WINDOW);

		right = ADC_read_avg(ADMUX_CHN_ADC0, ADC_AVG_WINDOW) > VALUERIGHT;
		middle = ADC_read_avg(ADMUX_CHN_ADC1, ADC_AVG_WINDOW) > VALUEMIDDLE;
		left = ADC_read_avg(ADMUX_CHN_ADC2, ADC_AVG_WINDOW) > VALUELEFT;
		
		
		//lowright = ADC_read_avg(ADMUX_CHN_ADC0, ADC_AVG_WINDOW) > VALUE400;
		//lowmiddle = ADC_read_avg(ADMUX_CHN_ADC1, ADC_AVG_WINDOW) > VALUE400;
		//lowleft = ADC_read_avg(ADMUX_CHN_ADC2, ADC_AVG_WINDOW) > VALUE400;
		
		
		//if(right && middle && left){
		//	//USART_print("0 \n");
		//	stopmotors(&CurrentState);
		//}else 
		if(middle && left){
			//USART_print("1 \n");
			driveleft(&CurrentState);
		}else if(middle && right){
			//USART_print("2 \n");
			driveright(&CurrentState);
		}else if(right){
			//USART_print("4 \n");
			drivelowright(&CurrentState);
		}else if(left){
			//USART_print("5 \n");
			drivelowleft(&CurrentState);
		}else if(middle){
			//USART_print("3 \n");
			drivemiddle(&CurrentState);
		}else if(CurrentState == middle){
			//USART_print("6 \n");
			drivemiddle(&CurrentState);
		}else if(CurrentState == right){
			//USART_print("7 \n");
			driveright(&CurrentState);
		}else if(CurrentState == left){
			//USART_print("8 \n");
			driveleft(&CurrentState);
		}

		
		//sprintf(strbuff, ADCMSG, adcval0, adcval1, adcval2);

		//USART_print(strbuff);
	}

	return 0;
}
