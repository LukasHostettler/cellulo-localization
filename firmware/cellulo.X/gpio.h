/**
 * @file gpio.h
 * @brief Header for GPIO pin functionality
 * @author Ayberk Özgür
 * @version 1.0
 * @date 2015-04-23
 */

#ifndef GPIO_H
#define	GPIO_H

#ifdef __XC32
    #include <xc.h>
#endif

#define BAT_PG          PORTBbits.RB5
#define BAT_STAT2       PORTBbits.RB4
#define BAT_STAT1_LBO   PORTBbits.RB3
#define VBAT_SENSE_ON   PORTBbits.RB12
#define BAT_CE          PORTBbits.RB13
#define BT_RESET_BAR    PORTCbits.RC12
#define BT_BAUD_9600    PORTCbits.RC15
#define TOUCH_RESET_BAR PORTFbits.RF3
#define TOUCH_CHANGE    PORTDbits.RD9
#define IMG_OE          PORTDbits.RD0
#define LED_XLAT        PORTCbits.RC14
#define IMG_RESET_BAR   PORTDbits.RD2
#define IMG_EXPOSURE    PORTDbits.RD3
#define IMG_STANDBY     PORTFbits.RF0
#define PIXEL_LINE      PORTFbits.RF1

/**
 * @brief Sets up GPIO pins
 */
void setupGPIO();

#endif	/* GPIO_H */

