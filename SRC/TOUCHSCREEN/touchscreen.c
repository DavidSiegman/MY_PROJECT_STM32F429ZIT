
#include "touchscreen.h"
#include "i2c.h"

void Init_TouchScreenController(void)
{
	
	
	
	unsigned char in_data_byte, out_data_byte;
	
	//a) Disable the clock gating for the touchscreen controller and ADC in the SYS_CFG2 register.
	in_data_byte = 0x0C; // SYS_CTRL2 = 0x0C;
	I2C3_START(SYS_CTRL2, &in_data_byte,  1, I2C_WRITE_EV_STATE);
	while(I2C3_GET_GLOBAL_STATE() != I2C_GLOBAL_STATE_REDY);
	
	//b) Configure the touchscreen operating mode and the window tracking index.
	in_data_byte = 0x20; // TSC_CTRL = 0x20
	I2C3_START(TSC_CTRL, &in_data_byte,  1, I2C_WRITE_EV_STATE);
	while(I2C3_GET_GLOBAL_STATE() != I2C_GLOBAL_STATE_REDY);
	
	//c) A touch detection status may also be enabled through enabling the 
	//   corresponding interrupt flag.  With this interrupt, the user is 
	//   informed through an interrupt when the touch is detected as well as lifted.
	in_data_byte = 0x1F; // INT_EN = 0x1F
	I2C3_START(INT_EN, &in_data_byte,  1, I2C_WRITE_EV_STATE);
	while(I2C3_GET_GLOBAL_STATE() != I2C_GLOBAL_STATE_REDY);
	
	in_data_byte = 0x03; // INT_CTRL = 0x03
	I2C3_START(INT_CTRL, &in_data_byte,  1, I2C_WRITE_EV_STATE);
	while(I2C3_GET_GLOBAL_STATE() != I2C_GLOBAL_STATE_REDY);
	
	//d) Configure the TSC_CFG register to specify the “panel voltage settling time”, 
	//   touch detection delays and the averaging method used.
	in_data_byte = 0x63; // TSC_CFG = 0x63
	I2C3_START(TSC_CFG, &in_data_byte,  1, I2C_WRITE_EV_STATE);
	while(I2C3_GET_GLOBAL_STATE() != I2C_GLOBAL_STATE_REDY);
	
	I2C3_START(TSC_CTRL, &out_data_byte, 0, I2C_WRITE_EV_STATE);
	while(I2C3_GET_GLOBAL_STATE() != I2C_GLOBAL_STATE_REDY);
	I2C3_START(TSC_CTRL, &out_data_byte, 1, I2C_READ_EV_STATE);
	while(I2C3_GET_GLOBAL_STATE() != I2C_GLOBAL_STATE_REDY);
	in_data_byte = 0x0D;
}
void EXTI15_10_IRQHandler(void)
{
	NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
}
