#ifdef __XC32
    #include <xc.h>
#endif

#include <stdint.h>
#include <stdbool.h> 

#include"oscillator.h"

int32_t main(void){
    setupOscillator();

    while(1){
        Nop();
    }
}
