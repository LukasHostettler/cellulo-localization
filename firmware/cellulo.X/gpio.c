/**
 * @file gpio.h
 * @brief Source for GPIO pin functionality
 * @author Ayberk Özgür
 * @version 1.0
 * @date 2015-04-23
 */

#include"gpio.h"

void setupGPIO(){
    
    //BAT_PG input
    TRISBbits.TRISB5 = 1;   //Set as input
    ANSELBbits.ANSB5 = 0;   //Digital function

    //BAT_STAT2 input
    TRISBbits.TRISB4 = 1;   //Set as input
    ANSELBbits.ANSB4 = 0;   //Digital function

    //BAT_STAT1/LBO input
    TRISBbits.TRISB3 = 1;   //Set as input
    ANSELBbits.ANSB3 = 0;   //Digital function

    //VBAT_SENSE_ON output
    TRISBbits.TRISB12 = 0;  //Set as output
    ANSELBbits.ANSB12 = 0;  //Digital function
    VBAT_SENSE_ON = 0;      //Turn off for now

    //BAT_CE output
    TRISBbits.TRISB13 = 0;  //Set as output
    ANSELBbits.ANSB13 = 0;  //Digital function
    BAT_CE = 0;             //Turn off for now

    //BT_RESET_BAR output
    TRISCbits.TRISC12 = 0;  //Set as output
    BT_RESET_BAR = 1;       //Do not reset for now

    //BT_BAUD_9600 output
    TRISCbits.TRISC15 = 0;  //Set as output
    BT_BAUD_9600 = 1;       //Set baud rate to 9600 for now

    //TOUCH_RESET_BAR output
    TRISFbits.TRISF3 = 0;   //Set as output
    TOUCH_RESET_BAR = 1;    //Do not reset for now

    //TOUCH_CHANGE (interrupt on change) input
    TRISDbits.TRISD9 = 1;   //Set as input
                            //TODO: Setup interrupt on change

    //IMG_OE output
    TRISDbits.TRISD0 = 0;   //Set as output
    IMG_OE = 0;             //Turn off for now

    //LED_XLAT output
    TRISCbits.TRISC14 = 0;  //Set as output
    LED_XLAT = 0;           //Do not rise for now

    //IMG_RESET_BAR output
    TRISDbits.TRISD2 = 0;   //Set as output
    IMG_RESET_BAR = 1;      //Do not reset for now

    //IMG_EXPOSURE output
    TRISDbits.TRISD3 = 0;   //Set as output
    IMG_EXPOSURE = 0;       //Turn off for now

    //IMG_STANDBY output
    TRISFbits.TRISF0 = 0;   //Set as output
    IMG_STANDBY = 0;        //Turn off for now

    //PIXEL_LINE (interrupt on change) input
    TRISFbits.TRISF1 = 1;   //Set as input
}
