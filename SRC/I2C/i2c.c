#include "i2c.h"

void I2C3_Init(void)
{
	int index = 0;
	
	RCC->APB1ENR |= RCC_APB1ENR_I2C3EN; //45 MHz
	
	RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOAEN;
	RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOCEN;
	
	// I2C3_SCL_PIN Initialisation ================================================================== AF4
	I2C3_SCL_PORT->MODER &= ~(GPIO_MODER_MODE0_Msk << I2C3_SCL_PIN*2);
	I2C3_SCL_PORT->MODER |=  (GPIO_MODER_MODE0_1 << I2C3_SCL_PIN*2);
	
	I2C3_SCL_PORT->OSPEEDR &= ~(GPIO_MODER_MODE0_Msk << I2C3_SCL_PIN*2);
	I2C3_SCL_PORT->OSPEEDR |=  (GPIO_MODER_MODE0_Msk << I2C3_SCL_PIN*2);
	
	index = I2C3_SCL_PIN/8;
	I2C3_SCL_PORT->AFR[index] &= ~(GPIO_AFRL_AFRL0 << (I2C3_SCL_PIN - 8*index)*4);
	I2C3_SCL_PORT->AFR[index] |=  (4 << (I2C3_SCL_PIN - 8*index)*4);
	
	// I2C3_SDA_PIN Initialisation ================================================================= AF4
	I2C3_SDA_PORT->MODER &= ~(GPIO_MODER_MODE0_Msk << I2C3_SDA_PIN*2);
	I2C3_SDA_PORT->MODER |=  (GPIO_MODER_MODE0_1 << I2C3_SDA_PIN*2);
	
	I2C3_SDA_PORT->OSPEEDR &= ~(GPIO_MODER_MODE0_Msk << I2C3_SDA_PIN*2);
	I2C3_SDA_PORT->OSPEEDR |=  (GPIO_MODER_MODE0_Msk << I2C3_SDA_PIN*2);
	
	index = I2C3_SDA_PIN/8;
	I2C3_SDA_PORT->AFR[index] &= ~(GPIO_AFRL_AFRL0 << (I2C3_SDA_PIN - 8*index)*4);
	I2C3_SDA_PORT->AFR[index] |=  (4 << (I2C3_SDA_PIN - 8*index)*4);
	
	// I2C3_INT1_PIN Initialisation ================================================================ AF5
	I2C3_INT1_PORT->MODER &= ~(GPIO_MODER_MODE0_Msk << I2C3_INT1_PIN*2);
	I2C3_INT1_PORT->PUPDR |= (1 << I2C3_INT1_PIN);
	I2C3_INT1_PORT->OSPEEDR &= ~(GPIO_MODER_MODE0_Msk << I2C3_INT1_PIN*2);
	I2C3_INT1_PORT->OSPEEDR |=  (GPIO_MODER_MODE0_Msk << I2C3_INT1_PIN*2);
	
	// EXTI Initialisation
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI15;
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI15_PA;
	
	EXTI->IMR |= EXTI_IMR_MR15;
	EXTI->FTSR |= EXTI_FTSR_TR15;
	
	I2C3->CR2  &= ~ I2C_CR2_FREQ_Msk;
	I2C3->CR2  |= (40 << I2C_CR2_FREQ_Pos); // 40MHz
	
	I2C3->CCR  |= I2C_CCR_FS;               // FM Mode
	I2C3->CCR  |= I2C_CCR_DUTY;             //
	I2C3->CCR  &= ~ I2C_CCR_CCR_Msk;
	I2C3->CCR  |= (4 << I2C_CCR_CCR_Pos);   //4
	I2C3->TRISE = 12;
	I2C3->FLTR |= 0x01;
	I2C3->CR1  |= I2C_CR1_PE;
}

void I2C3_Send_Data(unsigned char* data, unsigned char data_len)
{
	I2C3->CR1  |= I2C_CR1_START;
	while(!(I2C3->SR1 & I2C_SR1_SB));
	I2C3->DR = STMPE811_Adress;
	while(!(I2C3->SR1 & I2C_SR1_ADDR));
	while(!(I2C3->SR1 & I2C_SR1_TXE));
	I2C3->DR = data[0];
}
void I2C3_Read_Data(unsigned char* data, unsigned char data_len)
{
	
}