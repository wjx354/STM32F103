#include "motor.h"

/*
//TIM4 CH1 PWM 输出设置
//PWM 输出初始化
//arr：自动重装值
//psc：时钟预分频数
STM32F103C8T6
PB6 - TIM4_CH1
PB7 - TIM4_CH2
PB8 - TIM4_CH3
PB9 - TIM4_CH4

这时的TIMx时钟为72MHz，用这个TIMx时钟72MHz除以(PSC+1)，
得到定时器每隔多少秒涨一次，这里给PSC赋7199，计算得定时
器每隔0.0001秒涨一次，即此时频率为10KHz，再把这个值乘以
(ARR+1)得出PWM频率。

假如ARR值为0，即0.0001*(0+1)，则输出PWM频率为10KHz,再假如输出频率为100Hz的PWM，则将ARR寄存器设置为99即可
*/
//50Hz->20ms->arr=199,psc=7199
//PWM频率 = 72M / ((arr+1)*(psc+1))(单位：Hz)
//占空比 = TIM4->CCR1 / arr(单位：%)
//设置pwm频率为20HZ，则需50ms中断，arr=499，psc=7199
//如需占空比50%，频率为20hz的pwm
//需设置TIM4->CCR1=占空比*arr=250
//占空比25%----->TIM4->CCR1=125
//占空比50%----->TIM4->CCR1=250
//占空比75%----->TIM4->CCR1=375
void Tim4_PWM_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //使能 TIMx 外设
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能 PB 时钟
	TIM_DeInit(TIM4);
 
  //设置该引脚为复用输出功能,输出 TIM4 CH1 的 PWM 脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; //TIM4_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用功能输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化 GPIO
 
	TIM_TimeBaseStructure.TIM_Period = arr; //设置自动重装载周期值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置预分频值 不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //初始化 TIMx
	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //CH1 PWM2 模式
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	
	
	TIM_OCInitStructure.TIM_Pulse = 100; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //OC1 高电平有效
	TIM_OC1Init(TIM4, &TIM_OCInitStructure); //根据指定的参数初始化外设 TIMx
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); //CH1 预装载使能
	
	
	TIM_OCInitStructure.TIM_Pulse = 400; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //OC2 高电平有效
	TIM_OC2Init(TIM4, &TIM_OCInitStructure); //根据指定的参数初始化外设 TIMx
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); //CH2 预装载使能
	
	
	TIM_OCInitStructure.TIM_Pulse = 600; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //OC1 高电平有效
	TIM_OC3Init(TIM4, &TIM_OCInitStructure); //根据指定的参数初始化外设 TIMx
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); //CH3 预装载使能
	
	
	TIM_OCInitStructure.TIM_Pulse = 800; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //OC1 高电平有效
	TIM_OC4Init(TIM4, &TIM_OCInitStructure); //根据指定的参数初始化外设 TIMx
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable); //CH4 预装载使能
	
	
	TIM_ARRPreloadConfig(TIM4, ENABLE); //使能 TIMx 在 ARR 上的预装载寄存器
//	TIM_CtrlPWMOutputs(TIM4,ENABLE);  //MOE 主输出使能,高级定时器必须开启
	TIM_Cmd(TIM4, ENABLE); //使能 TIMx
}

//越大电机转速越低
void set_PWM(int width1,int width2,int width3,int width4){
				
   	TIM4->CCR1=7199-width1;
		TIM4->CCR2=7199-width2;
		TIM4->CCR3=7199-width3;
		TIM4->CCR4=7199-width4;
	
}

