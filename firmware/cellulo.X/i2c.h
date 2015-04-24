/**
 * @file i2c.h
 * @brief Header for I2C pin functionality
 * @author Ayberk Özgür
 * @version 1.0
 * @date 2015-04-23
 */

#ifndef I2C_H
#define	I2C_H

#ifdef __XC32
    #include <xc.h>
#endif

/**
 * @brief Sets up I2C pins
 */
void setupI2C();

#endif	/* I2C_H */

