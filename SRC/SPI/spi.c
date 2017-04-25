#include "spi.h"

void SPI5_INIT(void)
{
	// ==================================================================================================
	// SPI5 PORTS INITIALISATION ========================================================================
	// ==================================================================================================
	int index = 0;
	
	RCC->AHB1RSTR |= RCC_AHB1RSTR_GPIOFRST;
	RCC->AHB1RSTR &= ~RCC_AHB1RSTR_GPIOFRST;
	RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOFEN;
	
	// SPI5_SCK_PIN Initialisation ================================================================== AF5
	SPI5_SCK_PORT->MODER &= ~(GPIO_MODER_MODE0_Msk << SPI5_SCK_PIN*2);
	SPI5_SCK_PORT->MODER |=  (GPIO_MODER_MODE0_1 << SPI5_SCK_PIN*2);
	
	SPI5_SCK_PORT->OSPEEDR &= ~(GPIO_MODER_MODE0_Msk << SPI5_SCK_PIN*2);
	SPI5_SCK_PORT->OSPEEDR |=  (GPIO_MODER_MODE0_Msk << SPI5_SCK_PIN*2);
	
	index = SPI5_SCK_PIN/8;
	SPI5_SCK_PORT->AFR[index] &= ~(GPIO_AFRL_AFRL0 << (SPI5_SCK_PIN - 8*index)*4);
	SPI5_SCK_PORT->AFR[index] |=  (5 << (SPI5_SCK_PIN - 8*index)*4);
	
	// SPI5_MISO_PIN Initialisation ================================================================= AF5
	SPI5_MISO_PORT->MODER &= ~(GPIO_MODER_MODE0_Msk << SPI5_MISO_PIN*2);
	SPI5_MISO_PORT->MODER |=  (GPIO_MODER_MODE0_1 << SPI5_MISO_PIN*2);
	
	SPI5_MISO_PORT->OSPEEDR &= ~(GPIO_MODER_MODE0_Msk << SPI5_MISO_PIN*2);
	SPI5_MISO_PORT->OSPEEDR |=  (GPIO_MODER_MODE0_Msk << SPI5_MISO_PIN*2);
	
	index = SPI5_MISO_PIN/8;
	SPI5_MISO_PORT->AFR[index] &= ~(GPIO_AFRL_AFRL0 << (SPI5_MISO_PIN - 8*index)*4);
	SPI5_MISO_PORT->AFR[index] |=  (5 << (SPI5_MISO_PIN - 8*index)*4);
	
	// SPI5_MISO_PIN Initialisation ================================================================ AF5
	SPI5_MOSI_PORT->MODER &= ~(GPIO_MODER_MODE0_Msk << SPI5_MOSI_PIN*2);
	SPI5_MOSI_PORT->MODER |=  (GPIO_MODER_MODE0_1 << SPI5_MOSI_PIN*2);
	
	SPI5_MOSI_PORT->OSPEEDR &= ~(GPIO_MODER_MODE0_Msk << SPI5_MOSI_PIN*2);
	SPI5_MOSI_PORT->OSPEEDR |=  (GPIO_MODER_MODE0_Msk << SPI5_MOSI_PIN*2);
	
	index = SPI5_MOSI_PIN/8;
	SPI5_MOSI_PORT->AFR[index] &= ~(GPIO_AFRL_AFRL0 << (SPI5_MOSI_PIN - 8*index)*4);
	SPI5_MOSI_PORT->AFR[index] |=  (5 << (SPI5_MOSI_PIN - 8*index)*4);
	
	// =================================================================================================
	// SPI5 PERIPHERIAL INITIALISATION =================================================================
	// =================================================================================================
	
	RCC->APB2RSTR |= RCC_APB2RSTR_SPI5RST;
	RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI5RST;
	RCC->APB2ENR  |= RCC_APB2ENR_SPI5EN;
	
	SPI5->CR1 |= SPI_CR1_MSTR;
	SPI5->CR1 |= SPI_CR1_SSM;
	SPI5->CR1 |= SPI_CR1_SSI;
	// APB2 CLK = 90 MHz
	SPI5->CR1 &= ~SPI_CR1_BR_Msk;
	SPI5->CR1 |= (3 << SPI_CR1_BR_Pos); //90 MHz / 16 = 5.625 MHz
}
void SPI5_SendByte(unsigned char byte)
{
	SPI5->DR = (unsigned char)(byte);
	while(!(SPI5->SR & SPI_SR_TXE));
}
unsigned char SPI5_ReadByte(void)
{
	while(!(SPI5->SR & SPI_SR_RXNE));
	return *(unsigned char*)(SPI5->DR);
}
void SPI5_Enable(void)
{
	SPI5->CR1 |= SPI_CR1_SPE;
}
void SPI5_Disable(void)
{
	SPI5->CR1 &= ~SPI_CR1_SPE;
}
void SPI5_BIDI_Enable(void)
{
	SPI5->CR1 |= SPI_CR1_BIDIMODE;
}
void SPI5_BIDI_Disable(void)
{
	SPI5->CR1 &= ~SPI_CR1_BIDIMODE;
}
void SPI5_BIDIO_Enable(void)
{
	SPI5->CR1 |= SPI_CR1_BIDIOE;
}
void SPI5_BIDIO_Disable(void)
{
	SPI5->CR1 &= ~SPI_CR1_BIDIOE;
}
