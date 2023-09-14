#ifndef __CONTROL_H
#define __CONTROL_H
#include "main.h"
 
#include "ano.h"
#include "my_uart.h"

#include "PID.h"
#include "motor.h"
#include "act_motor.h"
#include "logic.h"

#include "StepMotor.h"
#include "Servo.h"

 

extern int task  ; //总任务表示序号
extern int do_cnt  ;//单个任务下 动作标识序号
extern unsigned char B_updataFlag;
#define ReturnToCarRun    do{ SendCmdA;  HAL_Delay(15); ClearRxData; do_cnt=finish_Arm;    }while(0) //从机械臂动作回到巡线模式

void Other_Actions();


/*PID调控参数 与 调控函数   

debug PID 用下面参数

系统统一转换为脉冲数进行处理，脉冲数分辨率高，控制精度高 */
extern int rpm;   //速度   5~15~35~45左右完全可以
extern float num; //编码数  9000


#endif








// extern int Target_Velocity,Reality_Velocity;   /* 目标速度，实际速度 */
// extern int Target_Position,Reality_Position;   /* 目标位置，实际位置 */
/*---------------------------------------------------------------*/
/* 算法参考原型*/
/* PID算法参考原型*/
//int Position_PID(int reality,int target);			//位置式 参考原型
//int Incremental_PID(int reality,int target); 	//增量式 参考原型  

// /**************************************************************************
// 函数功能：位置式PID控制器
// 入口参数：实际位置，目标位置
// 返回  值：电机PWM
// 根据位置式离散PID公式 
// 位置环out=    Kp*e(k)+          Ki*∑e(k)+   Kd[e(k)-e(k-1)]
// e(k)代表本次偏差 
// e(k-1)代表上一次的偏差  
// ∑e(k)代表e(k)以及之前的偏差的累积和;其中k为1,2,...,k;
// pwm代表输出
// **************************************************************************/
// int Position_PID(int reality,int target)
// { 	
//     static float Bias,Pwm,Last_Bias,Integral_bias=0;
    
//     Bias=target-reality;                            /* 计算偏差 */
//     Integral_bias+=Bias;	                        	/* 偏差累积 */
    
//     if(Integral_bias> 5000) Integral_bias = 5000;   /* 积分限幅 */
//     if(Integral_bias<-5000) Integral_bias =-5000;
    
//     Pwm = (Position_KP*Bias)                        /* 比例环节 */
//          +(Position_KI*Integral_bias)               /* 积分环节 */
//          +(Position_KD*(Bias-Last_Bias));           /* 微分环节 */
    
//     Last_Bias=Bias;                                 /* 保存上次偏差 */
//     return Pwm;                                     /* 输出结果 */
// }

// /**************************************************************************
// 函数功能：增量PID控制器
// 入口参数：实际值，目标值
// 返回  值：电机PWM
// 根据增量式离散PID公式 
// 速度环out=    Kp[e(k)-e(k-1)]+  Ki*e(k) +   Kd[e(k)-2e(k-1)+e(k-2)]
// e(k)代表本次偏差 
// e(k-1)代表上一次的偏差  以此类推 
// pwm代表增量输出
// **************************************************************************/
// int Incremental_PID(int reality,int target)
// { 	
// 	 static float Bias,Pwm,Last_bias=0,Prev_bias=0;
    
// 	 Bias=target-reality;                                   /* 计算偏差 */
    
// 	 Pwm += (Incremental_KP*(Bias-Last_bias))               	/* 比例环节 */
//            +(Incremental_KI*Bias)                           /* 积分环节 */
//            +(Incremental_KD*(Bias-2*Last_bias+Prev_bias));  /* 微分环节 */ 
    
// 	 Prev_bias=Last_bias;       /* 保存上上次偏差 */
// 	 Last_bias=Bias;	        /* 保存上一次偏差 */
    
// 	 return Pwm; 		/* 输出结果 */
// }



