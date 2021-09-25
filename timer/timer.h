#ifndef __TIMER_H
#define __TIMER_H
#include <sys.h>	


/*
*	Timer2:
*    	CH1 ~ PA0
*    	CH2 ~ PA1
*    	CH3 ~ PA2
*    	CH4 ~ PA3
*/



void Timer1_Init(u16 arr,u16 psc);
void Timer2_CAP_Init(u16 arr, u16 psc);
void Timer2_IRQHandler(void);
#endif
