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
        printf("key0   \r\n" );//ok

        if ( HAL_GPIO_ReadPin(GPIOE,Key0_EXIT4_Pin)  == RESET)
        {
            task = 1;
            do_cnt = 0;
        }
    }

    if(GPIO_Pin == Key1_EXIT3_Pin)
    {

        StepMotorTask=0 ; 

        printf("key1  \r\n" );//ok

				task = Stop;
				do_cnt = 0;
			
        StepMotor_Drive(StepALL_STOP,600);   
    }

    Limit_Arm_InitPosition_EXIT(GPIO_Pin);
}






       