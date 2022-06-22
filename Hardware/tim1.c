#include "tim1.h"
#include "config.h"


static void _Tim1_GPIO_Init(void)	//PA8->TIM1_CH1 PA11->TIM1_CH4
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_TIM1);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11; //GPIOA8 11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA8 11
}


static void _Tim1_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //①时钟 TIM1 使能
	
	TIM_TimeBaseInitTypeDef tm;
	tm.TIM_Prescaler = PWM_PRESCALER - 1;         
	tm.TIM_CounterMode = TIM_CounterMode_Up;  
	tm.TIM_Period = PWM_COUNTER - 1;  
	tm.TIM_ClockDivision = 0; //不分频　	
	tm.TIM_RepetitionCounter=0;
	//tm.TIM_ClockDivision = TIM_CKD_DIV1;  
	TIM_TimeBaseInit(TIM1, &tm);  
	
	//TIM_DMAConfig(TIM1,Tim_dmabase_, TIM_DMABurstLength_1Transfer);
	TIM_DMACmd(TIM1, TIM_DMA_Update | TIM_DMA_Trigger | TIM_DMA_COM, ENABLE);
	TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update);
	
	
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //PWM1为正常占空比模式，PWM2为反极性模式
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低,有效电平为低电平
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;//在空闲时输出     低,这里的设置可以改变TIM_OCPolarity 如果没这句，第1通道有问题
	TIM_OCInitStructure.TIM_Pulse = 0; //输入通道1 CCR1（占空比数值）
	TIM_OC1Init(TIM1, &TIM_OCInitStructure); //Ch1初始化
	TIM_OC4Init(TIM1, &TIM_OCInitStructure); //Ch4初始化
	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR1上的预装载寄存器,CCR自动装载默认也是打开的
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPE使能 
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	TIM_Cmd(TIM1,ENABLE);

}


void Tim1_Init(void)
{
	_Tim1_GPIO_Init();
	_Tim1_Init();
}


void Tim1_SetPWM1(float percent)
{
	if(percent < PWM_MIN_PERCENT)
		percent = PWM_MIN_PERCENT;
	if(percent > PWM_MAX_PERCENT)
		percent = PWM_MAX_PERCENT;
	u32 pwm = (u16)(PWM_COUNTER * percent);
	TIM1->CCR1 = pwm;
}

void Tim1_SetPWM4(float percent)
{
	if(percent < PWM_MIN_PERCENT)
		percent = PWM_MIN_PERCENT;
	if(percent > PWM_MAX_PERCENT)
		percent = PWM_MAX_PERCENT;
	u32 pwm = (u32)(PWM_COUNTER * percent);
	TIM1->CCR4 = pwm;
}

void Tim1_SetPWM_ADC_PULSE(float percent)
{
	if(percent < PWM_MIN_PERCENT)
		percent = PWM_MIN_PERCENT;
	if(percent > PWM_MAX_PERCENT)
		percent = PWM_MAX_PERCENT;
	u32 pwm = (u32)(PWM_COUNTER * percent);
	
	TIM1->CCR4 = pwm;
	
	if(ADC_SAMPLE_POS > 0)
		pwm =  (u32)(pwm * ADC_SAMPLE_POS);
	else if(ADC_SAMPLE_POS < 0)
	{
		pwm += ADC_SAMPLE_POS;
		if(pwm <= 0)
			pwm -= ADC_SAMPLE_POS;
	}
	
	TIM1->CCR1 = pwm;

}
































