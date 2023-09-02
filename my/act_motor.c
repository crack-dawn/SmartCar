#include "PID.h"
#include "act_motor.h"
#include "main.h"
#include "control.h"
#include "motor.h"
 

void MotorDrive(float tarV1,float tarL1,float tarV2,float tarL2, float tarAngle)
{
    pid_speed.target_val = tarV1;// record now target speed
    pid_speed.pre_target_val = tarV1;// backup now target speed
	
    pid_speed2.target_val =	tarV2;// record now target speed
    pid_speed2.pre_target_val =	tarV2;// backup now target speed
    
    pid_location.target_val = tarL1;		
    pid_location2.target_val = tarL2;  
    
    pid_angle.target_val = tarAngle;
	
    pid_location.param3 =  PIDparam3_doing;	 
    pid_location2.param3 = PIDparam3_doing;  
    pid_angle.param3 =     PIDparam3_doing;
}
 
void CAR_RUN_Local(Action_Mode action,float tarV, float tarL1,float tarL2,float angle)
{
    float tarV2=tarV;
    if(action != Accum_GO  )
    {
        pid_location.actual_val = 0 ; //clear location
        pid_location2.actual_val = 0 ; 
    }
    else if (action == Accum_GO)
    {
        tarL1 += pid_location.target_val;
        tarL2 += pid_location2.target_val;
    }
		
    // if (angle)//车子正前方为0  左为-  右为+90    35-7-0  28 5倍速度差
 
    
    pid_angle.param5 = PIDparam5_Local_Control;
    
    MotorDrive(tarV,tarL1,   tarV2,tarL2,   0);
}

void CAR_RUN_Speed(Action_Mode action,float tarV1,float tarV2, float tarL1,float tarL2)
{

    pid_angle.param5 = PIDparam5_Speed_Control;//pid控制模式 根据action模式转变

    MotorDrive(tarV1,tarL1,   tarV2,tarL2,   0);
}



void CAR_TURN_Left()
{
    CAR_RUN(Zero_GO,runSpeed/2, 8.2,				runSpeed,475,					 0);
}

/**
 * @brief 
 * 
 * @param action 
 * @param tarV1     左实际速度值 20ms内编码值个数  0~60
 * @param tarL1     实际距离值 mmm
 * @param tarV2     右实际速度值 20ms内编码值个数  0~60
 * @param tarL2     实际距离值 mmm
 * @param target_angle  目标角度
 */
void CAR_RUN(Action_Mode action,float tarV1, float tarL1, float tarV2, float tarL2, float target_angle)
{
 
        tarL1 = DTE(tarL1);
        tarL2 = DTE(tarL2);
    

    pid_location.param5 = 1;
    pid_location2.param5 = 1;
    pid_angle.param5 = PIDparam5_Local_Control;
    
    if(action == Zero_GO  )
    {
        pid_location.actual_val = 0 ; //clear current location,then go to next
        pid_location2.actual_val = 0 ; 
    }
    else if (action == Correct_GO)
    {   

        pid_location.actual_val = 0 ; //clear current accurate location
        pid_location2.actual_val = 0 ; 
        pid_angle.param5 = PIDparam5_Angle_Control;
    }
    else if (action == Abs_GO)
    {
        
    }
    else if (action == Wait)  //wait in  now location
    {
        tarL1 = pid_location.actual_val;
        tarL2 = pid_location2.actual_val;
    }
    else if (action == Accum_GO)
    {
        tarL1 += pid_location.target_val;
        tarL2 += pid_location2.target_val;
    }
    else if(action == speed_debug)
    {
        pid_angle.param5 = PIDparam5_Speed_Control;
    }
    MotorDrive(tarV1,tarL1,   tarV2,tarL2,   0);
}














 


 


