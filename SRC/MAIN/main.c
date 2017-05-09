#include "main.h"
#include "rcc.h"
#include "spi.h"
#include "lcd.h"
#include "sdram.h"
#include "fonts.h"
#include "text_draw_driver.h"
#include "cmsis_os.h"
		
void MAINTimer_Callback(void const *pvParametrs);

int main(void)
{
	// INITIALISATION ========================================================
	// =======================================================================
	RCC_CLOCK_INIT();
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN |
                  RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOEEN | RCC_AHB1ENR_GPIOFEN |
                  RCC_AHB1ENR_GPIOGEN;
	SDRAM_Init();
	int line = 0, alfa = 255;
	for(unsigned int i = 0; i < ILI9341_LCD_PIXEL_WIDTH * ILI9341_LCD_PIXEL_HEIGHT;i++)
	{
		*((uint32_t *)(LAYER1_MEMORY_START_ADRESS)+i) = 0x00000000 | alfa << 24;
		*((uint32_t *)(LAYER2_MEMORY_START_ADRESS)+i) = 0x0F000000;
		
		if (i%240 == 0)
		{
			alfa = 0;
			line = i/240;
		}
		
		*((uint32_t *)(LAYER1_MEMORY_START_ADRESS)+i) = 0x00000000 | alfa << 24;
		alfa = alfa+1;
		
		if (i%10 == 0)
		{
			
			*((uint32_t *)(LAYER1_MEMORY_START_ADRESS)+i) |= 0xFFFFFF;
		}
		
		if ((i > 0)&&(i%240 == 0))
		{
			*((uint32_t *)(LAYER1_MEMORY_START_ADRESS)+i-1) |= 0xFFFFFF;
		}
		
		if ((line%10 == 0)||(line == 319))
		{
			*((uint32_t *)(LAYER1_MEMORY_START_ADRESS)+i) |= 0xFFFFFF;
		}
		
	}
	sFONT curr_font = Font12;
	char m = 10, n = 10;
	
	LCD_draw_text("Hallo World!", curr_font,2,n+curr_font.Width*0+240*m,0x00000000,0xFFFFFFFF);
		
	SPI5_INIT();
	LCD_INIT();
	
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
	//static unsigned int *curr_pix = (uint32_t *)(SDRAM_BASE+10);
	//static unsigned int *pre_pix = (uint32_t *)(SDRAM_BASE);
	for( ;; ) 
	{	
		/*
		for(unsigned int i = 0; i < 10; i++)
		{
			*(pre_pix++) = 0x00000000;
			*(curr_pix++) = 0xFFFFFFFF; 
		}
		if ((unsigned int)(pre_pix) > ((unsigned int)((uint32_t *)(SDRAM_BASE)+ILI9341_LCD_PIXEL_WIDTH * ILI9341_LCD_PIXEL_HEIGHT)))
		{
			pre_pix = (uint32_t *)(SDRAM_BASE);
		}
		if ((unsigned int)(curr_pix) > ((unsigned int)((uint32_t *)(SDRAM_BASE)+ILI9341_LCD_PIXEL_WIDTH * ILI9341_LCD_PIXEL_HEIGHT)))
		{
			curr_pix = (uint32_t *)(SDRAM_BASE);
		}
		*/
		osDelay(200); 
	}
}

void MAINTimer_Callback(void const *pvParametrs)
{
	
}
