#include "StepMotor.h"
/*
注意的点：
1.Motor1是转圈的电机，Motor2是大臂电机，Motor3是小臂电机
2.就当前机械爪来看，小臂是初始状态是有24°的别忘了
*/
#define  Abs(x)   ((x) < 0 ? -(x) : (x))  //求绝对值 
float Angle1,Angle2,Angle3;
Step_Motor Motor_1,Motor_2,Motor_3;


/*===================================================*/
/*===============       算法函数     =================*/
/*===================================================*/

float Calculate_DisHorizon()//计算水平距离
{
    return (     L1*sin((Motor_2.Pulse)*Bu*Factor) \
                +L2*sin(((Motor_3.Pulse)*Bu/+OriginalAngle3)*Factor)  ) ;
}
 
 /**
 * @brief 保持基座朝向， 水平平移距离， 只设置参数 不进行驱动
 * 
 * @param dis  水平平移距离 +前 -后
 */
void StepMotor_HorizonMove(float dis)
{
    float S =Calculate_DisHorizon();
    float ss=sqrt((dis+S)*(dis+S)+H*H);
    float n=acos((L1*L1+L2*L2-ss*ss)/(2*L1*L2))/(Factor);
    float m1 = acos(H / ss) / (Factor);
    float m2 = acos((L1 * L1 + ss * ss - L2 * L2) / (2 * ss * L1)) / (Factor);
    Angle1=Motor_1.Pulse*Bu;
    Angle2=180-m1-m2;
    Angle3=n-Angle2;
    StepMotor_Set_AnglePulse(  Angle1,  Angle2,  Angle3);
}



//垂直下落，效果不好
// void SetMotor_Vertical(uint8_t Dis_H,uint8_t Con_Num)
// {
//     if(Con_Num==1||Con_Num==2)
//     {
//         float S =Calculate_DisHorizon();
//         float HB=L2*cos(((Motor_3.Pulse)*Bu/TRS+OriginalAngle3)*Factor) -L1*cos(((Motor_2.Pulse)*Bu*Factor))+Dis_H;
//         float HP=sqrt(HB*HB+S*S);
//         float m=acos((L1*L1+HP*HP-L2*L2)/(2*L1*HP))/(Factor);
//         float n=acos((L2*L2+HP*HP-L1*L1)/(2*L2*HP))/(Factor);
//         Angle1=Motor_1.Pulse*Bu;
//         Angle2=180-m-asin(S/HP)/(Factor);
//         Angle3=90-n-asin(HB/HP)/(Factor);
//         StepMotor_Set_AnglePulse(Angle1,Angle2,Angle3);
//         StepMotor_Arm_Drive(1,450);
//         while (OVER==Flag_doing);
//     }

//     else if (Con_Num==3)
//     {
//         float S =Calculate_DisHorizon();
//         float HB=Dis_H-(L2*cos(((Motor_3.Pulse)*Bu/TRS+OriginalAngle3)*Factor) -L1*cos(((Motor_2.Pulse)*Bu*Factor)));
//         float HP=sqrt(HB*HB+S*S);
//         float m=acos((L1*L1+HP*HP-L2*L2)/(2*L1*HP))/(Factor);
//         float n=acos((L2*L2+HP*HP-L1*L1)/(2*L2*HP))/(Factor);
//         float z=acos((L2*L2+L1*L1-HP*HP)/(2*L2*L1))/(Factor);
//         Angle1=Motor_1.Pulse*Bu;
//         Angle2=90-m-asin(HB/HP)/(Factor);
//         Angle3=z-Angle2;
//         StepMotor_Set_AnglePulse(Angle1,Angle2,Angle3);
//         StepMotor_Arm_Drive(1,450);
//         while (OVER==Flag_doing);
//     }
//    





/*=======================================================*/
/*===============       底层驱动如下     =================*/
/*=======================================================*/

 void StepMotor_CallBack_TIM5(TIM_HandleTypeDef *htim)
{
// void  HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)  //原来
// void  HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim) 
// void  HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) 
    //一下代码段内容注释掉的内容千万别放出来！！！！！！\
    ！！！！！！！！！！！！！！！！！！！！！！！！！！\
    ！！！！！！！！！！！！！！！！！！！！！！！！！！\
    ！！！！！！！！！！！！！！！！！！！！！！！！！！
    if(htim->Instance == StepMotorTIM ) // 
	{
        // if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) //ccr1
        {
        // 处理PWM通道1的中断       
                StepMotor_UpdataPulse(ID_1);
        }
        // else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
        {
        // 处理PWM通道2的中断      
                StepMotor_UpdataPulse(ID_2);       
        }
        // else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
        { 
        // 处理PWM通道3的中断
                StepMotor_UpdataPulse(ID_3);
        }

//停止 到达目标
            if( (Motor_3.pwmPulse>=Motor_3.tarPulse)\
                &&(Motor_2.pwmPulse>=Motor_2.tarPulse)\
                &&(Motor_1.pwmPulse>=Motor_1.tarPulse))
            {
                StepMotor_Drive(StepALL_STOP, 0);
            }
	}
}

