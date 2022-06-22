#include "command.h"
#include "config.h"
#include "ccontrol.h"
#include "adc.h"
#include "tim1.h"
#include "stdio.h"
#include "string.h"

static void _Print_Version(void)
{
	printf("Version=%s\r\n",VERSION);
}

static void _Set_PID(float p,float i,float d,u8 rst)
{
	Controller_SetPID(p,i,d,rst);
	printf("Pid parameter set finished!\r\n");
}

static void _SetPWM_Out(u8 out)
{
	if(out)
	{
		TIM1_PWM4_ON();
		printf("PWM output(aka Timer1->CC4) now ON!\r\n");
	}
	else
	{
		TIM1_PWM4_OFF();
		printf("PWM output(aka Timer1->CC4) now OFF!\r\n");
	}
		
}

static void _SetPWM(float val)
{
	Tim1_SetPWM_ADC_PULSE(val);
	printf("PWM value now set to %0.1f%%\r\n", val*100);
}

static void _Control_EN(u8 en)
{
	if(en)
	{
		CC_Run = 1;
		printf("Current control now Running!\r\n");
	}
	else
	{
		CC_Run = 0;
		printf("Current control now Stopped!\r\n");
	}
}

static void _Print_LastADC(void)
{
	u16 adc = ADC_BUFF[ADC_DMA_LEN - 1];
	float val = ADC_VREF * adc / 4096;
	printf("Last ADC=%f V, raw value=0x%04X(%d)\r\n",val,adc,adc);
}

static void _ReadPWM(void)
{
	u16 cc = TIM1->CCR4;
	float per = (float)cc/PWM_COUNTER;
	printf("PWM is now set to %0.1f%%, raw counter=%d\r\n",per*100,cc);
}

static void _Set_Aim(float aim, u8 rst)
{
	Aim_Current = aim;

	char* p = "\r\n";
	if(rst)
	{
		Controller_ResetPWM();
		p = ", and PID state buffer is Reseted!\r\n";
	}
	
	printf("Aim current is set to %0.2f%s",aim,p);
}

void Command_Parse(const char* buff,u16 len)
{
	if(!strncmp(buff,"ver",3))
	{
		_Print_Version();
	}
	else if(!strncmp(buff,"setpid",6))
	{
		float p,i,d;
		int r;
		sscanf(buff,"setpid %f %f %f %d",&p,&i,&d,&r);
		_Set_PID(p,i,d,(u8)r);
	}
	else if(!strncmp(buff,"pwmout",6))
	{
		int out;
		sscanf(buff,"pwmout %d", &out);
		_SetPWM_Out(out);
	}
	else if(!strncmp(buff,"setpwm",6))
	{
		float val;
		sscanf(buff,"setpwm %f", &val);
		_SetPWM(val);
	}
	else if(!strncmp(buff,"runctrl",7))
	{
		int en;
		sscanf(buff,"runctrl %d", &en);
		_Control_EN(en);
	}
	else if(!strncmp(buff,"lastadc",7))
	{
		_Print_LastADC();
	}
	else if(!strncmp(buff,"readpwm",7))
	{
		_ReadPWM();
	}
	else if(!strncmp(buff,"setaim",6))
	{
		float aim;
		int reset = 0;
		sscanf(buff,"setaim %f %d", &aim, &reset);
		_Set_Aim(aim,reset);
	}
	else
	{
		printf("Wrong command!\r\n");
	}
}

































