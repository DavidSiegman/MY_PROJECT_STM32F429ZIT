#include "main.h"
#include "rcc.h"
#include "spi.h"
#include "lcd.h"
#include "cmsis_os.h"

#define SDRAM_BASE 0xD0000000
#define SDRAM_SIZE 0x00800000

#define TMRD(x) (x << 0)
#define TXSR(x) (x << 4)
#define TRAS(x) (x << 8)
#define TRC(x)  (x << 12)
#define TWR(x)  (x << 16)
#define TRP(x)  (x << 20)
#define TRCD(x) (x << 24)

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
		GPIOB, GPIOA, GPIOA, GPIOB, GPIOG,
		GPIOA, GPIOG, GPIOB, GPIOB, GPIOC, GPIOD,
		GPIOG, GPIOG, GPIOA, GPIOB, GPIOB,
		GPIOA, GPIOC, GPIOF, GPIOG,
		0};

static 	uint8_t const PINInitTable[] = {
		0, 11, 12, 1, 6,
		6, 10, 10, 11, 7, 3,
		11, 12, 3, 8, 9,
		4, 6, 10, 7,
		0};

static  uint8_t const AFInitTable[] = {
		9, 14, 14, 9, 14,
		14, 9, 14, 14, 14, 14,
		14, 9, 14, 14, 14,
		14, 14, 14, 14,
		0};
//=================================================================================================
// GPIO configuration data
//=================================================================================================
static  GPIO_TypeDef * const GPIOInitTableSDRAM[] = {
		GPIOF, GPIOF, GPIOF, GPIOF, GPIOF, GPIOF, GPIOF, GPIOF, GPIOF, GPIOF, GPIOG, GPIOG,
		GPIOD, GPIOD, GPIOD, GPIOD, GPIOE, GPIOE, GPIOE, GPIOE, GPIOE, GPIOE, GPIOE, GPIOE, GPIOE,
		GPIOD, GPIOD, GPIOD,
		GPIOB, GPIOB, GPIOC, GPIOE, GPIOE, GPIOF, GPIOG, GPIOG, GPIOG, GPIOG,
		0
};
static uint8_t const PINInitTableSDRAM[] = {
		0, 1, 2, 3, 4, 5, 12, 13, 14, 15, 0, 1,
		14, 15, 0, 1, 7, 8, 9, 10, 11, 12, 13, 14, 15,
		8, 9, 10,
		5, 6, 0, 0, 1, 11, 4, 5, 8, 15,
		0
};
			
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

//=================================================================================================
// SDRAM_Init function
//=================================================================================================
void SDRAM_Init(void)
{
	volatile uint32_t ptr = 0;
	volatile uint32_t i = 0;

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOEEN | RCC_AHB1ENR_GPIOFEN | RCC_AHB1ENR_GPIOGEN;

	while(GPIOInitTableSDRAM[i] != 0){
		gpio_conf(GPIOInitTableSDRAM[i], PINInitTableSDRAM[i],  MODE_AF, TYPE_PUSHPULL, SPEED_100MHz, PULLUP_NONE, 12);
		i++;
	}
	
	RCC->AHB3ENR |= RCC_AHB3ENR_FMCEN;	
	// Initialization step 1
	FMC_Bank5_6->SDCR[0] = FMC_SDCR1_SDCLK_1  | FMC_SDCR1_RBURST | FMC_SDCR1_RPIPE_1;
	FMC_Bank5_6->SDCR[1] = FMC_SDCR1_NR_0	  | FMC_SDCR1_MWID_0 | FMC_SDCR1_NB | FMC_SDCR1_CAS;
	// Initialization step 2
	FMC_Bank5_6->SDTR[0] = TRC(7)  | TRP(2);
	FMC_Bank5_6->SDTR[1] = TMRD(2) | TXSR(7) | TRAS(4) | TWR(2) | TRCD(2);
	// Initialization step 3	
	while(FMC_Bank5_6->SDSR & FMC_SDSR_BUSY);
	FMC_Bank5_6->SDCMR 	 = 1 | FMC_SDCMR_CTB2 | (1 << 5);
	// Initialization step 4
	for(i = 0; i  < 1000000; i++);
	// Initialization step 5
	while(FMC_Bank5_6->SDSR & FMC_SDSR_BUSY);
	FMC_Bank5_6->SDCMR 	 = 2 | FMC_SDCMR_CTB2 | (1 << 5);
	// Initialization step 6
	while(FMC_Bank5_6->SDSR & FMC_SDSR_BUSY);
	FMC_Bank5_6->SDCMR 	 = 3 | FMC_SDCMR_CTB2 | (4 << 5);	
	// Initialization step 7
	while(FMC_Bank5_6->SDSR & FMC_SDSR_BUSY);
	FMC_Bank5_6->SDCMR 	 = 4 | FMC_SDCMR_CTB2 | (1 << 5) | (0x231 << 9);
	// Initialization step 8
	while(FMC_Bank5_6->SDSR & FMC_SDSR_BUSY);
	FMC_Bank5_6->SDRTR |= (683 << 1);
	while(FMC_Bank5_6->SDSR & FMC_SDSR_BUSY);
}

