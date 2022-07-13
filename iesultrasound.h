


#define ECHO PB4
#define TRIGGER PB5


ISR (PCINT0_vect);
ISR (TIMER1_COMPA_vect);
void setupTimer1();
void setupPCINTPB4();
