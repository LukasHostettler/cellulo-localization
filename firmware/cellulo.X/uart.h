/**
 * @file uart.h
 * @brief Header for UART functionality
 * @author Ayberk Özgür
 * @version 1.0
 * @date 2015-04-23
 */

#ifndef UART_H
#define	UART_H

#ifdef __XC32
    #include <xc.h>
#endif

/**
 * @brief Sets up UART pins
 */
void setupUART();

#endif	/* UART_H */

