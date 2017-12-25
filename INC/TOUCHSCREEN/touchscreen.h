#ifndef __TOUCHSCREEN_H 
#define __TOUCHSCREEN_H

#include "stm32f429xx.h"

// Registers of STMPE811
#define CHIP_ID       0x00    
#define ID_VER        0x02
#define SYS_CTRL1     0x03
#define SYS_CTRL2     0x03
#define SPI_CFG       0x08
#define INT_CTRL      0x09
#define INT_EN        0x0A
#define INT_STA       0x0B
#define GPIO_INT_EN   0x0C
#define GPIO_INT_STA  0x0D
#define ADC_INT_EN    0x0E
#define ADC_INT_STA   0x0F

#define TSC_CTRL      0x40
#define TSC_CFG       0x41

void Init_TouchScreenController(void);

#endif /* end __TOUCHSCREEN_H */
