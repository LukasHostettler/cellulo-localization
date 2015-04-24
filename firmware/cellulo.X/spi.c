/**
 * @file spi.c
 * @brief Source for SPI functionality
 * @author Ayberk Özgür
 * @version 1.0
 * @date 2015-04-23
 */

#include"spi.h"

void setupSPI(){
    RPC13R = 0b0101;        //Remap RPC13 to SDO1
    
    SPI1BRG = 24;           //F_SCK = F_PB/(2*(SPI1_BRG + 1)) = 100MHz/(2*(24 + 1)) = 4MHz
    
    SPI1CONbits.MODE32 = 1; //32-bit mode
    SPI1CONbits.MSTEN = 1;  //Master mode
    SPI1CONbits.DISSDI = 1; //SDI bit is disabled
    SPI1CONbits.ENHBUF = 0; //Disable enhanced buffer for now
    SPI1CONbits.CKE = 1;    //Data changes on falling edge

    IEC3bits.SPI1EIE = 0;   //Disable SPI1 fault interrupt
    IEC3bits.SPI1RXIE = 0;  //Disable SPI1 receive interrupt
    IEC3bits.SPI1TXIE = 0;  //Disable SPI1 transmit interrupt

    SPI1CONbits.ON = 1;     //Enable SPI1 module
}
