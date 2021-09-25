#include "timer.h"
/*
static uint32_t start[RC_CH_NUM] = {0};
static uint32_t end[RC_CH_NUM] = {0};
uint32_t interval[RC_CH_NUM] = {0};
static uint32_t ch_cnt[RC_CH_NUM] = {0};
*/
/**************************************************************************
�������ܣ���ʱ�жϳ�ʼ��
��ڲ�����arr���Զ���װֵ  psc��ʱ��Ԥ��Ƶ�� 
����  ֵ����
�ٶ���ṹ�壨��ʱ��+�ж����ȼ���
�ڿ�����ʱ��1��ʱ��
�������ж�ʱ��=((Ԥ��Ƶ��+1)*(�Զ���װֵ+1)/72000000���룩)
������ģʽ��Ȼ���ʼ���ṹ��
�������ж����ȼ�
��ʹ�ܶ�ʱ��1
���ڶ�ʱ��1���жϺ�����д����ز���
//Timer1_Init(99,7199);           //=====10MS��һ���жϷ�����
**************************************************************************/
void Timer1_Init(u16 arr,u16 psc)    
{  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	//ʹ�ܶ�ʱ��1ʱ��
	 
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
	
	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	TIM_Cmd(TIM1, ENABLE); 
}

//arr:�Զ���װֵ psc:ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㣺Tout=((arr+1)*(psc+1))/Ft   us
//Ft = ��ʱ������Ƶ�ʣ���λMhz
//TIM2��APB1�ϣ�ΪHCLK/2   36Mhz
/*
void Timer2_CAP_Init(u16 arr, u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_ICInitTypeDef TIM_ICInitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    //����TIM2��GPIOʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
	
    //PA0��ʼ��
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;            //��������
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    //TIM2��ʱ����ʼ��
    TIM_TimeBaseInitStruct.TIM_Period = arr;
    TIM_TimeBaseInitStruct.TIM_Prescaler = psc;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;        
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;        //���ϼ���
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
    
    //TIM2_CH1���벶���ʼ��
    TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStruct.TIM_ICFilter = 0x00;                            //���˲�
    TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;        //�����ز���
    TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;                //����������Ƶ
    TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;    //ӳ�䵽IC1
    TIM_ICInit(TIM2, &TIM_ICInitStruct);
    
    TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStruct.TIM_ICFilter = 0x00;                            //���˲�
    TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;        //�����ز���
    TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;                //����������Ƶ
    TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;    //ӳ�䵽IC2
    TIM_ICInit(TIM2, &TIM_ICInitStruct);
    
    TIM_ICInitStruct.TIM_Channel = TIM_Channel_3;
    TIM_ICInitStruct.TIM_ICFilter = 0x00;                            //���˲�
    TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;        //�����ز���
    TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;                //����������Ƶ
    TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;    //ӳ�䵽IC3
    TIM_ICInit(TIM2, &TIM_ICInitStruct);
    
    TIM_ICInitStruct.TIM_Channel = TIM_Channel_4;
    TIM_ICInitStruct.TIM_ICFilter = 0x00;                            //���˲�
    TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;        //�����ز���
    TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;                //����������Ƶ
    TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;    //ӳ�䵽IC4
    TIM_ICInit(TIM2, &TIM_ICInitStruct);
    
    
    //�жϷ����ʼ��
    NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStruct);    
    
    TIM_ITConfig(TIM2, TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4, ENABLE);            //�����жϺ�CC1IE�����ж�
    
    TIM_Cmd(TIM2, ENABLE);
    printf("Init TIM2 CAP success\n");
}

void Timer2_IRQHandler(void)
{
    static uint32_t cnt = 0; 
    
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        cnt++;
        return;
    }
    // CH1 PA0 PPM_CH1 
    if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET) {  			//����1���������¼�
        // Clear TIM2_CH1 Capture compare interrupt pending bit  
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);  
        
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == SET) {
            start[RC_CH1] = TIM_GetCapture1(TIM2);
            GPIO_SetBits(GPIOB, GPIO_Pin_0);
            
            TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling);
            
            ch_cnt[RC_CH1] = cnt;
            return;
        } else {
            end[RC_CH1] = TIM_GetCapture1(TIM2);
                   
            interval[RC_CH1] = (cnt - ch_cnt[RC_CH1]) * TIM_COUNTER + end[RC_CH1] - start[RC_CH1];

            // limit the scale to 2sec.
            if (interval[RC_CH1] > 72000)
                interval[RC_CH1] = 72000;
            
            GPIO_ResetBits(GPIOB, GPIO_Pin_0);
            TIM_OC1PolarityConfig(TIM2, TIM_ICPolarity_Rising); 
            return;
        }
    }
    
    // CH2 PA1 PPM_CH2
    if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET) {  
        // Clear TIM2_CH2 Capture compare interrupt pending bit  
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);  
        
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == SET) {
            start[RC_CH2] = TIM_GetCapture2(TIM2);
            
            TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Falling);
            ch_cnt[RC_CH2] = cnt;
            return;
        } else {
            end[RC_CH2] = TIM_GetCapture2(TIM2);
                   
            interval[RC_CH2] = (cnt - ch_cnt[RC_CH2]) * TIM_COUNTER + end[RC_CH2] - start[RC_CH2];
            // limit the scale within 2 secs.
            if (interval[RC_CH2] > 72000)
                interval[RC_CH2] = 72000;

            TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Rising); 
            return;
        }
    }  
    
    // CH3 PA2 PPM_CH3 
    if (TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET) {  
        // Clear TIM2_CH3 Capture compare interrupt pending bit   
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);  
        
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == SET) {
            start[RC_CH3] = TIM_GetCapture3(TIM2);
            
            TIM_OC3PolarityConfig(TIM2,TIM_ICPolarity_Falling);
            ch_cnt[RC_CH3] = cnt;
            return;
        } else {
            end[RC_CH3] = TIM_GetCapture3(TIM2);
                   
            interval[RC_CH3] = (cnt - ch_cnt[RC_CH3]) * TIM_COUNTER + end[RC_CH3] - start[RC_CH3];
            // limit the scale within 2 secs.
            if (interval[RC_CH3] > 72000)
                interval[RC_CH3] = 72000;

            TIM_OC3PolarityConfig(TIM2,TIM_ICPolarity_Rising); 
            return;
        }
    }  
    
    // CH4 PA3 PPM_CH4 
    if (TIM_GetITStatus(TIM2, TIM_IT_CC4) != RESET) {  
        // Clear TIM2_CH4 Capture compare interrupt pending bit  
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);  
        
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == SET) {
            start[RC_CH4] = TIM_GetCapture4(TIM2);
            
            TIM_OC4PolarityConfig(TIM2,TIM_ICPolarity_Falling);
            ch_cnt[RC_CH4] = cnt;
            return;
        } else {
            end[RC_CH4] = TIM_GetCapture4(TIM2);
                   
            interval[RC_CH4] = (cnt - ch_cnt[RC_CH4]) * TIM_COUNTER + end[RC_CH4] - start[RC_CH4];
            // limit the scale within 2 secs.
            if (interval[RC_CH4] > 72000)
                interval[RC_CH4] = 72000;

            TIM_OC4PolarityConfig(TIM2,TIM_ICPolarity_Rising); 
            return;
        }
    }
}
*/
