#include "Servo.h"

// 方向舵机
// 最右侧  1300  最左侧 2600

// 夹取舵机
// 闭合极限 900 夹取 950~1050  松开  1250~1300

void Servo_Init()
{
     HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_ALL);//2
     HAL_TIM_PWM_Start(&htim9,TIM_CHANNEL_1);
     HAL_TIM_PWM_Start(&htim9,TIM_CHANNEL_2);

     HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_ALL);//4


     ServoClaw(OpenPan);
     ServoTurn(90);
}

/**
 * @brief 
 * 
 * @param num   舵机编号   
 * @param duty  0~19999   周期20ms  划分为20000份  
 */
void Servo_Pwm_Duty(unsigned char num, unsigned short duty)
{
    switch (num)
    {
        case Claw:
            TIM9->CCR1 = duty;
            break;
            
        case turnClaw:
            TIM9->CCR2 = duty;
            break;

        default:
            break;
    }
}
 
/**
 * @brief 
 * 
 * @param open_cmd  0加紧闭合| 1 大开用于夹取物块 | 2小开 用于装车刚好开
 */
void ServoClaw(int open_cmd)
{
    //越小越闭合
    if (open_cmd == OpenSide)
    { // 抓取侧边物料开口        1320 1250
         TIM9->CCR1 = 1350;
    }
    else if (open_cmd == OpenCar)
    {// 车上放置后 松开     1150~1200
         TIM9->CCR1 = 1200;
    }
    else if(open_cmd == Close)
    { //越小越闭合  1000刚好 不小于 900比较紧张
         TIM9->CCR1 = 1000;
    }
    else if (open_cmd == OpenPan)
    {
        TIM9->CCR1 = 1900;
    }
}
 

void ServoTurn(int angle)
{
    /*
    0   - 2260
    90  - 1300 
    180 - 410
    */
//    2260-410  /180
    switch (angle)
    {
        case  90:
            TIM9->CCR2 = 1300; break;
        case  180:
            TIM9->CCR2 = 400; break;
        case 0:
            TIM9->CCR2 = 2340; break;
        default:
            TIM9->CCR2 = 1300;
            break;
    }
}

 