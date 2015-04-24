/**
 * @file i2c.c
 * @brief Source for I2C pin functionality
 * @author Ayberk Özgür
 * @version 1.0
 * @date 2015-04-23
 */

#include"i2c.h"

void setupI2C(){
    
    I2C5CONbits.ON = 1; //Enable I2C5
}
