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

enum
{
	I2C_WRITE_EV_STATE = 0,
	I2C_READ_EV_STATE,
	
	I2C_IDLE_STATE,
	I2C_START_EV_STATE,
	I2C_ADRESS_EV_STATE,
	I2C_STOP_EV_STATE,

	I2C_GLOBAL_STATE_BUSY,
	I2C_GLOBAL_STATE_REDY,
};

typedef struct
{
	volatile unsigned char  GLOBAL_STATE;
	volatile unsigned char  GLOBAL_MODE;
	volatile unsigned char  RW_Mode;
	volatile unsigned char  reg_adress;
	volatile unsigned char* data;
	volatile unsigned char  data_index;
	volatile unsigned char  data_len;
}I2C3_Structur;

void I2C3_Init(void);
void I2C3_START(unsigned char adress, unsigned char* data_a, unsigned char data_ln, unsigned char mode);
unsigned char I2C3_GET_GLOBAL_STATE(void);

#endif /* end __I2S_H */
