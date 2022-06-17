#include "iesmotors.h"
#include <util/delay.h>


int main(void) { 
	
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
    DDRB = (1 << DD0) | (1 << DD1) | (1 << DD3);

    // Set the duty cycles for PD5/PD6
    setDutyCycle(PD5, 155);
    setDutyCycle(PD6, 155);

	// set backword motors on 0 for safety
	PORTB &= ~(1 << PB0);
	PORTB &= ~(1 << PB1);

	// drive with both motors forward (till middle sensor is 0)
	while(1) {
		while(PINC & (1 << PINC1)){
			//Right motor on forward
			PORTB |= (1 << PB3);
			//Left motor on forward    
			PORTD |= (1 << PD7);
			
			
			
			//if middle and right sonsor are 1
			if((PINC & (1 << PINC1)) && (PINC & (1 << PINC0))){
				
				//Right motor off forward
				PORTB &= ~(1 << PB3);
				//Left motor off forward
				PORTD &= ~(1 << PD7);
				//_delay_ms(3000);
				while(PINC & (1 << PINC0)){
					PORTB |= (1 << PB3);
					}
				}
			
			//if middle and left sonsor are 1
			if((PINC & (1 << PINC1)) && (PINC & (1 << PINC2))){
				
				//Right motor off forward
				PORTB &= ~(1 << PB3);
				//Left motor off forward
				PORTD &= ~(1 << PD7);
				//_delay_ms(3000);
		
				while(PINC & (1 << PINC2)){
					PORTD |= (1 << PD7);
					}
				}
			
			
			
			
			
			
			
			
			
			
			
			
			
			// if right sensor 1 -> start right motor forward (till right sensor is 0)
			if(PINC & (1 << PINC0)){
				
				//Right motor off forward
				PORTB &= ~(1 << PB3);
				//Left motor off forward
				PORTD &= ~(1 << PD7);
				//_delay_ms(3000);
				while(PINC & (1 << PINC0)){
					PORTB |= (1 << PB3);
					}
				}
			// if left sensor 1 -> start left motor forward (till left sensor is 0)
			if(PINC & (1 << PINC2)){
				
				//Right motor off forward
				PORTB &= ~(1 << PB3);
				//Left motor off forward
				PORTD &= ~(1 << PD7);
				//_delay_ms(3000);
		
				while(PINC & (1 << PINC2)){
					PORTD |= (1 << PD7);
					}
				}
			
		}
		
		// if right sensor 1 -> start right motor forward (till right sensor is 0)
		if(PINC & (1 << PINC0)){
			
			//Right motor off forward
			PORTB &= ~(1 << PB3);
			//Left motor off forward
			PORTD &= ~(1 << PD7);
			//_delay_ms(3000);
			while(PINC & (1 << PINC0)){
				PORTB |= (1 << PB3);
				}
			}
		// if left sensor 1 -> start left motor forward (till left sensor is 0)
		if(PINC & (1 << PINC2)){
			
			//Right motor off forward
			PORTB &= ~(1 << PB3);
			//Left motor off forward
			PORTD &= ~(1 << PD7);
			//_delay_ms(3000);
	
			while(PINC & (1 << PINC2)){
				PORTD |= (1 << PD7);
				}
			}
		
	}
	return 0;
// Fehler/Problem: Es läuft eine Seite immer weiter, obwohl nichts dadrunter ist
}
