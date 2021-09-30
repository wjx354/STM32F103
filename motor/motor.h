#ifndef __MOTOR_H
#define __MOTOR_H
#include <sys.h>	 

#define PWMA   TIM4->CCR3
#define AIN2   PBout(4)
#define AIN1   PBout(5)
#define BIN1   PBout(6)
#define BIN2   PBout(7)
#define PWMB   TIM4->CCR4

void Tim4_PWM_Init(u16 arr,u16 psc);
extern void set_PWM(int width1,int width2,int width3,int width4);

#endif
