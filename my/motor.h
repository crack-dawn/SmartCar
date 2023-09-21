#ifndef _MOTOR_H_
#define _MOTOR_H_
#include "motor.h"

// #include "encoder.h"
#include "tim.h"
#include "PID.h"


#define  Motor_REDUCE   49

/*轮子直径mm535~530  一周编码值 */
// #define Diameter  53  // 1mm 直径
// #define Perimeter (float)53*3.1415926  //周长


#if(Motor_REDUCE == 30)
    #define M1_Circle_4_13_30 1560   
    #define M1_radio	    30						   
    #define M1_ppr          13               
    
    #define M2_Circle_4_13_30 1560   
    #define M2_radio	        30						   
    #define M2_ppr              13               

    #define  Pwm_Amplitude                  3550             /* PWM满幅是3600 */
	#define  Rpm_Max       					135				 /* 最大转速 20ms内脉冲数 */
    #define  NumEncoder_circle              1560
    #define _1mm_to_Encoder_                9.334  /* 9.369~9.375 */
    
    #define Err_disance     _1mm_to_Encoder_*2  //最小误差编码值  8~16
    #define Err_speed       2
    #define Err_angle       1.85   
    #define runSpeed    	35  //程序控制 转速范围
    #define MinSpeed    	10
    #define MaxSpeed    	45
#else if(Motor_REDUCE == 49) 
    #define M1_Circle_4_13_30 2475   
    #define M1_radio	    49						   
    #define M1_ppr          13               
    
    #define M2_Circle_4_13_30 2475   
    #define M2_radio	        49						   
    #define M2_ppr              13               


    #define  Pwm_Amplitude                   3550             /* PWM满幅是3600 */
	#define  Rpm_Max       					 80				 /* 最大转速 20ms内脉冲数 */
    #define  NumEncoder_circle               2475
    #define  _1mm_to_Encoder_                14.8086  /* 9.369~9.375 */

    #define Err_disance     (_1mm_to_Encoder_+10)  //最小误差编码值   
    #define Err_speed       2
    #define Err_angle       1.85   
    
    #define CorrectSpeed    55
    #define runSpeed    	CorrectSpeed  //程序控制 转速范围
    #define LowSpeed        30
    #define MinSpeed    	8  
    #define MaxSpeed    	80
#endif 

#define ETD(encoder)  ((float)encoder/(float)_1mm_to_Encoder_)
#define DTE(distance) ((float)distance*(float)_1mm_to_Encoder_)




// pid_speed2.actual_val   =  +(short)TIM3 -> CNT;   	//TIM3   PC6  PC7  E1 右轮子|  PE9   pwm1 pwmA | AN1 D10 AN2 D11| pid_speed2  右边 序号1  M1
// pid_speed.actual_val  	=  -(short)TIM4 -> CNT;   	//TIM4   PD12 PD13 E2 左轮子|  PE11  pwm2 pwmB | BN1 B8  BN2 B9 | pid_speed	  左边 序号2  M2

#define PWM1(pwm1)	    	    do{ TIM1->CCR1=pwm1; TIM1->CCR3=pwm1; }while(0)	         
#define PWM2(pwm2)              do{ TIM1->CCR2=pwm2; TIM1->CCR4=pwm2; }while(0)			 

#define  AIN1(x)  do{x?HAL_GPIO_WritePin(GPIOD,AN1_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOD,AN1_Pin,GPIO_PIN_RESET);}while(0)        
#define  AIN2(x)  do{x?HAL_GPIO_WritePin(GPIOD,AN2_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOD,AN2_Pin,GPIO_PIN_RESET);}while(0)  

#define  BIN1(x)  do{x?HAL_GPIO_WritePin(GPIOD,BN1_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOD,BN1_Pin,GPIO_PIN_RESET);}while(0)               
#define  BIN2(x)  do{x?HAL_GPIO_WritePin(GPIOD,BN2_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOD,BN2_Pin,GPIO_PIN_RESET);}while(0)  



void Car_Drive_Init();

/**
 * @brief 
 * 
 * @param pwmA  右侧电机驱动
 * @param pwmB  左侧电机驱动
 */
void Set_Pwm(int pwmA, int pwmB );


float Limit(float data,float max);
// void Moto_Stop(void);

#endif // _MOTOR_H_

