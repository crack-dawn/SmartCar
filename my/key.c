#include "key.h"
#include "gpio.h"
#include "control.h"
#include "StepMotor.h"


int cntt = 0;
int duty1 = 400;
int duty2 = 0;

int  StepMotorTask = 0;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == Key0_EXIT4_Pin)
    {
        StepMotorTask++ ; 
        printf("key0  %d \r\n", StepMotorTask);//ok


        if ( HAL_GPIO_ReadPin(GPIOE,Key0_EXIT4_Pin)  == RESET)
        {
            task = Stop;
            do_cnt = 0;
        }
    }

    if(GPIO_Pin == Key1_EXIT3_Pin)
    {

        StepMotorTask=0 ; 

        
        printf("key1  %d \r\n", StepMotorTask);//ok

				task = Stop;
				do_cnt = 0;
			
        StepMotor_Drive(StepALL_STOP,600);   
				
    }
}


   // duty2 += 50;
        // Servo_Pwm_Duty(turnClaw,duty2);
        // printf("Servo2 duty2:%d ccr2:%d\r\n", duty2,TIM9->CCR2);	
        // if (duty2>2600)  
        // {
        //     duty2=0;
        // }//400  2400


       