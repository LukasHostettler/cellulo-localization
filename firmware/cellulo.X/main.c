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
    setupSPI();
    setupUART();
    setupPMP();

    while(1){
        Nop();
    }
}
