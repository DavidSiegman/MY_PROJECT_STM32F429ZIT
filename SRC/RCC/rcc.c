#include "rcc.h"

void RCC_CLOCK_INIT(void)
{
	volatile static unsigned int redy_wait_index;
	
	RCC->CIR |= RCC_CIR_HSERDYIE | RCC_CIR_PLLRDYIE | RCC_CIR_PLLSAIRDYIE; 
	
	// HSE Enable, Switch SystemClock to HSE, HSI Disable ======
	// =========================================================
	RCC->CR |= RCC_CR_HSEON; 
	redy_wait_index = 0;
	while (!(RCC->CIR & RCC_CIR_HSERDYF))
	{
		redy_wait_index++;
		if (redy_wait_index > 1000000)
		{
			break;
		}
	}
	// PLL Initialisation =======================================
	// ==========================================================
	if (RCC->CR & RCC_CR_HSERDY)
	{
		RCC->CR |= RCC_CR_CSSON;
		RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC;
		RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM_Msk;
		RCC->PLLCFGR |= (8 << RCC_PLLCFGR_PLLM_Pos);          // 8 MHz / 8 = 1 MHz
	}
	else
	{
		RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC;
		RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM_Msk;
		RCC->PLLCFGR |= (16 << RCC_PLLCFGR_PLLM_Pos);          // 16 MHz / 16 = 1 MHz
	}

	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN_Msk;
	RCC->PLLCFGR |= (360 << RCC_PLLCFGR_PLLN_Pos);          // 1 MHz * 360 = 360 MHz
	
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLP_Msk;                  // 360 MHz / 2 = 180 MHz === SYSCLK
	
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLQ_Msk;
	RCC->PLLCFGR |= (8 << RCC_PLLCFGR_PLLQ_Pos);            // 360 MHz / 8 = 45 MHz
	
	RCC->CFGR &= ~RCC_CFGR_HPRE_Msk;							          // 180 MHz / 1 = 180 MHz === HCLK
	RCC->CFGR |= (0x9 << RCC_CFGR_HPRE_Pos);                // 180 MHz / 4 = 45 MHz  === HCLK
	
	RCC->CFGR &= ~RCC_CFGR_PPRE1_Msk;
	RCC->CFGR |= (0x0 << RCC_CFGR_PPRE1_Pos);               // 45 MHz / 1 = 45 MHz  === PCLK1
	
	RCC->CFGR &= ~RCC_CFGR_PPRE2_Msk;
	RCC->CFGR |= (0x0 << RCC_CFGR_PPRE2_Pos);               // 45 MHz / 1 = 45 MHz  === PCLK2
	
	/* Enable MAIN PLL */
	RCC->CR |= RCC_CR_PLLON;
	/* wait for MAIN PLL ready */
	redy_wait_index = 0;
	while ((RCC->CIR & RCC_CIR_PLLRDYF) == 0)
	{
		redy_wait_index++;
		if (redy_wait_index > 1000000)
		{
			break;
		}
	}
	for(unsigned int i = 0;i < 100000; i++){;}
	if ((RCC->CR & RCC_CR_PLLRDY))
	{
		RCC->CFGR |= RCC_CFGR_SW_PLL;
		//RCC->CFGR &= ~RCC_CFGR_SW_HSE;
		redy_wait_index = 0;
		while ((RCC->CFGR & RCC_CFGR_SWS_PLL) == 0)
		{
			redy_wait_index++;
			if (redy_wait_index > 1000000)
			{
				break;
			}
		}
	}
	
	RCC->PLLSAICFGR &= ~ RCC_PLLSAICFGR_PLLSAIN_Msk;
	RCC->PLLSAICFGR |= (180 << RCC_PLLSAICFGR_PLLSAIN_Pos); // 1 MHz * 180 = 180 MHz
	
	RCC->PLLSAICFGR &= ~ RCC_PLLSAICFGR_PLLSAIQ_Msk;
	RCC->PLLSAICFGR |= (2 << RCC_PLLSAICFGR_PLLSAIQ_Pos);   // 180 MHz / 2 = 90 MHz === PLLSAICLK
	
	RCC->PLLSAICFGR &= ~ RCC_PLLSAICFGR_PLLSAIR_Msk;
	RCC->PLLSAICFGR |= (5 << RCC_PLLSAICFGR_PLLSAIR_Pos);   // 180 MHz / 5 = 36 MHz === PLLLCDCLK
	
	RCC->DCKCFGR &= ~ RCC_DCKCFGR_PLLSAIDIVR_Msk;           
	RCC->DCKCFGR |= (2 << RCC_DCKCFGR_PLLSAIDIVR_Pos);			// 36 MHz / 4 = 9 MHz === LCD_CLK
	
	/* Enable SAI PLL */
	RCC->CR |= RCC_CR_PLLSAION;
	/* wait for SAI PLL ready */
	while((RCC->CR & RCC_CR_PLLSAIRDY) == 0){;}
	/*
	RCC->PLLI2SCFGR &= ~ RCC_PLLI2SCFGR_PLLI2SN_Msk;
	RCC->PLLI2SCFGR |= (360 << RCC_PLLI2SCFGR_PLLI2SN_Pos);
	
	RCC->PLLI2SCFGR &= ~ RCC_PLLI2SCFGR_PLLI2SR_Msk;
	RCC->PLLI2SCFGR |= (2 << RCC_PLLI2SCFGR_PLLI2SR_Pos);
	*/
}
