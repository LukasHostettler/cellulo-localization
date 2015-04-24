/**
 * @file uart.c
 * @brief Source for UART functionality
 * @author Ayberk Özgür
 * @version 1.0
 * @date 2015-04-23
 */

#include"uart.h"

void setupUART(){
    TRISGbits.TRISG7 = 1;   //RG7 is input
    U3RXR = 0b0001;         //RPG7 is mapped to UART3 RX

    TRISGbits.TRISG8 = 1;   //RG8 is input

    RPG6R = 0b0010;         //RPG6 is mapped to UART4 TX

    TRISGbits.TRISG9 = 0;   //RPG9 is output
    PORTGbits.RG9 = 0;      //Do not disable Bluetooth UART transmitter
}
