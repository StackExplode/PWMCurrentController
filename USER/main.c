#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "arm_math.h"


#include "adc.h"
#include "tim1.h"
#include "ccontrol.h"
#include "command.h"
#include "config.h"

int main(void)
{
	
	uart_init(115200);
	delay_init(84);
	Tim1_Init();
	Tim1_SetPWM_ADC_PULSE(0.1);
	TIM1_PWM1_ON();
	TIM1_PWM4_ON();
	ADC_My_Init();
	Controller_Init();
	
	Aim_Current = DEFAULT_AIM;
	CC_Run = 1;
	ADC_My_StartDMA();

  while(1){
		
		if(USART_REC_State->RecFinished)
		{
			Command_Parse((const char*)USART_RX_BUF,USART_REC_State->DataLen);
			
			USART_RX_STA = 0;
		}
		
		if(DMA_Flag == 1)
		{
			Current_Control();
			DMA_Flag = 0;
#if DMA_WAIT_CONTROL==1
			ADC_My_StopDMA();
			ADC_My_StartDMA();
#endif
		}
	  
	}
}




