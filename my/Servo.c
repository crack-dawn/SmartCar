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

    //  ServoClaw(OpenSide);
    //  ServoTurn(90);
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
    { // 抓取侧边物料开口    750~900
         TIM9->CCR1 = 1200;
    }
    else if (open_cmd == OpenCar)
    {// 车上放置后 松开     600~700
         TIM9->CCR1 = 1000;
    }
// 0700 下面抓放
// 0900 下面抓取
// 1200 平口开合
    else if(open_cmd == Close)
    { //越小越闭合   400~450
         TIM9->CCR1 = 760;
    }
    else if (open_cmd == OpenPan)
    { //圆盘 850~1000
        TIM9->CCR1 =  1300;
    }
}
 

void ServoTurn(int angle)
{
// 转向舵机
// 0585     正向  90读位置
//          正向 0度位置
// 1800     放置物块到圆环时物块应偏转的位置
// 2500     反向  -90度位置
// 10.63

    if (angle > -91  && angle < 91)
    {
        TIM9->CCR2 = 2500 - (angle + 90 )*10.63;

        switch (angle)
        {
            case  90: // 0500
                TIM9->CCR2 =  585; break;

            case 0: //比零度更靠后一些，兼容之前的写法，  放置物块到圆环时物块应偏转的角度
                TIM9->CCR2 = 1770; break;
            
            case  -90:
                TIM9->CCR2 = 2500; break;

            default:
                break;
        }
    }
}

 