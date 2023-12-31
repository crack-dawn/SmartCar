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


void Angle_vertical(float set_H,float set_S,int mod)           //垂直下落
{
    float HP = sqrt(set_H * set_H + set_S * set_S);
    float a = acos((L2 * L2 + L1 * L1 - HP * HP) / (2 * L1 * L2)) / (Factor);         //大臂和小臂的夹角
    //水平上还是水平下，在两种计算方法不一样
    if (set_H <= 0)
       Angle2 = a - (asin(set_S / HP) / (Factor)-acos((L2 * L2 + HP * HP - L1 * L1) / (2 * L2 * HP)) / (Factor)); 
    else if(set_H > 0)
        Angle2 =a-( 180 - (asin(set_S / HP) / (Factor)) - (acos((L2 * L2 + HP * HP - L1 * L1) / (2 * L2 * HP)) / (Factor)));
    
    float two_Point = sqrt(L2_1 * L2_1 + L1 * L1 - (2 * cos((180 - a)*Factor) * L2_1 * L1));
    float b = acos((L1 * L1 + two_Point * two_Point - L2_1 * L2_1) / (2 * L1 * two_Point)) / (Factor);       //四边形靠近大臂的夹角
    float c = acos((L3 * L3 + two_Point * two_Point - L1_1 * L1_1) / (2 * L3 * two_Point))/(Factor);              //四边形靠近小滑轮的夹角
    Angle3 = 90 + Angle2 - b - c;

    StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,Angle2,Angle3);
    if(set_H>0)
    {   
       int Ratio=Motor_3.tarPulse/Motor_2.tarPulse;

       if(mod==1)
       {
            StepMotor_AdVanceDrive(0,Abs(1000000/400*Ratio),400);   //调速
            while (OVER == Flag_doing)
            {
                if(Motor_1.Flag==Flag_finish&&Motor_3.Flag==Flag_finish)
                {
                    StepMotor_AdVanceDrive(0,800,0);
                    while (OVER == Flag_doing);
                }
            }
       }

       else if (mod==2)
       {
            StepMotor_AdVanceDrive(0,Abs(1000000/400*Ratio),400);   //调速
            while (OVER == Flag_doing);
       }
        
    }

    if(set_H<=0)
    {
        int Ratio=Motor_3.tarPulse/Motor_2.tarPulse;
        StepMotor_AdVanceDrive(0,800,400);   //调速
        while (OVER == Flag_doing);
    }
    
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

