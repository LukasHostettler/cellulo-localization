/**
 * @file analog.h
 * @brief Header for analog input pin functionality
 * @author Ayberk Özgür
 * @version 1.0
 * @date 2015-04-23
 */

#ifndef ANALOG_H
#define	ANALOG_H

#ifdef __XC32
    #include <xc.h>
#endif

/**
 * @brief Sets up analog pins
 */
void setupAnalog();

#endif	/* ANALOG_H */

