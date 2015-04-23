/**
 * @file spi.h
 * @brief Header for SPI functionality
 * @author Ayberk Özgür
 * @version 1.0
 * @date 2015-04-23
 */

#ifndef SPI_H
#define	SPI_H

#ifdef __XC32
    #include <xc.h>
#endif

/**
 * @brief Sets up SPI pins
 */
void setupSPI();

#endif	/* SPI_H */