void StepMotor_UpdataPulse(int StepMotorNum)
{
    switch (StepMotorNum)
    {
        case ID_1:
            if (Motor_1.Flag == Flag_doing)
            {
                if (Motor_1.Dir == Dir_Z)
                {
                    ++Motor_1.Pulse;
                } // 绝对位置脉冲表示
                else if (Motor_1.Dir == Dir_N)
                {
                    --Motor_1.Pulse;
                }
                ++Motor_1.pwmPulse; // 定时器已经发出的脉冲值
                if (Motor_1.pwmPulse >= Motor_1.tarPulse)
                {
                    Motor_1.pwmPulse = 0;
                    Motor_1.tarPulse = 0;
                    Motor_1.Flag = Flag_finish;
                    MOTOR1_STOP;
                }
            }
            else{
                    Motor_1.pwmPulse = 0;
                    Motor_1.tarPulse = 0;
                    Motor_1.Flag = Flag_finish;
                    MOTOR1_STOP;
            }
        break;        

        case ID_2:
            if (Motor_2.Flag == Flag_doing)
                {
                    if (Motor_2.Dir == Dir_Z)
                    {
                        ++Motor_2.Pulse;
                    }
                    else if (Motor_2.Dir == Dir_N)
                    {
                        --Motor_2.Pulse;
                    }

                    ++Motor_2.pwmPulse;
                    if (Motor_2.pwmPulse >= Motor_2.tarPulse)
                    {
                        Motor_2.pwmPulse = 0;
                        Motor_2.tarPulse = 0;
                        Motor_2.Flag = Flag_finish;
                        MOTOR2_STOP;
                    }
                }
                else{
                        Motor_2.pwmPulse = 0;
                        Motor_2.tarPulse = 0;
                        Motor_2.Flag = Flag_finish;
                        MOTOR2_STOP;
                }
        break;

        case ID_3:
            if (Motor_3.Flag == Flag_doing)
            {   
                if (Motor_3.Dir == Dir_Z)
                {
                    ++Motor_3.Pulse;
                }
                else if (Motor_3.Dir == Dir_N)
                {
                    --Motor_3.Pulse;
                }

                ++Motor_3.pwmPulse;
                if (Motor_3.pwmPulse >= Motor_3.tarPulse)
                {
                    Motor_3.pwmPulse = 0;
                    Motor_3.tarPulse = 0;
                    Motor_3.Flag = Flag_finish;
                    MOTOR3_STOP;
                }
            }
            else{
                    Motor_3.pwmPulse = 0;
                    Motor_3.tarPulse = 0;
                    Motor_3.Flag = Flag_finish;
                    MOTOR3_STOP;
            }
        break;

        default:
            break;
    }
}

/**
 * @brief 
 * 
 * @param Con  增加了单个电机停止的控制 StepALL_STOP| StepALL_START | Step1_STOP|  Step2_STOP|  Step3_STOP
 * @param speedPeriod 速度快慢周期 270 - 600 - 1000合适
 */
void StepMotor_Drive(int Con, int speedPeriod)  
{
    
    if(Con == StepALL_START)
    {
        MOTOR123_ALLSTART;
        LED0_ON;
        Motor_3.pwmPulse=0; Motor_1.Flag= Motor_1.tarPulse==0?Flag_finish: Flag_doing;
        Motor_1.pwmPulse=0; Motor_2.Flag= Motor_2.tarPulse==0?Flag_finish: Flag_doing;
        Motor_2.pwmPulse=0; Motor_3.Flag= Motor_3.tarPulse==0?Flag_finish: Flag_doing;
        StepMotor_Set_Speed(speedPeriod);
    }
    else if(Con == StepALL_STOP){
        
        MOTOR123_ALLSTOP;
        LED0_OFF;

        Motor_1.pwmPulse=0; Motor_1.Flag=Flag_finish; Motor_1.tarPulse=0;
        Motor_2.pwmPulse=0; Motor_2.Flag=Flag_finish; Motor_2.tarPulse=0;
        Motor_3.pwmPulse=0; Motor_3.Flag=Flag_finish; Motor_3.tarPulse=0;
    } 
    else if(Con == Step1_STOP)
    {
        Motor_1.pwmPulse=0; Motor_1.Flag=Flag_finish; Motor_1.tarPulse=0;
        MOTOR1_STOP;
        StepMotor_Set_Speed(speedPeriod);
    } 
    else if (Con == Step2_STOP)
    {
        Motor_2.pwmPulse=0; Motor_2.Flag=Flag_finish; Motor_2.tarPulse=0;
        MOTOR2_STOP;
        StepMotor_Set_Speed(speedPeriod);
    }
    else if (Con == Step3_STOP)
    {
        Motor_3.pwmPulse=0; Motor_3.Flag=Flag_finish; Motor_3.tarPulse=0;
        MOTOR3_STOP;
        StepMotor_Set_Speed(speedPeriod);
    }
}



