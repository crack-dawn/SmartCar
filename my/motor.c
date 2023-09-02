#include "motor.h"


#define Abs(x)   ((x) < 0 ? -(x) : (x))  //求绝对值


/**************************************************************************
函数功能：限幅 
入口参数：电机PWM值
返回  值：限制后的值
**************************************************************************/
float Limit(float data,float max)
{	
    if(data<-max) data=-max;	
    if(data> max) data= max;	
    return data;
}


void Car_Drive_Init()
{
        /*  start Tim encoder to count distance*/
      HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_ALL);  HAL_Delay(2);
      HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_ALL);  HAL_Delay(2);

        /*????TIM1 motor pwm??????*/
      //HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_ALL);
      HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);  
      // HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);  
      // HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);  
      HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);  

      /*????  TIM6   updata car speed and distance   */ 
      PID_param_init();                          
      HAL_TIM_Base_Start_IT(&htim6);      /*  TIM6 encoder and motor drive */
			/*????  TIM7   updata task  to do  */
      TIM3 -> CNT=0;  TIM4 -> CNT=0;    
      HAL_TIM_Base_Start_IT(&htim7);             
       /*  TIM7 update action status to control car to run */
      TIM3 -> CNT=0;  TIM4 -> CNT=0;             

    //   task = Stop;
    //   do_cnt = 0;
}


 
/**************************************************************************
函数功能：赋值给PWM寄存器  双路操作
入口参数：pwmA pwmB 分别调节电机1 电机2转速
返回  值：无
**************************************************************************/
/*pwmA 右侧电机-- pid_speed2，  pwmB 左侧电机---pid_speed*/
void Set_Pwm(int pwmA,int pwmB)
{
/**
		pid_speed2.actual_val   =  +(short)TIM3 -> CNT;   	//TIM3   PC6  PC7  E1 右轮子|  M1  PE9   pwmA | AN1 AN2| pid_speed2 |右轮子
		pid_speed.actual_val  	=  -(short)TIM4 -> CNT;   	//TIM4   PD12 PD13 E2 左轮子|  M2  PE11  pwmB | BN1 BN2| pid_speed  |左轮子
**/
#if(Motor == 1)
        if(pwmB>0)      /* 正转 */ /*左侧电机正反转驱动*/
        {
            BIN1(0);	BIN2(1);
        }
        else  if(pwmB<0)        /* 反转 */
        {
            BIN2(0);    BIN1(1);	
        }
        
        if(pwmA>0)      /* 正转 */ /*右侧电机正反转驱动*/
        {
            AIN1(0); AIN2(1);            
        }
        else  if(pwmA<0)         /* 反转 */
        {
            AIN2(0);  AIN1(1);    
        }
#else if(Motor == 3)
        if(pwmB>0)      /* 正转 */ /*左侧电机正反转驱动*/
        {
            BIN1(1);	BIN2(0);  /*左轮*/
        }
        else  if(pwmB<0)        /* 反转 */
        {
            BIN2(1);    BIN1(0);	
        }
        
        if(pwmA>0)      /* 正转 */ /*右侧电机正反转驱动*/
        {
            AIN1(1); AIN2(0);     /*右轮*/       
        }
        else  if(pwmA<0)         /* 反转 */
        {
            AIN2(1);  AIN1(0);    
        }
#endif // 

    if(pwmA || pwmB)        /* 控制器有输出 */        
    {  
        pwmB = Limit(pwmB,Pwm_Amplitude);
        pwmA = Limit(pwmA,Pwm_Amplitude);/* 限幅  */

		PWM2(   Abs(pwmB)   ); /*左侧电机驱动*/
        PWM1(   Abs(pwmA)   ); /*右侧电机驱动*/
        
    }
    else            /* 无输出，直接关闭驱动 */       
    {
		PWM1(0);         PWM2(0);   
        AIN1(0);     	 AIN2(0);
        BIN1(0);	   	 BIN2(0);
    }
}

void Moto_Stop(void)
{	
    PWM1(0);        PWM2(0);
		AIN1(0);				AIN2(0);     
		BIN1(0);				BIN2(0);  	 
}
