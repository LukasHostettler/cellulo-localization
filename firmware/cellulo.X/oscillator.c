/**
 * @file oscillator.c
 * @brief Source for oscillator configuration
 * @author Ayberk Özgür
 * @version 1.0
 * @date 2015-04-22
 */

#include "oscillator.h"

void setupOscillator(){

    //All necessary values are copied from configuration bits at power on

    while(OSCCONbits.SLOCK == 0);   //Wait until PLL locks
}