int main(void)
{
	// INITIALISATION ========================================================
	// =======================================================================
	RCC_CLOCK_INIT();
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN |
                  RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOEEN | RCC_AHB1ENR_GPIOFEN |
                  RCC_AHB1ENR_GPIOGEN;
	SDRAM_Init();
	int index = 0, line = 0;
	for(unsigned int i = 0; i < ILI9341_LCD_PIXEL_WIDTH * ILI9341_LCD_PIXEL_HEIGHT;i++)
	{
		*((uint16_t *)(SDRAM_BASE)+i) = 0x0000;
		
		if (i%240 == 0)
		{
			line = i/240;
		}
		if (i%10 == 0)
		{
			*((uint16_t *)(SDRAM_BASE)+i) = 0xFFFF;
		}
		if ((i > 0)&&(i%240 == 0))
		{
			*((uint16_t *)(SDRAM_BASE)+i-1) = 0xFFFF;
		}
		if ((line%10 == 0)||(line == 319))
		{
			*((uint16_t *)(SDRAM_BASE)+i) = 0xFFFF;
		}
	}
	
	uint8_t i = 0;
	/* GPIO pin configuration */
	while(GPIOInitTable[i] != 0){
		gpio_conf(GPIOInitTable[i], PINInitTable[i], MODE_AF, TYPE_PUSHPULL, SPEED_100MHz, PULLUP_NONE, AFInitTable[i]);
		i++;
	}
	GPIOC->MODER &= ~(GPIO_MODER_MODE0_Msk << 10*2);
	GPIOC->MODER |=  (GPIO_MODER_MODE0_0 << 10*2);
	
	GPIOC->OSPEEDR &= ~(GPIO_MODER_MODE0_Msk << 10*2);
	GPIOC->OSPEEDR |=  (GPIO_MODER_MODE0_Msk << 10*2);
	GPIOC->BSRR    |= (1 << (10+16));
	
	GPIOD->MODER &= ~(GPIO_MODER_MODE0_Msk << 6*2);
	GPIOD->MODER |=  (GPIO_MODER_MODE0_0 << 6*2);
	
	GPIOD->OSPEEDR &= ~(GPIO_MODER_MODE0_Msk << 6*2);
	GPIOD->OSPEEDR |=  (GPIO_MODER_MODE0_Msk << 6*2);
	GPIOD->BSRR    |= (1 << (6+16));

	SPI5_INIT();
	volatile static unsigned char in1[4] = {0,0,0,0};
	volatile static unsigned char in2[2] = {0,0};
	volatile static unsigned char in3[2] = {0,0};
	LCD_INIT();
	LCD_MCU_Read_Comand((unsigned char*)(in1), 0x09, 4);
	LCD_MCU_Read_Comand((unsigned char*)(in2), 0x0A, 2);
	LCD_MCU_Read_Comand((unsigned char*)(in3), 0x0C, 2);
	
	
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
	
	LTDC->BCCR = LTDC_LxDCCR_DCRED_Msk;
	LTDC_Layer1->CACR   = (255);
	/* Enable Layer */
	LTDC_Layer1->CR 	= LTDC_LxCR_LEN;
	/* Window Horizontal Position Configuration */
	LTDC_Layer1->WHPCR  = (0x10D << 16) | 0x1E;
	/* Window Vertical Position Configuration */
	LTDC_Layer1->WVPCR  = (0x145 << 16) | 0x06;
	/* Pixel Format Configuration */
	LTDC_Layer1->PFCR   = 2;
	/* Color Frame Buffer Address */
	LTDC_Layer1->CFBAR  = SDRAM_BASE;
	/* Color Frame Buffer Length */
	LTDC_Layer1->CFBLR  = ((ILI9341_LCD_PIXEL_WIDTH * 2) << 16) | (ILI9341_LCD_PIXEL_WIDTH * 2 + 3);
	LTDC_Layer1->CFBLNR = ILI9341_LCD_PIXEL_HEIGHT;
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
	
	static unsigned short *curr_pix = (uint16_t *)(SDRAM_BASE+10);
	static unsigned short *pre_pix = (uint16_t *)(SDRAM_BASE);
	for( ;; ) 
	{		/* GRAM start writing */
		//ili9341_WriteReg(LCD_GRAM);
		//for(unsigned int i = 0; i < 10; i++)
		//{
		//	*(pre_pix++) = 0x0000;
		//	*(curr_pix++) = 0xFFFF;
		//	if ((unsigned short)(pre_pix) > ((unsigned short)((uint16_t *)(SDRAM_BASE))+ILI9341_LCD_PIXEL_WIDTH * ILI9341_LCD_PIXEL_HEIGHT/2))
		//	{
		//		pre_pix = (uint16_t *)(SDRAM_BASE+i*4);
		//	}
		//	if ((unsigned short)(curr_pix) > ((unsigned short)((uint16_t *)(SDRAM_BASE))+ILI9341_LCD_PIXEL_WIDTH * ILI9341_LCD_PIXEL_HEIGHT/2))
		//	{
		//		curr_pix = (uint16_t *)(SDRAM_BASE+i*4);
		//	} 
		//}

		osDelay(200); 
	}
}

void MAINTimer_Callback(void const *pvParametrs)
{
	
}
