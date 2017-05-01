#include "lcd.h"
#include "spi.h"

void LCD_INIT(void)
{
	//RCC->AHB1RSTR |= RCC_AHB1RSTR_GPIOCRST;
	//RCC->AHB1RSTR &= ~RCC_AHB1RSTR_GPIOCRST;
	RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOCEN;
	
	//RCC->AHB1RSTR |= RCC_AHB1RSTR_GPIODRST;
	//RCC->AHB1RSTR &= ~RCC_AHB1RSTR_GPIODRST;
	RCC->AHB1ENR  |= RCC_AHB1ENR_GPIODEN;
	
	// LCD_CSX_PIN Initialisation ================================================================== OUT
	LCD_CSX_PORT->MODER &= ~(GPIO_MODER_MODE0_Msk << LCD_CSX_PIN*2);
	LCD_CSX_PORT->MODER |=  (GPIO_MODER_MODE0_0 << LCD_CSX_PIN*2);
	
	LCD_CSX_PORT->OSPEEDR &= ~(GPIO_MODER_MODE0_Msk << LCD_CSX_PIN*2);
	LCD_CSX_PORT->OSPEEDR |=  (GPIO_MODER_MODE0_Msk << LCD_CSX_PIN*2);
	LCD_CSX_PORT->BSRR    |= (1 << LCD_CSX_PIN);
	
	// LCD_DCX_PIN Initialisation ================================================================== OUT
	LCD_DCX_PORT->MODER &= ~(GPIO_MODER_MODE0_Msk << LCD_DCX_PIN*2);
	LCD_DCX_PORT->MODER |=  (GPIO_MODER_MODE0_0 << LCD_DCX_PIN*2);
	
	LCD_DCX_PORT->OSPEEDR &= ~(GPIO_MODER_MODE0_Msk << LCD_DCX_PIN*2);
	LCD_DCX_PORT->OSPEEDR |=  (GPIO_MODER_MODE0_Msk << LCD_DCX_PIN*2);
	LCD_DCX_PORT->BSRR   |= (1 << LCD_DCX_PIN);
	
	// MEM_CSX_PIN Initialisation ================================================================== OUT
	MEM_CSX_PORT->MODER &= ~(GPIO_MODER_MODE0_Msk << MEM_CSX_PIN*2);
	MEM_CSX_PORT->MODER |=  (GPIO_MODER_MODE0_0 << MEM_CSX_PIN*2);
	
	MEM_CSX_PORT->OSPEEDR &= ~(GPIO_MODER_MODE0_Msk << MEM_CSX_PIN*2);
	MEM_CSX_PORT->OSPEEDR |=  (GPIO_MODER_MODE0_Msk << MEM_CSX_PIN*2);
	MEM_CSX_PORT->BSRR    |= (1 << MEM_CSX_PIN);
	/* Configure LCD */
	
  ili9341_WriteReg(0xCA);
  ili9341_WriteData(0xC3);
  ili9341_WriteData(0x08);
  ili9341_WriteData(0x50);
  ili9341_WriteReg(LCD_POWERB);
  ili9341_WriteData(0x00);
  ili9341_WriteData(0xC1);
  ili9341_WriteData(0x30);
  ili9341_WriteReg(LCD_POWER_SEQ);
  ili9341_WriteData(0x64);
  ili9341_WriteData(0x03);
  ili9341_WriteData(0x12);
  ili9341_WriteData(0x81);
  ili9341_WriteReg(LCD_DTCA);
  ili9341_WriteData(0x85);
  ili9341_WriteData(0x00);
  ili9341_WriteData(0x78);
  ili9341_WriteReg(LCD_POWERA);
  ili9341_WriteData(0x39);
  ili9341_WriteData(0x2C);
  ili9341_WriteData(0x00);
  ili9341_WriteData(0x34);
  ili9341_WriteData(0x02);
  ili9341_WriteReg(LCD_PRC);
  ili9341_WriteData(0x20);
  ili9341_WriteReg(LCD_DTCB);
  ili9341_WriteData(0x00);
  ili9341_WriteData(0x00);
	ili9341_WriteReg(LCD_DFC);
  ili9341_WriteData(0x01);
  ili9341_WriteData(0x8F);
  ili9341_WriteData(0x27);
  ili9341_WriteData(0x04);
  ili9341_WriteReg(LCD_FRMCTR1);
  ili9341_WriteData(0x00);
  ili9341_WriteData(0x1B);
  ili9341_WriteReg(LCD_POWER1);
  ili9341_WriteData(0x10);
  ili9341_WriteReg(LCD_POWER2);
  ili9341_WriteData(0x10);
  ili9341_WriteReg(LCD_VCOM1);
  ili9341_WriteData(0x45);
  ili9341_WriteData(0x15);
  ili9341_WriteReg(LCD_VCOM2);
  ili9341_WriteData(0x90);
  ili9341_WriteReg(LCD_MAC);
  ili9341_WriteData(0xC8);
  ili9341_WriteReg(LCD_3GAMMA_EN);
  ili9341_WriteData(0x00);
  ili9341_WriteReg(LCD_RGB_INTERFACE);
  ili9341_WriteData(0xC2);
	ili9341_WriteReg(LCD_PIXEL_FORMAT);
  ili9341_WriteData(0x55);
	ili9341_WriteReg(LCD_WDB);
	ili9341_WriteData(0x00);
	ili9341_WriteReg(LCD_TEOFF);
  
	//ili9341_WriteReg(LCD_RGBSET);
	//for(unsigned int i = 0; i < 128;i++)
	//{
	//	ili9341_WriteData(i+2);
	//}
  //Colomn address set 
  //ili9341_WriteReg(LCD_COLUMN_ADDR);
  //ili9341_WriteData(0x00);
  //ili9341_WriteData(0x00);
  //ili9341_WriteData(0x00);
  //ili9341_WriteData(0xEF);
  // Page address set 
  //ili9341_WriteReg(LCD_PAGE_ADDR);
  //ili9341_WriteData(0x00);
  //ili9341_WriteData(0x00);
  //ili9341_WriteData(0x01);
  //ili9341_WriteData(0x3F);
  ili9341_WriteReg(LCD_INTERFACE);
  ili9341_WriteData(0x01);
  ili9341_WriteData(0x10);
  ili9341_WriteData(0x06);
 
  ili9341_WriteReg(LCD_GAMMA);
  ili9341_WriteData(0x01);
  
  ili9341_WriteReg(LCD_PGAMMA);
  ili9341_WriteData(0x0F);
  ili9341_WriteData(0x29);
  ili9341_WriteData(0x24);
  ili9341_WriteData(0x0C);
  ili9341_WriteData(0x0E);
  ili9341_WriteData(0x09);
  ili9341_WriteData(0x4E);
  ili9341_WriteData(0x78);
  ili9341_WriteData(0x3C);
  ili9341_WriteData(0x09);
  ili9341_WriteData(0x13);
  ili9341_WriteData(0x05);
  ili9341_WriteData(0x17);
  ili9341_WriteData(0x11);
  ili9341_WriteData(0x00);
  ili9341_WriteReg(LCD_NGAMMA);
  ili9341_WriteData(0x00);
  ili9341_WriteData(0x16);
  ili9341_WriteData(0x1B);
  ili9341_WriteData(0x04);
  ili9341_WriteData(0x11);
  ili9341_WriteData(0x07);
  ili9341_WriteData(0x31);
  ili9341_WriteData(0x33);
  ili9341_WriteData(0x42);
  ili9341_WriteData(0x05);
  ili9341_WriteData(0x0C);
  ili9341_WriteData(0x0A);
  ili9341_WriteData(0x28);
  ili9341_WriteData(0x2F);
  ili9341_WriteData(0x0F);
	
  LCD_Delay(1000);
  ili9341_WriteReg(LCD_SLEEP_OUT);
  LCD_Delay(200);
  ili9341_WriteReg(LCD_DISPLAY_ON);
  /* GRAM start writing */
	ili9341_WriteReg(LCD_GRAM);
}
void LCD_MCU_Read_Comand(unsigned char* parameters,unsigned char comand, unsigned char numb_of_parameters)
{

	SPI5_BIDI_Enable();
	SPI5_BIDIO_Enable();
	SPI5_Enable();
	LCD_CSX_PORT->BSRR    |= (1 << (LCD_CSX_PIN+16));
	LCD_DCX_PORT->BSRR    |= (1 << (LCD_DCX_PIN+16));

	SPI5_SendByte(comand);
	
	LCD_DCX_PORT->BSRR    |= (1 << LCD_DCX_PIN);
	SPI5_Disable();
	SPI5_BIDIO_Disable();
	SPI5_Enable();
	
	for (unsigned int i = 0; i < numb_of_parameters; i++)
	{
		//LCD_CSX_PORT->ODR &= ~LCD_CSX_PIN;
		
		*(parameters + i) = SPI5_ReadByte();
		//LCD_CSX_PORT->ODR |= LCD_CSX_PIN;
	}
	LCD_CSX_PORT->BSRR    |= (1 << LCD_CSX_PIN);
	LCD_DCX_PORT->BSRR    |= (1 << LCD_DCX_PIN);
	SPI5_Disable();
}
void ili9341_WriteReg(unsigned char comand)
{
	SPI5_BIDI_Enable();
	SPI5_BIDIO_Enable();
	SPI5_Enable();
	
	LCD_CSX_PORT->BSRR    |= (1 << (LCD_CSX_PIN+16));
	LCD_DCX_PORT->BSRR    |= (1 << (LCD_DCX_PIN+16));

	SPI5_SendByte(comand);

	LCD_CSX_PORT->BSRR    |= (1 << LCD_CSX_PIN);
	SPI5_Disable();
}
void ili9341_WriteData(unsigned char comand)
{
	SPI5_BIDI_Enable();
	SPI5_BIDIO_Enable();
	SPI5_Enable();
	
	LCD_DCX_PORT->BSRR    |= (1 << LCD_DCX_PIN);
	LCD_CSX_PORT->BSRR    |= (1 << (LCD_CSX_PIN+16));

	SPI5_SendByte(comand);

	LCD_CSX_PORT->BSRR    |= (1 << LCD_CSX_PIN);
	SPI5_Disable();
}
void LCD_Delay(int index)
{
	index *= 1000;
	while(index > 0)
	{
		index--;
	}
}
