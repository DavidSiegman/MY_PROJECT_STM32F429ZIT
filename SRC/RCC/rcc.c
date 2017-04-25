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
		RCC->PLLCFGR |= (2 << RCC_PLLCFGR_PLLM_Pos); // 8 MHz / 2 = 4 MHz
	}
	else
	{
		RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC;
		RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM_Msk;
		RCC->PLLCFGR |= (4 << RCC_PLLCFGR_PLLM_Pos); // 16 MHz / 4 = 4 MHz
	}
	
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN_Msk;
	RCC->PLLCFGR |= (90 << RCC_PLLCFGR_PLLN_Pos);  // 4 MHz * 90 = 360 MHz
	
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLP_Msk;         // 360 MHz / 2 = 180 MHz
	
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLQ_Msk;
	RCC->PLLCFGR |= (8 << RCC_PLLCFGR_PLLQ_Pos);   // 360 MHz / 8 = 45 MHz
}
