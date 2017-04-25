#include "main.h"
#include "rcc.h"
#include "cmsis_os.h"

void MAINTimer_Callback(void const *pvParametrs);

int main(void)
{
	// INITIALISATION ========================================================
	// =======================================================================
	RCC_CLOCK_INIT();
	
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
