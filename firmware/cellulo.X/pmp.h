/**
 * @file pmp.h
 * @brief Header for PMP functionality
 * @author Ayberk Özgür
 * @version 1.0
 * @date 2015-04-23
 */

#ifndef PMP_H
#define	PMP_H

#ifdef __XC32
    #include <xc.h>
#endif

/**
 * @brief Sets up PMP pins
 */
void setupPMP();

#endif	/* PMP_H */

