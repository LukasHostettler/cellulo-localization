#ifdef __XC32
    #include <xc.h>
#endif

#include <stdint.h>
#include <stdbool.h> 

#include"analog.h"
#include"gpio.h"
#include"oscillator.h"
#include"pmp.h"
#include"spi.h"
#include"uart.h"

int32_t main(void){
    setupOscillator();
    setupGPIO();
    setupAnalog();
    setupI2C();
    setupSPI();
    setupUART();
    setupPMP();

    int i,j;

    for(j=0;j<7;j++){
        SPI1BUF = 0b00000000000000000000000000000000;
        for(i=0;i<200000;i++)
            Nop();
        
    }
    LED_XLAT = 1;
    for(i=0;i<200000;i++)
        Nop();
    LED_XLAT = 0;

    SPI1BUF = 0b00000000111111111111000000000000;
    for(i=0;i<200000;i++)
        Nop();
    SPI1BUF = 0b00000000000011111111111100000000;
    for(i=0;i<200000;i++)
        Nop();
    SPI1BUF = 0b00000000000000001111111111110000;
    for(i=0;i<200000;i++)
        Nop();
    SPI1BUF = 0b00000000000000000000111111111111;
    for(i=0;i<200000;i++)
        Nop();
    SPI1BUF = 0b00000000000000000000000011111111;
    for(i=0;i<200000;i++)
        Nop();
    SPI1BUF = 0b11110000000000000000000000001111;
    for(i=0;i<200000;i++)
        Nop();
    SPI1BUF = 0b11111111000000000000000000000000;
    for(i=0;i<200000;i++)
        Nop();
        
    LED_XLAT = 1;
    for(i=0;i<200000;i++)
        Nop();
    LED_XLAT = 0;

    while(1){
        Nop();
    }
}
