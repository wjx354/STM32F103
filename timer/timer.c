#include "timer.h"
/*
static uint32_t start[RC_CH_NUM] = {0};
static uint32_t end[RC_CH_NUM] = {0};
uint32_t interval[RC_CH_NUM] = {0};
static uint32_t ch_cnt[RC_CH_NUM] = {0};
*/
/**************************************************************************
函数功能：定时中断初始化
入口参数：arr：自动重装值  psc：时钟预分频数 
返回  值：无
①定义结构体（定时器+中断优先级）
②开启定时器1的时钟
③设置中断时间=((预分频数+1)*(自动重装值+1)/72000000（秒）)
④配置模式，然后初始化结构体
⑤配置中断优先级
⑥使能定时器1
⑦在定时器1的中断函数中写入相关操作
//Timer1_Init(99,7199);           //=====10MS进一次中断服务函数
**************************************************************************/
void Timer1_Init(u16 arr,u16 psc)    
{  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	//使能定时器1时钟
	 
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
	
	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //抢占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	TIM_Cmd(TIM1, ENABLE); 
}

//arr:自动重装值 psc:时钟预分频数
//定时器溢出时间计算：Tout=((arr+1)*(psc+1))/Ft   us
//Ft = 定时器工作频率，单位Mhz
//TIM2在APB1上，为HCLK/2   36Mhz
/*
void Timer2_CAP_Init(u16 arr, u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_ICInitTypeDef TIM_ICInitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    //开启TIM2和GPIO时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
	
    //PA0初始化
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;            //下拉输入
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    //TIM2定时器初始化
    TIM_TimeBaseInitStruct.TIM_Period = arr;
    TIM_TimeBaseInitStruct.TIM_Prescaler = psc;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;        
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;        //向上计数
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
    
    //TIM2_CH1输入捕获初始化
    TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStruct.TIM_ICFilter = 0x00;                            //不滤波
    TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;        //上升沿捕获
    TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;                //输入器不分频
    TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;    //映射到IC1
    TIM_ICInit(TIM2, &TIM_ICInitStruct);
    
    TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStruct.TIM_ICFilter = 0x00;                            //不滤波
    TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;        //上升沿捕获
    TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;                //输入器不分频
    TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;    //映射到IC2
    TIM_ICInit(TIM2, &TIM_ICInitStruct);
    
    TIM_ICInitStruct.TIM_Channel = TIM_Channel_3;
    TIM_ICInitStruct.TIM_ICFilter = 0x00;                            //不滤波
    TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;        //上升沿捕获
    TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;                //输入器不分频
    TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;    //映射到IC3
    TIM_ICInit(TIM2, &TIM_ICInitStruct);
    
    TIM_ICInitStruct.TIM_Channel = TIM_Channel_4;
    TIM_ICInitStruct.TIM_ICFilter = 0x00;                            //不滤波
    TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;        //上升沿捕获
    TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;                //输入器不分频
    TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;    //映射到IC4
    TIM_ICInit(TIM2, &TIM_ICInitStruct);
    
    
    //中断分组初始化
    NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStruct);    
    
    TIM_ITConfig(TIM2, TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4, ENABLE);            //更新中断和CC1IE捕获中断
    
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
    if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET) {  			//捕获1发生捕获事件
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
