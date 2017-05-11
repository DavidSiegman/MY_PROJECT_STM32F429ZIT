#ifndef __I2C_H 
#define __I2C_H

#include "stm32f429xx.h"

#define I2C3_SCL_PORT         GPIOA
#define I2C3_SCL_PIN          8      // AF4

#define I2C3_SDA_PORT         GPIOC
#define I2C3_SDA_PIN          9      // AF4

#define I2C3_INT1_PORT        GPIOA 
#define I2C3_INT1_PIN         15    // EXTI

#define STMPE811_Adress       0x82

void I2C3_Init(void);
void I2C3_Send_Data(unsigned char* data, unsigned char data_len);
void I2C3_Read_Data(unsigned char* data, unsigned char data_len);

#endif /* end __I2S_H */