void StepArm_Task_ReInitPosition(int X, int Y, int Z)
{
       int xPulse = X;
      int yPulse = Y;
      int zPulse = Z;

 //调整条件
     StepMotor_Set_TarPulses(X,Y,Z); //然后校准 大臂和转轴，小臂放下
     StepMotor_Drive(1, 800);

 //初始化复位条件
     while ( OVER != Flag_finish) {     Limit_Arm_InitPosition_INPUT();   }


 //初始化复位值
     Motor_1.Pulse = 0;
     Motor_2.Pulse = 0;
     Motor_3.Pulse = -2577;
 
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

void StepMotor_AdVanceDrive(int period1,int period2,int period3)
{
         StepMotor_Set_Speeds(period1,Abs(period2),period3);
         if (Motor_1.tarPulse != 0 )
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

/*------------------------*/
void StepMotor_Init(void)
{
    
    // 失能motor
    STEP1234_ALLSTOP; 
    StepPulseEN(ENABLE);//后使能 之后全程使能

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
    // g_Motor->frequency = CLK_Freq/period;
}
void StepMotor_Set_Speeds(unsigned int period1,unsigned int period2,unsigned int period3)
{
        Motor_1.period = period1;     
        Motor_2.period = period2;     
        Motor_3.period = period3;     

        // Motor_1.frequency = CLK_Freq/period1;
        // Motor_2.frequency = CLK_Freq/period2;
        // Motor_3.frequency = CLK_Freq/period3;
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
    // if (htim->Instance == StepMotorTIMHandle.Instance)
    // {
    //     if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    //     {
    //         if (HAL_GPIO_ReadPin(StepPulsePinGPIO, StepPulsePin_1) == GPIO_PIN_RESET)
    //         {

    //             __HAL_TIM_SET_COMPARE(&StepMotorTIMHandle, StepPulseCHannel_1, __HAL_TIM_GET_COUNTER(&StepMotorTIMHandle) + (Motor_1.speedValue>>2));
    //         }
    //         else
    //         {
    //             StepMotor_UpdataPulse(&Motor_1);
    //             __HAL_TIM_SET_COMPARE(&StepMotorTIMHandle, StepPulseCHannel_1, __HAL_TIM_GET_COUNTER(&StepMotorTIMHandle) + (Motor_1.speedValue>>2));
    //         }
    //     }
    //     if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    //     {
    //         if (HAL_GPIO_ReadPin(StepPulsePinGPIO, StepPulsePin_2) == GPIO_PIN_RESET)
    //         {
    //             __HAL_TIM_SET_COMPARE(&StepMotorTIMHandle, StepPulseCHannel_2, __HAL_TIM_GET_COUNTER(&StepMotorTIMHandle) + (Motor_2.speedValue>>2));
    //         }
    //         else
    //         {
    //             StepMotor_UpdataPulse(&Motor_2);
    //             __HAL_TIM_SET_COMPARE(&StepMotorTIMHandle, StepPulseCHannel_2, __HAL_TIM_GET_COUNTER(&StepMotorTIMHandle) + (Motor_2.speedValue>>2));
    //         }
    //     }
    //     if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
    //     {
    //         if (HAL_GPIO_ReadPin(StepPulsePinGPIO, StepPulsePin_3) == GPIO_PIN_RESET)
    //         {
    //             __HAL_TIM_SET_COMPARE(&StepMotorTIMHandle, StepPulseCHannel_3, __HAL_TIM_GET_COUNTER(&StepMotorTIMHandle) + (Motor_3.speedValue>>2));
    //         }
    //         else
    //         {
    //             StepMotor_UpdataPulse(&Motor_3);
    //             __HAL_TIM_SET_COMPARE(&StepMotorTIMHandle, StepPulseCHannel_3, __HAL_TIM_GET_COUNTER(&StepMotorTIMHandle) + (Motor_3.speedValue>>2));
    //         }
    //     }
    // }
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






/*=======================================================================================================================================*/
/*=======================================================================================================================================*/
/*=======================================================================================================================================*/
/*=======================================================================================================================================*/
//200段 周期加速过程 下列是递减的周期序列
// unsigned int CurveS[200]={2998,2998,2997,2997,2997,2997,2996,2996,2996,2995,2995,2995,2994,\
// 	2994,2993,2993,2992,2992,2991,2990,2989,2989,2988,2987,2986,2985,2984,2982,2981,2980,2978,\
// 	2977,2975,2973,2971,2969,2967,2964,2962,2959,2956,2953,2950,2946,2942,2938,2934,2929,2924,\
// 	2918,2913,2907,2900,2893,2885,2877,2869,2860,2850,2840,2829,2817,2805,2792,2778,2763,2748,\
// 	2731,2714,2695,2675,2655,2633,2610,2586,2561,2534,2506,2477,2447,2415,2382,2347,2312,2275,\
// 	2236,2197,2156,2114,2071,2027,1983,1937,1890,1843,1795,1747,1698,1649,1600,1552,1503,1454,\
// 	1406,1358,1311,1264,1218,1174,1130,1087,1045,1004,965,926,889,854,819,786,754,724,695,667,\
// 	640,615,591,568,546,526,506,487,470,453,438,423,409,396,384,372,361,351,341,332,324,316,\
// 	308,301,294,288,283,277,272,267,263,259,255,251,248,245,242,239,237,234,232,230,228,226,\
// 	224,223,221,220,219,217,216,215,214,213,212,212,211,210,209,209,208,208,207,207,206,206,\
// 	206,205,205,205,204,204,204,204,203,203,203};

// /*速度调整*/
// void SpeedChoose(Step_Motor* g_Motor)
// {
// 	g_Motor->speedValue = CurveS_Para[g_Motor->Count];    //计算本次速度
		
// 	if(g_Motor->adjustStatus == SPEED_INCREASE)            //加速
// 	{
// 		g_Motor->CountTemp++;
// 	}
// 	else if(g_Motor->adjustStatus == SPEED_DECREASE)       //减速
// 	{
// 		g_Motor->CountTemp--;	
// 	}
// 	g_Motor->Count = g_Motor->CountTemp*5/g_Motor->CountPropor;    //加速次数
// }


// /*函数周期性调用*/
// void StepMotorSpeedAdjust(Step_Motor* g_Motor)
// {
// 	switch(g_Motor->adjustStatus)
// 	{
// 		/*加速*/						
// 	case SPEED_INCREASE:
// 		if(g_Motor->Count <= g_Motor->AccelPulse || g_Motor->tarPulse - )
// 		{
// 			g_Motor->speedValue = CurveS_Para[g_Motor->Count];    //计算本次速度周期
//             ++g_Motor->Count;
// 		}
// 		else
// 		{
// 			g_Motor->adjustStatus = SPEED_STABLE;
// 		}
// 		break;
		
// 		/*匀速*/	
// 	case SPEED_STABLE:
// 		if(g_Motor->PWMcount >= (g_Motor->PWMneed-g_Motor->SpeedDecrPWM))
// 		{		
// 			g_Motor->adjustStatus = SPEED_DECREASE;	
// 		}
// 		break;
		
// 		/*减速*/	
// 	case SPEED_DECREASE:
// 		if(g_Motor->Count >= 0)
// 		{
// 			SpeedChoose(g_Motor->Count); 
// 		}
		
// 		if(g_Motor->PWMover == 1)
// 		{		
// 			HAL_TIM_PWM_Stop_IT(&htim1, TIM_CHANNEL_1);
// 		}
// 		break;
		
// 	default :
// 		break;
// 	}
// }









/*******************************************************/
/******************    初始化位置    ********************/
/******************                 ********************/
/*******************************************************/
void Limit_Arm_InitPosition_EXIT(uint16_t GPIO_Pin)
{
   if (GPIO_Pin == Limit_Turn_Pin && HAL_GPIO_ReadPin(Limit_Turn_GPIO_Port, Limit_Turn_Pin)== RESET)
    {
        Motor_1.stop(); Motor_1.Pulse = 0; //底座接转轴  PB11
    }
    if (GPIO_Pin == Limit_BigArm_Pin && HAL_GPIO_ReadPin(Limit_BigArm_GPIO_Port, Limit_BigArm_Pin)== RESET)
    {
        Motor_2.stop(); Motor_2.Pulse = 0; //大臂接  PE12
    }
     if (GPIO_Pin == Limit_SmallArm_Pin && HAL_GPIO_ReadPin(Limit_SmallArm_GPIO_Port, Limit_SmallArm_Pin)== RESET)
    {
        Motor_3.stop(); Motor_3.Pulse = 0; //小臂接  PB15
        ServoTurn(90); //舵机归位
        ServoClaw(OpenSide);
    }
}

void Limit_Arm_InitPosition_INPUT()
{
    if (HAL_GPIO_ReadPin(Limit_Turn_GPIO_Port, Limit_Turn_Pin)== RESET)
    {
        Motor_1.stop(); Motor_1.Pulse = 0;//底座转轴
    }
     if (HAL_GPIO_ReadPin(Limit_BigArm_GPIO_Port, Limit_BigArm_Pin)== RESET)
    {
        Motor_2.stop(); Motor_2.Pulse = 0;//大臂
    }
     if( HAL_GPIO_ReadPin(Limit_SmallArm_GPIO_Port, Limit_SmallArm_Pin)== RESET)
    {
        Motor_3.stop(); Motor_3.Pulse = 0;//小臂
        // ServoTurn(90); //舵机归位
        // ServoClaw(OpenSide);
    }
}

void StepArm_Task_InitPosition()
{
     StepPulseEN(ENABLE);//后使能 之后全程使能
    StepPulseEN(ENABLE);
    StepPulseEN(ENABLE);
     StepMotor_Set_TarPulses(3000,-3000 ,-3000); //然后校准 大臂和转轴，小臂放下
     StepMotor_Drive(1, 1200);
     while ( OVER != Flag_finish) {     Limit_Arm_InitPosition_INPUT();   }
     
     Motor_1.Pulse = 0;
     Motor_2.Pulse = 0;
     Motor_3.Pulse = -2577;

     HAL_NVIC_DisableIRQ(Limit_Turn_EXTI_IRQn);
     HAL_NVIC_DisableIRQ(Limit_SmallArm_EXTI_IRQn);
     HAL_NVIC_DisableIRQ(Limit_BigArm_EXTI_IRQn);

     ServoTurn(90); //舵机归位
     ServoClaw(OpenCar);
     HAL_Delay(1750);
}
