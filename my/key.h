#ifndef _KEY_H_
#define _KEY_H_

#include "main.h"

#include "PID.h"
#include "my_uart.h"
#include "control.h"
#include "StepMotor.h"
#include "motor.h"

#define LED0_ON HAL_GPIO_WritePin(LED0_PF9_GPIO_Port,LED0_PF9_Pin, GPIO_PIN_RESET)    //步进电机执行指示灯
#define LED0_OFF HAL_GPIO_WritePin(LED0_PF9_GPIO_Port,LED0_PF9_Pin, GPIO_PIN_SET)  

#define LED1_ON HAL_GPIO_WritePin(LED1_PF10_GPIO_Port,LED1_PF10_Pin, GPIO_PIN_RESET)  //主程序执行指示灯
#define LED1_OFF HAL_GPIO_WritePin(LED1_PF10_GPIO_Port,LED1_PF10_Pin, GPIO_PIN_SET)  
 
extern int StepMotorTask ;

#endif // _KEY_H_