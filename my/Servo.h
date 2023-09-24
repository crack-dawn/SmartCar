#ifndef _SERVO_H_
#define _SERVO_H_

#include "main.h"
#include "tim.h"

typedef enum  
{
    Claw=1,     //PE5
    turnClaw,   //PE6

}Servo_num;


#define Close       0
#define OpenCar     2
#define OpenSide    1
#define OpenPan     3

#define ServoType 2
#if( ServoType == 1)
    #define TurnPan 1100
#else if( ServoType == 2)
    #define TurnPan 1100
#endif
void ServoClaw(int open_cmd);
void ServoTurn(int angle);
void Servo_Init();
void Servo_Pwm_Duty(unsigned char num, unsigned short duty);

#endif // _SERVO_H_