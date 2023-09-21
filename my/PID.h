#ifndef _PID_H_
#define _PID_H_

#include "main.h"
#include "motor.h"


typedef struct
{
    float pre_target_val;           // espcially for location 
    float target_val;               // target:  set as a goal
    float actual_val;        		// actual:  input by updating data from hardware

    float err;             			
    float err_last;          		
    float err_pre;                  
    float integral;          		
    float out;                      
    float Kp,Ki,Kd;          		

    unsigned char param1 ;       // number of motor and its partner encoder 
    unsigned char param2 ;       // pid type : V speed;  L location
    unsigned char param3 ;       // status of action excute: 0 finished;  1 doing; 
    unsigned char param4 ;       // | the buffer time for the car to be stable after a action
    unsigned char param5 ;       // | espcially used for angle pid
}_pid;
extern _pid pid_speed, pid_speed2;    
extern _pid pid_location,pid_location2;
extern _pid pid_angle;

// 三环模式
#define PIDparam5_Local_Control     0
#define PIDparam5_Angle_Control     1
#define PIDparam5_Speed_Control     2


#define PIDparam1_M1 1   
#define PIDparam1_M2 2  
#define PIDparam1_M2_Left    2
#define PIDparam1_M1_Right   1

#define PIDparam1_An 3 
#define PIDparam2_I  1   //increase type
#define PIDparam2_L  2   //location type


#define PIDparam3_doing  1
#define PIDparam3_finish 0
#define PIDparam3_fail   2

#define PIDparam4_ready_low     35 // 20ms * 35 = 700ms | the buffer time for the car to be stable after a action
#define PIDparam4_ready_middle  20
#define PIDparam4_ready_quick   12 
#define PIDparam4_ready_master  12

 
/*---------------------------------*/
/*Top module: motor and mpu6050 driver*/


/*Top module1: update status and paramters*/
void PID_Calculate_Updata(_pid *pid_L, _pid *pid_V, _pid *pid_A);
unsigned char PID_JudgeStatus(); 


/*init funtion*/
void PID_param_init(void); // 系统初始化pid时 初始化参数用
void Top_PID_Control_CallBack_TIM6(TIM_HandleTypeDef *htim);




















/*math f or private f  私有函数 不需要外部调用*/
float location_pid_realize(_pid *pid );
float Increase_pid_realize(_pid *pid );
float Increase_pid_realize_angle(_pid *pid);
void  PID_Param_Updata(_pid *pid_L, _pid *pid_V);
float Limit_Amplitude(float Lout, float Lerr,float minV, float maxV);


/*useless*/
// void   PIDchangeStatus(unsigned char motorNum, char status);
// void   set_pid_target(_pid *pid, float temp_val);
// float  get_pid_target(_pid *pid);
// void   set_p_i_d(_pid *pid, float p, float i, float d);

#endif // _PID_H_

