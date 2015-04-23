/**
 * @file oscillator.h
 * @brief Header for oscillator configuration
 * @author Ayberk Özgür
 * @version 1.0
 * @date 2015-04-22
 */

#ifndef OSCILLATOR_H
#define	OSCILLATOR_H

#ifdef __XC32
    #include <xc.h>          /* Defines special funciton registers, CP0 regs  */
#endif

#define SYSCLK       200000000L

/**
 * @brief Sets up oscillator configuration after reset
 */
void setupOscillator();

#endif	/* OSCILLATOR_H */
