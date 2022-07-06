#include <stdlib.h>
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include "iesmotors.h"
#include "iesusart.h"
#include "iesadc.h"
#include "followLine.h"
#include "iessreg.h"

typedef enum State {SOFTLEFT, SOFTRIGHT, FORWARD, LEFT, RIGHT} State;

enum LEFT_MOTOR_PINS {
	ENA = PD5,
	IN1 = PD7,
	IN2 = PB0
};
enum RIGHT_MOTOR_PINS {
	ENB = PD6,
	IN3 = PB1,
	IN4 = PB3
};

// left motor forward:  PORTD |= (1 << PD7);
// left motor backward: PORTB |= (1 << PB0);
// right motor forward: PORTB |= (1 << PB3);
// right motor backward:PORTB |= (1 << PB1); 


// Dutycicle
void setDutyCicle(char left, char right){
	 setDutyCycle(ENA, left);
	 setDutyCycle(ENB, right);
	}

// motor left
void leftforward(){
	PORTD |= (1 << IN1);
	PORTB &= ~(1 << IN2);
	}

void leftbackward(){
	PORTD &= ~(1 << IN1);
	PORTB |= (1 << IN2);
	}

// motor right
void rightforward(){
	PORTB &= ~(1 << IN3);
	PORTB |= (1 << IN4);
	}

void rightbackward(){
	PORTB |= (1 << IN3);
	PORTB &= ~(1 << IN4);
	}


// drive directions
void drivemiddle(){
	setDutyCicle(100,100);
	leftforward();
	rightforward();
	}

void driveright(){
	setDutyCicle(180,140);
    leftforward();
	rightbackward();
	}

void drivelowright(){
	setDutyCicle(160,120);
	leftforward();
	rightforward();
	}

void driveleft(){
	setDutyCicle(140,180);
	rightforward();
	leftbackward();
	}
	
void drivelowleft(){
	setDutyCicle(120,160);
	rightforward();
	leftforward();
	}

int main(void) {

	//Delete everything on ports B and D
    DDRD = 0;
    DDRB = 0;
    
	ADC_init();
	
	setup_ddr_all();
	USART_init(UBRR_SETTING);
	// Set Data Direction Register C [0|1|2| as input.
    //DDRC &= ~((1<<DDC0) | (1<<DDC1) | (1<<DDC2));


    


    // Allocate 1 byte in memory/on heap for a representation (model)
    // of the register and clear the contents directly, and update
    // everything accordingly.
    srr_t *regmdl = malloc(sizeof(srr_t));
    clear(regmdl);

    srr_t last_model_state = *regmdl;

    

	
	
	
	
	
	//funktion schrieben motors init

    // Set PD5 and PD6 as output (EN[A|B]!)
    DDRD |= (1 << DD5) | (1 << DD6);

    // Set PD7 as output (IN1)
    DDRD |= (1 << DD7);

    // Make PWM work on PD[5|6]
    setupTimer0();

    // Set PB0, PB1, and PB3 as output (IN[2|3|4])
    DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB3);

	unsigned char strbuff[sizeof(ADCMSG) + 15]; // WTF, why + 15? Oo

	uint16_t adcval0 = 0;
	uint16_t adcval1 = 0;
	uint16_t adcval2 = 0;

	State CurrentState = FORWARD;
	
	//LED------
	// Set Data Direction Register B5 as output.
    //DDRB = (1<<DDB5);
    
    char right_on_line;
	char middle_on_line;
	char left_on_line;
	
	while(1) {
        
		adcval0 = ADC_read_avg(ADMUX_CHN_ADC0, ADC_AVG_WINDOW);
		adcval1 = ADC_read_avg(ADMUX_CHN_ADC1, ADC_AVG_WINDOW);
		adcval2 = ADC_read_avg(ADMUX_CHN_ADC2, ADC_AVG_WINDOW);

		right_on_line = ADC_read_avg(ADMUX_CHN_ADC0, ADC_AVG_WINDOW) > VALUERIGHT;
		middle_on_line = ADC_read_avg(ADMUX_CHN_ADC1, ADC_AVG_WINDOW) > VALUEMIDDLE;
		left_on_line = ADC_read_avg(ADMUX_CHN_ADC2, ADC_AVG_WINDOW) > VALUELEFT;
		
		switch(CurrentState){
			case FORWARD: 
				drivemiddle(&CurrentState);
				if(right_on_line){
				//USART_print("4 \n");
					CurrentState = SOFTRIGHT;
					USART_print("SOFTRIGHT \n");
				}else if(left_on_line){
					//USART_print("5 \n");
					CurrentState = SOFTLEFT;
					USART_print("SOFTLEFT \n");
				}
				break;
				
			case SOFTLEFT:
				drivelowleft();
				if(!left_on_line && middle_on_line){
					CurrentState = FORWARD;
					USART_print("FORWARD \n");
				}else if(!middle_on_line && !left_on_line){
					CurrentState = LEFT;
					USART_print("LEFT \n");
					}
				break;
				
			case SOFTRIGHT:
				drivelowright();
				if(!right_on_line && middle_on_line){
					CurrentState = FORWARD;
						USART_print("FORWARD \n");
				}else if(!middle_on_line && !right_on_line){
					CurrentState = RIGHT;
					USART_print("RIGHT \n");
					}
				break;
				
			case LEFT:
				driveleft();
				if(!left_on_line && middle_on_line){
					CurrentState = FORWARD;
					USART_print("FORWARD \n");
				}
				break;
				
			case RIGHT:
				driveright();
				if(!right_on_line && middle_on_line){
					CurrentState = FORWARD;
					USART_print("FORWARD \n");
				}
				break;
			}
			
		
		update_model(regmdl, left_on_line, middle_on_line, right_on_line);

		if (*regmdl != last_model_state) {
		   update_hardware(regmdl);
		   last_model_state = *regmdl;
		}

    
	
	
		
		//sprintf(strbuff, ADCMSG, adcval0, adcval1, adcval2);

		//USART_print(strbuff);
	}

	return 0;
}
