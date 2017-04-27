#include "main.h"
#include "rcc.h"
#include "spi.h"
#include "lcd.h"
#include "cmsis_os.h"
#define MODE_INPUT 	0
#define MODE_GPIO		1
#define MODE_AF			2
#define MODE_ANALOG	3

#define TYPE_PUSHPULL 	0
#define TYPE_OPENDRAIN 	1

#define SPEED_2MHz		0
#define SPEED_25MHz		1
#define SPEED_50MHz		2
#define SPEED_100MHz	3

#define PULLUP_NONE		0
#define PULLUP_UP			1
#define PULLUP_DOWN		2

#define MASK1BIT(pin) ((uint32_t)~(1 << (pin * 1)))
#define MASK2BIT(pin) ((uint32_t)~(3 << (pin * 2)))
#define MASK4BIT(pin) ((uint32_t)~(15 << (pin * 4)))
#define AFMASKL(pin)	((uint32_t)~(15 << (pin * 4)))
#define AFMASKH(pin)	((uint32_t)~(15 << ((pin - 8) * 4)))

static GPIO_TypeDef* GPIOInitTable[] = {
		GPIOC, GPIOB, GPIOA, GPIOA, GPIOB, GPIOG,
		GPIOA, GPIOG, GPIOB, GPIOB, GPIOC, GPIOD,
		GPIOD, GPIOG, GPIOG, GPIOA, GPIOB, GPIOB,
		GPIOA, GPIOC, GPIOF, GPIOG,
		0};

static 	uint8_t const PINInitTable[] = {
		10, 0, 11, 12, 1, 6,
		6, 10, 10, 11, 7, 3,
		6, 11, 12, 3, 8, 9,
		4, 6, 10, 7,
		0};

static  uint8_t const AFInitTable[] = {
		14, 9, 14, 14, 9, 14,
		14, 9, 14, 14, 14, 14,
		14, 14, 9, 14, 14, 14,
		14, 14, 14, 14,
		0};

void MAINTimer_Callback(void const *pvParametrs);
void gpio_conf(GPIO_TypeDef * GPIO, uint8_t pin, uint8_t mode, uint8_t type, uint8_t speed, uint8_t pullup, uint8_t af)
{
		GPIO->MODER 	= (GPIO->MODER   & MASK2BIT(pin))   | (mode << (pin * 2));
		GPIO->OTYPER 	= (GPIO->OTYPER  & MASK1BIT(pin))   | (type << pin);
		GPIO->OSPEEDR = (GPIO->OSPEEDR & MASK2BIT(pin))   | (speed << (pin * 2));
		GPIO->PUPDR		= (GPIO->PUPDR   & MASK2BIT(pin))		| (pullup << (pin * 2));
		if(pin > 7)
			{
				GPIO->AFR[1] = (GPIO->AFR[1] & AFMASKH(pin)) | (af << ((pin - 8) * 4));
			}
		else
			{
				GPIO->AFR[0] = (GPIO->AFR[0] & AFMASKL(pin)) | (af << ((pin) * 4));
			}
}
int main(void)
{
	// INITIALISATION ========================================================
	// =======================================================================
	RCC_CLOCK_INIT();
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN |
                  RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOEEN | RCC_AHB1ENR_GPIOFEN |
                  RCC_AHB1ENR_GPIOGEN;
	SPI5_INIT();
	volatile static unsigned char in1[4] = {0,0,0,0};
	volatile static unsigned char in2[2] = {0,0};
	volatile static unsigned char in3[2] = {0,0};
	LCD_MCU_Read_Comand((unsigned char*)(in1), 0x09, 4);
	LCD_MCU_Read_Comand((unsigned char*)(in2), 0x0A, 2);
	LCD_MCU_Read_Comand((unsigned char*)(in3), 0x0C, 2);
	LCD_INIT();
	LCD_MCU_Read_Comand((unsigned char*)(in1), 0x09, 4);
	LCD_MCU_Read_Comand((unsigned char*)(in2), 0x0A, 2);
	LCD_MCU_Read_Comand((unsigned char*)(in3), 0x0C, 2);
	
	uint8_t i = 0;
	/* GPIO pin configuration */
	while(GPIOInitTable[i] != 0){
		gpio_conf(GPIOInitTable[i], PINInitTable[i], MODE_AF, TYPE_PUSHPULL, SPEED_100MHz, PULLUP_NONE, AFInitTable[i]);
		i++;
	}
	/* Enable SAI PLL */
	RCC->CR |= RCC_CR_PLLSAION;
	/* wait for SAI PLL ready */
	while((RCC->CR & RCC_CR_PLLSAIRDY) == 0);
	/* enable clock for LTDC */
	RCC->APB2ENR 	   |= RCC_APB2ENR_LTDCEN;
	/* Synchronization Size Configuration */
	LTDC->SSCR 			= ((ILI9341_HSYNC - 1) << 16) 	| (ILI9341_VSYNC - 1);
	/* Back Porch Configuration */
	LTDC->BPCR 			= ((ILI9341_HSYNC + ILI9341_HBP - 1) << 16) 	| (ILI9341_VSYNC + ILI9341_VBP - 1);
	/* Active Width Configuration */
	LTDC->AWCR 			= ((ILI9341_HSYNC + ILI9341_HBP + ILI9341_LCD_PIXEL_WIDTH - 1) << 16) | (ILI9341_VSYNC + ILI9341_VBP + ILI9341_LCD_PIXEL_HEIGHT - 1);
	/* Total Width Configuration */
	LTDC->TWCR 			= ((ILI9341_HSYNC + ILI9341_HBP + ILI9341_LCD_PIXEL_WIDTH + ILI9341_HFP - 1) << 16) 	| (ILI9341_VSYNC + ILI9341_VBP + ILI9341_LCD_PIXEL_HEIGHT + ILI9341_VFP - 1);
	LTDC_Layer1->CACR   = (20);
	LTDC_Layer1->DCCR   = (255 << 16);
	/* Enable Layer */
	LTDC_Layer1->CR 	= LTDC_LxCR_LEN;
	/* Window Horizontal Position Configuration */
	LTDC_Layer1->WHPCR  = 0x1E | ((0x1D) << 16);
	/* Window Vertical Position Configuration */
	LTDC_Layer1->WVPCR  = 4 | ((3) << 16);
	/* Pixel Format Configuration */
	LTDC_Layer1->PFCR   = 2;
	/* Color Frame Buffer Address */
	LTDC_Layer1->CFBAR  = 0xD0000000;
	/* Color Frame Buffer Length */
	LTDC_Layer1->CFBLR  = ((ILI9341_LCD_PIXEL_WIDTH * 2) << 16) | ((ILI9341_LCD_PIXEL_HEIGHT * 2) + 3);
	/* Immediate Reload */
	LTDC->SRCR 			= LTDC_SRCR_IMR;
	/* Enable LTDC */
	LTDC->GCR  			= LTDC_GCR_LTDCEN;
	
	static uint32_t  exec2 = 2;
	
	osTimerDef (MAINTimer, MAINTimer_Callback); 
  if (osTimerCreate (osTimer(MAINTimer), osTimerPeriodic, &exec2) != NULL)  {
    // Periodic timer created
  }
	
	osThreadDef(MAIN_Thread, MAINThread, osPriorityHigh, 0, 768);
  osThreadCreate (osThread(MAIN_Thread), NULL); 

	osKernelStart ();
	
	while(1)
	{
		
	}
}

void MAINThread(void const *pvParametrs)
{
	for( ;; ) 
	{ 
		osDelay(700); 
	}
}

void MAINTimer_Callback(void const *pvParametrs)
{
	
}
