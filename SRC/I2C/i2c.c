#include "i2c.h"

static I2C3_Structur I2C3_Struct;

void I2C3_Init(void)
{
	int index = 0;
	
	RCC->APB1ENR  |= RCC_APB1ENR_I2C3EN; //45 MHz
	
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
	
	EXTI->IMR  |= EXTI_IMR_MR15;
	EXTI->FTSR |= EXTI_FTSR_TR15;
	
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	
	I2C3->CR1  |= I2C_CR1_SWRST;
	I2C3->CR1  &= ~I2C_CR1_SWRST;
	
	I2C3->CR2  &= ~ I2C_CR2_FREQ_Msk;
	I2C3->CR2  |= (40 << I2C_CR2_FREQ_Pos); // 40MHz
	I2C3->CR2  |= I2C_CR2_ITEVTEN;
	I2C3->CR2  |= I2C_CR2_ITBUFEN;
	I2C3->CR2  |= I2C_CR2_ITERREN;

	NVIC_EnableIRQ(I2C3_ER_IRQn);
	
	NVIC_SetPriority(I2C3_EV_IRQn,15);
	NVIC_SetPriority(I2C3_ER_IRQn,14);

	I2C3->CCR  |= I2C_CCR_FS;               // FM Mode
	I2C3->CCR  |= I2C_CCR_DUTY;             //
	I2C3->CCR  &= ~ I2C_CCR_CCR_Msk;
	I2C3->CCR  |= (4 << I2C_CCR_CCR_Pos);   //4
	I2C3->TRISE = 12;
	I2C3->FLTR |= 0x01;
	I2C3->CR1  |= I2C_CR1_PE;
	
}

unsigned char I2C3_GET_GLOBAL_STATE(void)
{
	return I2C3_Struct.GLOBAL_STATE;
}

// Запуск передачи данных
// *data_a - буфер
// data_ln - количество байт данных
// mode - режим, 0-передача 1-приём
void I2C3_START(unsigned char adress, unsigned char* data_a, unsigned char data_ln, unsigned char mode)
{
	I2C3_Struct.GLOBAL_STATE = I2C_GLOBAL_STATE_BUSY; // Статус в состояние "BUSY"
	I2C3_Struct.reg_adress = adress;
	I2C3_Struct.data = data_a;
	I2C3_Struct.data_len = data_ln;
	I2C3_Struct.data_index = 0;
	I2C3_Struct.RW_Mode = mode;
	NVIC_ClearPendingIRQ(I2C3_EV_IRQn);
	NVIC_EnableIRQ(I2C3_EV_IRQn);
	I2C3->CR1  |= I2C_CR1_START;
}

void I2C3_STOP(void)
{
	I2C3->CR1  |= I2C_CR1_STOP;
	NVIC_DisableIRQ(I2C3_EV_IRQn);
	I2C3_Struct.GLOBAL_STATE = I2C_GLOBAL_STATE_REDY; // Статус в состояние "REDY"
}

// Event interrupt
void I2C3_EV_IRQHandler(void)
{
	if((I2C3->SR1 & I2C_SR1_SB))
	{
		I2C3->DR = STMPE811_Adress | I2C3_Struct.RW_Mode;
	}
	if((I2C3->SR1 & I2C_SR1_ADDR))
	{
		if (I2C3_Struct.RW_Mode == I2C_WRITE_EV_STATE)
		{
			if ((I2C3->SR2 & I2C_SR2_TRA))
			{
				I2C3->DR = I2C3_Struct.reg_adress;
			}
		}
		else if (I2C3_Struct.RW_Mode == I2C_READ_EV_STATE)
		{
			if (I2C3_Struct.data_len > 2)
			{
				I2C3->CR1  |= I2C_CR1_ACK;
				I2C3->CR1  &= ~I2C_CR1_POS;
				if (!(I2C3->SR2 & I2C_SR2_TRA))
				{
					
				}
				I2C3_Struct.data_len--;
			}
			else if (I2C3_Struct.data_len == 2)
			{
				I2C3->CR1  |= I2C_CR1_ACK;
				I2C3->CR1  |= I2C_CR1_POS;
				if (!(I2C3->SR2 & I2C_SR2_TRA))
				{
					
				}
				I2C3_Struct.data_len--;
			}
			else if (I2C3_Struct.data_len == 1)
			{
				I2C3->CR1  &= ~I2C_CR1_ACK;
				I2C3->CR1  &= ~I2C_CR1_POS;
				if (!(I2C3->SR2 & I2C_SR2_TRA))
				{
					
				}
				I2C3_Struct.data_len--;
			}
			else if (I2C3_Struct.data_len == 0)
			{
				I2C3->CR1  &= ~I2C_CR1_ACK;
				I2C3->CR1  &= ~I2C_CR1_POS;
				//I2C3_STOP();
				if (!(I2C3->SR2 & I2C_SR2_TRA))
				{
					
				}
			}
			
		}
	}

	if((I2C3->SR1 & I2C_SR1_RXNE)&& !(I2C3->SR2 & I2C_SR2_TRA))
	{
		if (I2C3_Struct.data_len > 1)
		{
			if ((I2C3->SR1 & I2C_SR1_BTF))
		   {
				I2C3->CR1  |= I2C_CR1_ACK;
				I2C3->CR1  &= ~I2C_CR1_POS;
				*(I2C3_Struct.data + I2C3_Struct.data_index) = I2C3->DR;
				I2C3_Struct.data_index++;
				I2C3_Struct.data_len--;
			}
		}
		else if (I2C3_Struct.data_len == 1)
		{
			if ((I2C3->SR1 & I2C_SR1_BTF))
		   {
				*(I2C3_Struct.data + I2C3_Struct.data_index) = I2C3->DR;
				I2C3_Struct.data_index++;
				I2C3_Struct.data_len--;
				I2C3->CR1  &= ~I2C_CR1_ACK;
				I2C3->CR1  &= ~I2C_CR1_POS;
				I2C3_STOP();
				*(I2C3_Struct.data + I2C3_Struct.data_index) = I2C3->DR;
				I2C3_Struct.data_index = 0;
				I2C3_Struct.data_len   = 0;
			}
		}
		else if (I2C3_Struct.data_len == 0)
		{
			I2C3->CR1  &= ~I2C_CR1_ACK;
			I2C3->CR1  &= ~I2C_CR1_POS;
			I2C3_STOP();
			*(I2C3_Struct.data + I2C3_Struct.data_index) = I2C3->DR;
			I2C3_Struct.data_index = 0;
			I2C3_Struct.data_len   = 0;
		}
	}
	else if((I2C3->SR1 & I2C_SR1_TXE)&&(I2C3->SR2 & I2C_SR2_TRA))
	{
		if (I2C3_Struct.data_len > 0)
		{
			if(I2C3->SR1 & I2C_SR1_BTF)
			{
				I2C3->DR = *(I2C3_Struct.data + I2C3_Struct.data_index);
				I2C3_Struct.data_index++;
				I2C3_Struct.data_len--;
			}
		}
		else if (I2C3_Struct.data_len == 0)
		{
			if(I2C3->SR1 & I2C_SR1_BTF)
			{
				I2C3_STOP();
			}
		}
	}
}
// Error interrupt
void I2C3_ER_IRQHandler(void)
{
	NVIC_ClearPendingIRQ(I2C3_ER_IRQn);
}
