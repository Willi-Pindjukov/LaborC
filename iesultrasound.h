

#ifndef iesultrasound_h
#define iesultrasound_h

#define ECHO PB4
#define TRIGGER PB5

uint16_t cnt;
uint16_t echo_start;
uint16_t echo_end;
int16_t echo_duration;


//ISR (PCINT0_vect);
//ISR (TIMER1_COMPA_vect);
void setupTimer1();
void setupPCINTPB4();

#endif