void StepMotor_Set_TarPulse(int tar1, int tar2,int tar3) //值设定目标值
{
    Motor_1.tarPulse = Abs(tar1);   
    Motor_2.tarPulse = Abs(tar2);   
    Motor_3.tarPulse = Abs(tar3);   

        if (tar1 >= 0)
        {
            Motor_1.Dir =Dir_Z;  MOTOR1_DIR_CW;
        }
        else if (tar1 < 0)
        {
            Motor_1.Dir =Dir_N;  MOTOR1_DIR_CCW;
        }    

        if (tar2 >= 0)
        {
            Motor_2.Dir =Dir_Z;  MOTOR2_DIR_CW;
        }
        else if (tar2 < 0)
        {
            Motor_2.Dir =Dir_N;  MOTOR2_DIR_CCW;
        }  

        if (tar3 >= 0)
        {
            Motor_3.Dir =Dir_Z;  MOTOR3_DIR_CW;
        }
        else if (tar3 < 0)
        {
            Motor_3.Dir =Dir_N;  MOTOR3_DIR_CCW;
        }  
}


void StepMotor_Set_AbsPulse(int tar1, int tar2,int tar3)
{
//假设
/*
现在 4000位置 pulse
要到达 5000位置 tar1

+1000

现在 5000 pulse
要到达 0   tar1

-5000

*/
    tar1 = tar1-Motor_1.Pulse;
    tar2 = tar2-Motor_2.Pulse;
    tar3 = tar3-Motor_3.Pulse;
    StepMotor_Set_TarPulse(tar1,tar2,tar3);

}
void StepMotor_Set_AnglePulse(float Angle1_,float Angle2_,float Angle3_)
{   
// 16细分  0.18/16        360*16/0.18
/*
当前位置 Pulse         50000
目标位置 angle转pulse  60000

要走   +10000 

*/
    // Motor_1.tarPulse=(Angle1_-Motor_1.Pulse*Bu)  /Bu;
    // Motor_2.tarPulse=(Angle2_-Motor_2.Pulse*Bu)  /Bu;
    // Motor_3.tarPulse=(Angle3_-(Motor_3.Pulse*Bu+OriginalAngle3))  /Bu;

    StepMotor_Set_TarPulse( (Angle1_-Motor_1.Pulse*Bu)  /Bu ,\
                            (Angle2_-Motor_2.Pulse*Bu)  /Bu, \
                            (Angle3_-(Motor_3.Pulse*Bu+OriginalAngle3))  /Bu );
} 
 



void StepMotor_Init(void)
{

    //失能motor 
    MOTOR123_ALLSTOP;
    StepMotor_Set_Speed(StepSpeed); // stepspeed  900   600     450  400   300 is suitable //默认速度 StepSpeed =600;

    // MOTOR1_DIR_CCW;
    // MOTOR2_DIR_CCW;
    // MOTOR3_DIR_CW;

    Motor_1.pwmPulse=0;   Motor_1.ID=ID_1,Motor_1.Pulse=0,Motor_1.Flag=Flag_finish,   Motor_1.tarPulse=0;
    Motor_2.pwmPulse=0;   Motor_2.ID=ID_2,Motor_2.Pulse=0,Motor_2.Flag=Flag_finish,   Motor_2.tarPulse=0;
    Motor_3.pwmPulse=0;   Motor_3.ID=ID_3,Motor_3.Pulse=0,Motor_3.Flag=Flag_finish,   Motor_3.tarPulse=0;

    HAL_TIM_Base_Start_IT(&htim5);
}

void StepMotor_Set_Speed(unsigned int speed)
{   
    __HAL_TIM_SET_AUTORELOAD(&StepMotorTIMHandle, speed-1); //设置计数周期
    __HAL_TIM_SET_COMPARE(&StepMotorTIMHandle, TIM_CHANNEL_1, speed / 2 - 15);
    __HAL_TIM_SET_COMPARE(&StepMotorTIMHandle, TIM_CHANNEL_2, speed / 2 + 15);
    __HAL_TIM_SET_COMPARE(&StepMotorTIMHandle, TIM_CHANNEL_3, speed / 2);
}

int StepMotor_Isover()
{
    if(Motor_1.Flag==Flag_finish &&Motor_2.Flag==Flag_finish &&Motor_3.Flag==Flag_finish )
        return Flag_finish;
    else
        return Flag_doing;
}



