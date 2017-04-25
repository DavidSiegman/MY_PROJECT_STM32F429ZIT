#ifndef __LCD_H 
#define __LCD_H

#include "stm32f4xx.h"

#define LCD_CSX_PORT GPIOC
#define LCD_CSX_PIN  2

#define LCD_DCX_PORT GPIOD
#define LCD_DCX_PIN  13

void LCD_INIT(void);
unsigned char LCD_MCU_Read_Comand(unsigned char comand);
void LCD_MCU_Write_Comand(unsigned char comand);

#endif /* end __LCD_H */
