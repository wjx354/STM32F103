#include "motor.h"

/*
//TIM4 CH1 PWM �������
//PWM �����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
STM32F103C8T6
PB6 - TIM4_CH1
PB7 - TIM4_CH2
PB8 - TIM4_CH3
PB9 - TIM4_CH4

��ʱ��TIMxʱ��Ϊ72MHz�������TIMxʱ��72MHz����(PSC+1)��
�õ���ʱ��ÿ����������һ�Σ������PSC��7199������ö�ʱ
��ÿ��0.0001����һ�Σ�����ʱƵ��Ϊ10KHz���ٰ����ֵ����
(ARR+1)�ó�PWMƵ�ʡ�

����ARRֵΪ0����0.0001*(0+1)�������PWMƵ��Ϊ10KHz,�ټ������Ƶ��Ϊ100Hz��PWM����ARR�Ĵ�������Ϊ99����
*/
//50Hz->20ms->arr=199,psc=7199
//PWMƵ�� = 72M / ((arr+1)*(psc+1))(��λ��Hz)
//ռ�ձ� = TIM4->CCR1 / arr(��λ��%)
//����pwmƵ��Ϊ20HZ������50ms�жϣ�arr=499��psc=7199
//����ռ�ձ�50%��Ƶ��Ϊ20hz��pwm
//������TIM4->CCR1=ռ�ձ�*arr=250
//ռ�ձ�25%----->TIM4->CCR1=125
//ռ�ձ�50%----->TIM4->CCR1=250
//ռ�ձ�75%----->TIM4->CCR1=375
void Tim4_PWM_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʹ�� TIMx ����
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //ʹ�� PB ʱ��
	TIM_DeInit(TIM4);
 
  //���ø�����Ϊ�����������,��� TIM4 CH1 �� PWM ���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; //TIM4_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //���ù������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ�� GPIO
 
	TIM_TimeBaseStructure.TIM_Period = arr; //�����Զ���װ������ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����Ԥ��Ƶֵ ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //��ʼ�� TIMx
	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //CH1 PWM2 ģʽ
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	
	
	TIM_OCInitStructure.TIM_Pulse = 100; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //OC1 �ߵ�ƽ��Ч
	TIM_OC1Init(TIM4, &TIM_OCInitStructure); //����ָ���Ĳ�����ʼ������ TIMx
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); //CH1 Ԥװ��ʹ��
	
	
	TIM_OCInitStructure.TIM_Pulse = 400; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //OC2 �ߵ�ƽ��Ч
	TIM_OC2Init(TIM4, &TIM_OCInitStructure); //����ָ���Ĳ�����ʼ������ TIMx
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); //CH2 Ԥװ��ʹ��
	
	
	TIM_OCInitStructure.TIM_Pulse = 600; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //OC1 �ߵ�ƽ��Ч
	TIM_OC3Init(TIM4, &TIM_OCInitStructure); //����ָ���Ĳ�����ʼ������ TIMx
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); //CH3 Ԥװ��ʹ��
	
	
	TIM_OCInitStructure.TIM_Pulse = 800; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //OC1 �ߵ�ƽ��Ч
	TIM_OC4Init(TIM4, &TIM_OCInitStructure); //����ָ���Ĳ�����ʼ������ TIMx
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable); //CH4 Ԥװ��ʹ��
	
	
	TIM_ARRPreloadConfig(TIM4, ENABLE); //ʹ�� TIMx �� ARR �ϵ�Ԥװ�ؼĴ���
//	TIM_CtrlPWMOutputs(TIM4,ENABLE);  //MOE �����ʹ��,�߼���ʱ�����뿪��
	TIM_Cmd(TIM4, ENABLE); //ʹ�� TIMx
}

//Խ����ת��Խ��
void set_PWM(int width1,int width2,int width3,int width4){
				
   	TIM4->CCR1=7199-width1;
		TIM4->CCR2=7199-width2;
		TIM4->CCR3=7199-width3;
		TIM4->CCR4=7199-width4;
	
}

