/**
 * @file pmp.c
 * @brief Source for PMP functionality
 * @author Ayberk Özgür
 * @version 1.0
 * @date 2015-04-23
 */

#include"pmp.h"

void setupPMP(){
    PMCONbits.PTRDEN = 1;   //Read/write strobe port enable
    PMCONbits.CSF = 0b10;   //PMCSx behave like chip select
    PMCONbits.CS1P = 1;     //PMCS1 is active high
    PMCONbits.WRSP = 1;     //Write strobe active high
    PMCONbits.RDSP = 1;     //Read strobe active high

    PMMODEbits.IRQM = 0b00; //No interrupts for now
    PMMODEbits.MODE = 0b00; //Legacy slave mode

    PMADDRbits.CS1 = 1;     //PMCS1 is active

    PMCONbits.ON = 1;       //Turn on the PMP module
}
