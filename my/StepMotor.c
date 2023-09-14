#include "StepMotor.h"
/*
注意的点：
1.Motor1是转圈的电机，Motor2是大臂电机，Motor3是小臂电机
2.就当前机械爪来看，小臂是初始状态是有24°的别忘了
*/
#define Abs(x) ((x) < 0 ? -(x) : (x)) // 求绝对值
float Angle1, Angle2, Angle3;
Step_Motor Motor_1, Motor_2, Motor_3,  Motor_4;//第四通道没有用到 驱动也没加上



/*===================================================*/
/*===============       算法函数     =================*/
/*===================================================*/


void Angle_vertical(float set_H,float set_S)           //垂直下落
{
    float HP = sqrt(set_H * set_H + set_S * set_S);
    float a = acos((L2 * L2 + L1 * L1 - HP * HP) / (2 * L1 * L2)) / (Factor);         //大臂和小臂的夹角
    Angle2 = a - (asin(set_S / HP) / (Factor)-acos((L2 * L2 + HP * HP - L1 * L1) / (2 * L2 * HP)) / (Factor));
    float two_Point = sqrt(L2_1 * L2_1 + L1 * L1 - (2 * cos((180 - a)*Factor) * L2_1 * L1));
    float b = acos((L1 * L1 + two_Point * two_Point - L2_1 * L2_1) / (2 * L1 * two_Point)) / (Factor);       //四边形靠近大臂的夹角
    float c = acos((L3 * L3 + two_Point * two_Point - L1_1 * L1_1) / (2 * L3 * two_Point))/(Factor);              //四边形靠近小滑轮的夹角
    Angle3 = 90 + Angle2 - b - c;

    StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,Angle2,Angle3);
    StepMotor_Drive(StepALL_START, 300);
     while (OVER == Flag_doing); 
}

float Calculate_DisHorizon()//计算水平距离
{
    float a=90-(Motor_3.Pulse*Bu+OriginalAngle3)+Motor_2.Pulse*Bu;
    float xie=sqrt(L1*L1+L3*L3-2*cos(a*Factor)*L1*L3);
    float a1=acos((L1*L1+xie*xie-L3*L3)/(2*L1*xie))/(Factor);
    float a2=acos((xie*xie+L2_1*L2_1-L1_1*L1_1)/(2*L2_1*xie))/(Factor);
    float b=180-a1-a2-Motor_2.Pulse*Bu;
    return (L1*sin(Motor_2.Pulse*Bu*Factor)+L2*sin(b*Factor));
}
/*=======================================================*/
/*===============       底层驱动如下     =================*/
/*=======================================================*/
void StepMotor_Set_AbsPulse(int tar1, int tar2, int tar3)
{
    // 假设
    /*
    现在 4000位置 pulse
    要到达 5000位置 tar1

    +1000

    现在 5000 pulse
    要到达 0   tar1

    -5000

    */
    tar1 = tar1 - Motor_1.Pulse;
    tar2 = tar2 - Motor_2.Pulse;
    tar3 = tar3 - Motor_3.Pulse;
    StepMotor_Set_TarPulses(tar1, tar2, tar3);
}
void StepMotor_Set_AnglePulse(float Angle1_, float Angle2_, float Angle3_)
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

    StepMotor_Set_TarPulses((Angle1_ - Motor_1.Pulse * Bu) / Bu,
                           (Angle2_ - Motor_2.Pulse * Bu) / Bu,
                           (Angle3_ - (Motor_3.Pulse * Bu + OriginalAngle3)) / Bu);
}

/*------------------------*/
void StepMotor_Drive(int Con, int speedPeriod)
{

    if (Con == StepALL_START){

         StepMotor_Set_Speeds(speedPeriod,speedPeriod,speedPeriod);
         if (Motor_1.tarPulse != 0)
         {
            Motor_1.start();
         }
         if (Motor_2.tarPulse != 0)
         {
           Motor_2.start();
         }
         if (Motor_3.tarPulse != 0)
         {
           Motor_3.start();
         }
    }
    else if (Con == StepALL_STOP){
         Motor_1.stop();
         Motor_2.stop();
         Motor_3.stop();
    }
    else if (Con == Step1_STOP){
         Motor_1.stop();
    }
    else if (Con == Step2_STOP){
          Motor_2.stop();
    }
    else if (Con == Step3_STOP){
          Motor_3.stop();
    }
}



