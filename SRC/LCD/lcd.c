#include "lcd.h"
#include "spi.h"

void LCD_INIT(void)
{
	RCC->AHB1RSTR |= RCC_AHB1RSTR_GPIOCRST;
	RCC->AHB1RSTR &= ~RCC_AHB1RSTR_GPIOCRST;
	RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOCEN;
	
	RCC->AHB1RSTR |= RCC_AHB1RSTR_GPIODRST;
	RCC->AHB1RSTR &= ~RCC_AHB1RSTR_GPIODRST;
	RCC->AHB1ENR  |= RCC_AHB1ENR_GPIODEN;
	
	// LCD_CSX_PIN Initialisation ================================================================== OUT
	LCD_CSX_PORT->MODER &= ~(GPIO_MODER_MODE0_Msk << LCD_CSX_PIN*2);
	LCD_CSX_PORT->MODER |=  (GPIO_MODER_MODE0_0 << LCD_CSX_PIN*2);
	
	LCD_CSX_PORT->OSPEEDR &= ~(GPIO_MODER_MODE0_Msk << LCD_CSX_PIN*2);
	LCD_CSX_PORT->OSPEEDR |=  (GPIO_MODER_MODE0_Msk << LCD_CSX_PIN*2);
	
	// LCD_CSX_PIN Initialisation ================================================================== OUT
	LCD_DCX_PORT->MODER &= ~(GPIO_MODER_MODE0_Msk << LCD_DCX_PIN*2);
	LCD_DCX_PORT->MODER |=  (GPIO_MODER_MODE0_0 << LCD_DCX_PIN*2);
	
	LCD_DCX_PORT->OSPEEDR &= ~(GPIO_MODER_MODE0_Msk << LCD_DCX_PIN*2);
	LCD_DCX_PORT->OSPEEDR |=  (GPIO_MODER_MODE0_Msk << LCD_DCX_PIN*2);
	
}
unsigned char LCD_MCU_Read_Comand(unsigned char comand)
{
	unsigned char result;
	LCD_CSX_PORT->ODR &= ~LCD_CSX_PIN;
	LCD_DCX_PORT->ODR |=  LCD_DCX_PIN;
	
	SPI5_BIDI_Enable();
	SPI5_BIDIO_Enable();
	SPI5_Enable();
	SPI5_SendByte(comand);
	SPI5_BIDIO_Disable();
	result = SPI5_ReadByte();
	
	LCD_DCX_PORT->ODR &=  ~LCD_DCX_PIN;
	LCD_CSX_PORT->ODR |=  LCD_CSX_PIN;
	return result;
}
void LCD_MCU_Write_Comand(unsigned char comand)
{
	LCD_CSX_PORT->ODR &= ~LCD_CSX_PIN;
	LCD_DCX_PORT->ODR &= ~LCD_DCX_PIN;
	
	SPI5_BIDI_Enable();
	SPI5_BIDIO_Enable();
	SPI5_Enable();
	SPI5_SendByte(comand);

	LCD_DCX_PORT->ODR |=  LCD_DCX_PIN;
	LCD_CSX_PORT->ODR |=  LCD_CSX_PIN;
}
