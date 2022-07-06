#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "iesusart.h"
#include "iessreg.h"



/**
  Clocks the real hardware -- whenever this is called,
  a rising edge on PD4 is generated.
*/
void clk() {
    OR_SR_CLK &= ~(1 << OP_SR_CLK);
    OR_SR_CLK |= (1 << OP_SR_CLK);
    // There may be dragons in here. How long has a
    // clock pulse to be high? How do you know?
    OR_SR_CLK &= ~(1 << OP_SR_CLK);
}

/**
  Writes the in-memory-representation (the "model") of
  the robot's shift-register to the real hardware.
*/
void update_hardware(srr_t *regmdl) {
    for(cntv8_t i = 0; i < REGWIDTH; i++) {
        unsigned char position_set = (*regmdl & (1 << i));
        if (position_set) {
           OR_SR_DATA |= (1 << OP_SR_DATA);
           switch (position_set) {
              case (1 << SR_LED_BLUE):
                 //USART_print("LEFT LED SET/LD FIRES!\n\n");
                 break;
              case (1 << SR_LED_GREEN):
                 //USART_print("MIDDLE LED SET/LF FIRES!\n\n");
                 break;
              case (1 << SR_LED_YELLOW):
                 //USART_print("RIGHT LED SET/LF FIRES!\n\n");
                 break;
              default:
				 break;
                 //USART_print("NO OR MULTIPLE LFs FIRE!\n\n");
           }
        } else {
           OR_SR_DATA &= ~(1 << OP_SR_DATA);
        }
        clk();
    }
}

/**
  Updates the model (memory), that represents the
  robot's shift-register.
*/
void update_model(srr_t *regmdl, char left, char middle, char right) {

    if (right) {
        *regmdl |= (1 << SR_LED_YELLOW);
    } else {
        *regmdl &= ~(1 << SR_LED_YELLOW);
    }

    if (middle) {
        *regmdl |= (1 << SR_LED_GREEN);
    } else {
        *regmdl &= ~(1 << SR_LED_GREEN);
    }

    if (left) {
        *regmdl |= (1 << SR_LED_BLUE);
    } else {
        *regmdl &= ~(1 << SR_LED_BLUE);
    }

}

/**
  Clears the in-memory-representation (the "model") of
  the robot's shift-register.
*/
void clear(srr_t *regmdl) {
    *regmdl = 0;
    update_hardware(regmdl);
}

/**
  Sets data directions.
*/
void setup_ddr_all() {
    // Set Data Direction Register B2 as output.
    DR_SR_DATA = 1 << DP_SR_DATA;

    // Set Data Direction Register D4 as output. To D4,
    // the CLK-line of the robot's shift-register is connected.
    DR_SR_CLK = 1 << DP_SR_CLK;

    // Set Data Direction Register C[0|1|2] as input. To these pins,
    // the robot's line-infrared-reflection-sensors are attached.
    DR_LF_L &= ~(1 << DP_LF_L);
    DR_LF_M &= ~(1 << DP_LF_R);
    DR_LF_M &= ~(1 << DP_LF_M);
}

/*int main(void) {
    // Setup everything
    setup_ddr_all();
    USART_init(UBRR_SETTING);

    // Allocate 1 byte in memory/on heap for a representation (model)
    // of the register and clear the contents directly, and update
    // everything accordingly.
    srr_t *regmdl = malloc(sizeof(srr_t));
    clear(regmdl);

    srr_t last_model_state = *regmdl;

    while (1) {

        update_model(regmdl);

        if (*regmdl != last_model_state) {
           update_hardware(regmdl);
           last_model_state = *regmdl;
        }

    }

    return 0;
}*/