/*------------------------*/
void StepMotor_Init(void)
{
    
    // 失能motor
    STEP1234_ALLSTOP; 
    StepPulseEN(DISABLE);//先失能

    Motor_1.pwmPulse = 0;
    Motor_1.Pulse = 0;
    Motor_1.tarPulse = 0;
    Motor_1.Flag = Flag_finish;
    Motor_1.ID = ID_1;

    Motor_1.ccw =   Step1_Dir_CCw;
    Motor_1.cw =    Step1_Dir_Cw;
    Motor_1.start = Step1_Start;
    Motor_1.stop =  Step1_Stop;

/**---------------------------------*/
    Motor_2.pwmPulse = 0;
    Motor_2.Pulse = 0;
    Motor_2.tarPulse = 0;
    Motor_2.Flag = Flag_finish;
    Motor_2.ID = ID_2;

    Motor_2.ccw =   Step2_Dir_CCw;
    Motor_2.cw =    Step2_Dir_Cw;
    Motor_2.start = Step2_Start;
    Motor_2.stop =  Step2_Stop;


/**---------------------------------*/
    Motor_3.pwmPulse = 0;
    Motor_3.Pulse = 0;
    Motor_3.tarPulse = 0;
    Motor_3.Flag = Flag_finish;
    Motor_3.ID = ID_3;
    
    Motor_3.ccw =   Step3_Dir_CCw;
    Motor_3.cw =    Step3_Dir_Cw;
    Motor_3.start = Step3_Start;
    Motor_3.stop =  Step3_Stop;

    StepPulseEN(ENABLE);//后使能 之后全程使能
    StepPulseEN(ENABLE);//后使能 之后全程使能
    STEP1234_ALLSTART;
    // HAL_TIM_Base_Start_IT(&StepMotorTIMHandle);
}
void StepMotor_UpdataPulse(Step_Motor* g_Motor)
{
    if (g_Motor->Flag == Flag_doing)
    {
        if (g_Motor->Dir == Dir_CW)         { ++(g_Motor->Pulse);   }
        else if (g_Motor->Dir == Dir_CCW)    { --(g_Motor->Pulse);   }

        ++(g_Motor->pwmPulse);

        if (g_Motor->pwmPulse  >=   Abs( g_Motor->tarPulse) ){
            g_Motor->stop();
        }            
    }
    else
    {
        g_Motor->stop();
    }
}

/*------------------------*/
void StepMotor_Set_Dir(Step_Motor*  g_Motor, int dir) 
{
    g_Motor->Dir =( dir>0? Dir_CW: Dir_CCW ) ;
    if (dir == Dir_CW || dir > 0)
    {
        g_Motor->cw();
    }
    else if (dir == Dir_CCW || dir < 0)
    {
        g_Motor->ccw();
    }
}
void StepMotor_Set_Dirs(int dir1, int dir2, int dir3)       // 设定方向
{
    ( dir1 >0 ) ? ( Motor_1.cw() ) : ( Motor_1.ccw() ) ;
    ( dir2 >0 ) ? ( Motor_2.cw() ) : ( Motor_2.ccw() ) ;
    ( dir3 >0 ) ? ( Motor_3.cw() ) : ( Motor_3.ccw() ) ;
}

/*------------------------*/
void StepMotor_Set_Speed(Step_Motor*  g_Motor, unsigned int period)
{
    g_Motor->period = period;
}
void StepMotor_Set_Speeds(unsigned int period1,unsigned int period2,unsigned int period3)
{
        Motor_1.period = period1;
        Motor_2.period = period2;
        Motor_3.period = period3;
}

/*------------------------*/
void StepMotor_Set_TarPulse(Step_Motor*  g_Motor, int tarPulse) // 值设定目标值
{
    g_Motor->tarPulse = Abs(tarPulse);
    StepMotor_Set_Dir(g_Motor, tarPulse);
}
void StepMotor_Set_TarPulses(int tar1, int tar2, int tar3) // 值设定目标值
{
    Motor_1.tarPulse = Abs(tar1);
    Motor_2.tarPulse = Abs(tar2);
    Motor_3.tarPulse = Abs(tar3);
    StepMotor_Set_Dirs(tar1,tar2,tar3);
}

/*------------------------*/
char StepMotor_Isover()
{
    if (Motor_1.Flag == Flag_finish && Motor_2.Flag == Flag_finish && Motor_3.Flag == Flag_finish)
        return Flag_finish;
    else
        return Flag_doing;
}




