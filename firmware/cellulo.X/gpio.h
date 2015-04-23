/**
 * @file gpio.h
 * @brief Header for gpio pin functionality
 * @author Ayberk Özgür
 * @version 1.0
 * @date 2015-04-23
 */

#ifndef GPIO_H
#define	GPIO_H

#ifdef __XC32
    #include <xc.h>
#endif

/**
 * @brief Sets up GPIO pins
 */
void setupGPIO();

#endif	/* GPIO_H */

