#include "rcc.h"

void RCC_CLOCK_INIT(void)
{
	unsigned int redy_wait_index;
	
	// HSE Enable, Switch SystemClock to HSE, HSI Disable ======
	// =========================================================
	RCC->CR |= RCC_CR_HSEON; 
	redy_wait_index = 0;
	while (!(RCC->CR & RCC_CR_HSERDY))
	{
		redy_wait_index++;
		if (redy_wait_index > 1000000)
		{
			break;
		}
	}
	if (RCC->CR & RCC_CR_HSERDY)
	{
		RCC->CFGR |= RCC_CFGR_SW_HSE;
		redy_wait_index = 0;
		while (!(RCC->CFGR & RCC_CFGR_SWS_HSE))
		{
			redy_wait_index++;
			if (redy_wait_index > 1000000)
			{
				break;
			}
		}
	}
	if (RCC->CFGR & RCC_CFGR_SWS_HSE)
	{
		RCC->CR &= ~RCC_CR_HSION;
		redy_wait_index = 0;
		while (RCC->CR & RCC_CR_HSIRDY)
		{
			redy_wait_index++;
			if (redy_wait_index > 1000000)
			{
				break;
			}
		}
	}
	// PLL Initialisation =======================================
	// ==========================================================
	if (RCC->CFGR & RCC_CFGR_SWS_HSE)
	{
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
	
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLQ_Msk;
	RCC->PLLCFGR |= (8 << RCC_PLLCFGR_PLLQ_Pos);            // 360 MHz / 8 = 45 MHz
	
	RCC->CFGR &= ~RCC_CFGR_HPRE_Msk;							          // 180 MHz / 1 = 180 MHz === HCLK
	
	RCC->CFGR &= ~RCC_CFGR_PPRE1_Msk;
	RCC->CFGR |= (0x5 << RCC_CFGR_PPRE1_Pos);               // 180 / 4 = 45 MHz      === PCLK1
	
	RCC->CFGR &= ~RCC_CFGR_PPRE2_Msk;
	RCC->CFGR |= (0x4 << RCC_CFGR_PPRE2_Pos);               // 180 / 2 = 90 MHz      === PCLK1
	
	RCC->PLLSAICFGR &= ~ RCC_PLLSAICFGR_PLLSAIN_Msk;
	RCC->PLLSAICFGR |= (129 << RCC_PLLSAICFGR_PLLSAIN_Pos); // 1 MHz * 129 = 129 MHz
	
	RCC->PLLSAICFGR &= ~ RCC_PLLSAICFGR_PLLSAIQ_Msk;
	RCC->PLLSAICFGR |= (2 << RCC_PLLSAICFGR_PLLSAIQ_Pos);   // 129 MHz / 2 = 64,5 MHz === PLLSAICLK
	
	RCC->PLLSAICFGR &= ~ RCC_PLLSAICFGR_PLLSAIR_Msk;
	RCC->PLLSAICFGR |= (5 << RCC_PLLSAICFGR_PLLSAIR_Pos);   // 256 MHz / 5 = 25,8 MHz === PLLLCDCLK
	
	RCC->DCKCFGR &= ~ RCC_DCKCFGR_PLLSAIDIVR_Msk;           
	RCC->DCKCFGR |= (2 << RCC_DCKCFGR_PLLSAIDIVR_Pos);			// 25,8 MHz / 4 = 6.45 MHz === LCD_CLK
}