/*输出比较回调函数*/
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
#if( COMPARE_MODE ==  SIMPLE_MMODE )
    
    if (htim->Instance == StepMotorTIMHandle.Instance)
    {
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {
            if (HAL_GPIO_ReadPin(StepPulsePinGPIO, StepPulsePin_1) == GPIO_PIN_RESET)
            {

                __HAL_TIM_SET_COMPARE(&StepMotorTIMHandle, StepPulseCHannel_1, __HAL_TIM_GET_COUNTER(&StepMotorTIMHandle) + (Motor_1.period>>2));
            }
            else
            {
                StepMotor_UpdataPulse(&Motor_1);
                __HAL_TIM_SET_COMPARE(&StepMotorTIMHandle, StepPulseCHannel_1, __HAL_TIM_GET_COUNTER(&StepMotorTIMHandle) + (Motor_1.period>>2));
            }
        }
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
        {
            if (HAL_GPIO_ReadPin(StepPulsePinGPIO, StepPulsePin_2) == GPIO_PIN_RESET)
            {
                
                __HAL_TIM_SET_COMPARE(&StepMotorTIMHandle, StepPulseCHannel_2, __HAL_TIM_GET_COUNTER(&StepMotorTIMHandle) + (Motor_2.period>>2));
            }
            else
            {
                StepMotor_UpdataPulse(&Motor_2);
                __HAL_TIM_SET_COMPARE(&StepMotorTIMHandle, StepPulseCHannel_2, __HAL_TIM_GET_COUNTER(&StepMotorTIMHandle) + (Motor_2.period>>2));
            }
        }
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
        {
            if (HAL_GPIO_ReadPin(StepPulsePinGPIO, StepPulsePin_3) == GPIO_PIN_RESET)
            {

                __HAL_TIM_SET_COMPARE(&StepMotorTIMHandle, StepPulseCHannel_3, __HAL_TIM_GET_COUNTER(&StepMotorTIMHandle) + (Motor_3.period>>2));
            }
            else
            {
                StepMotor_UpdataPulse(&Motor_3);
                __HAL_TIM_SET_COMPARE(&StepMotorTIMHandle, StepPulseCHannel_3, __HAL_TIM_GET_COUNTER(&StepMotorTIMHandle) + (Motor_3.period>>2));
            }
        }
    }
#endif
   

#if( COMPARE_MODE == ADVANCE_MODE )
    if (htim->Instance == StepMotorTIMHandle.Instance)
    {
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {
            if (HAL_GPIO_ReadPin(StepPulsePinGPIO, StepPulsePin_1) == GPIO_PIN_RESET)
            {

                __HAL_TIM_SET_COMPARE(&StepMotorTIMHandle, StepPulseCHannel_1, __HAL_TIM_GET_COUNTER(&StepMotorTIMHandle) + Motor_1.period*Motor_1.duty);
            }
            else
            {
                __HAL_TIM_SET_COMPARE(&StepMotorTIMHandle, StepPulseCHannel_1, __HAL_TIM_GET_COUNTER(&StepMotorTIMHandle) + Motor_1.period*(1-Motor_1.duty));
            }
        }
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
        {
            if (HAL_GPIO_ReadPin(StepPulsePinGPIO, StepPulsePin_2) == GPIO_PIN_RESET)
            {

                __HAL_TIM_SET_COMPARE(&StepMotorTIMHandle, StepPulseCHannel_2, __HAL_TIM_GET_COUNTER(&StepMotorTIMHandle) + Motor_2.period*Motor_2.duty);
            }
            else
            {
                __HAL_TIM_SET_COMPARE(&StepMotorTIMHandle, StepPulseCHannel_2, __HAL_TIM_GET_COUNTER(&StepMotorTIMHandle) + Motor_2.period*(1-Motor_2.duty));
            }
        }
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
        {
            if (HAL_GPIO_ReadPin(StepPulsePinGPIO, StepPulsePin_3) == GPIO_PIN_RESET)
            {

                __HAL_TIM_SET_COMPARE(&StepMotorTIMHandle, StepPulseCHannel_3, __HAL_TIM_GET_COUNTER(&StepMotorTIMHandle) + Motor_3.period*Motor_3.duty);
            }
            else
            {
                __HAL_TIM_SET_COMPARE(&StepMotorTIMHandle, StepPulseCHannel_3, __HAL_TIM_GET_COUNTER(&StepMotorTIMHandle) + Motor_3.period*(1-Motor_3.duty));
            }
        }
    }
#endif
}




