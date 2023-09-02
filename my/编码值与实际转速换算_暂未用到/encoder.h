#ifndef __ENCODER_H
#define __ENCODER_H

#include "main.h"


#define Encoder_1_TIM   TIM3
#define Encoder_2_TIM   TIM4 
	 			     

 
float Moto_Speed(int encoder_cnt,uint16_t ppr,uint16_t ratio,uint16_t cnt_time);
long Num_Encoder_Cnt(float num,uint16_t ppr,float ratio); 
long Rpm_Encoder_Cnt(float rpm,uint16_t ppr,uint16_t ratio,uint16_t cnt_time); 

#endif




