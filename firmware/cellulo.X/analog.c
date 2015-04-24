/**
 * @file analog.c
 * @brief Source for analog input pin functionality
 * @author Ayberk Özgür
 * @version 1.0
 * @date 2015-04-23
 */

#include"analog.h"

void setupAnalog(){

    //VBAT_SENSE
    TRISBbits.TRISB11 = 1;  //Set as input
    ANSELBbits.ANSB11 = 1;  //Analog functionality

    //TODO: SETUP ADC

}