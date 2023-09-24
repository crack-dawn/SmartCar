#include "Servo.h"




#if( ServoType == 1)
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
/*
2470      -90 真实值
1800      0 真实值
1145      90 真实值
0540      180 真实值
min = { 2470- 540 } /  180-（-90） 
*/
    if (angle == TurnPan)
    {
       TIM9->CCR2 = TurnPan;

    }
    

    if (angle > -91  && angle < 181)
    {
        TIM9->CCR2 = 2470 - (angle + 90 )*7.148;

        switch (angle)
        {
            case  90:  // 真实的 90度 
                TIM9->CCR2 =  1145; break;

            case 0:    // 真实的零度
                TIM9->CCR2 =  1800; break;
            
            case  -90: //真实的-90度
                TIM9->CCR2 =  2470; break;

            case 180:  //伪装的180  为了方便调用
                TIM9->CCR2 =   620; break;
 
            default:
                break;
        }
    }
}

#else if(ServoType == 2)
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
         TIM9->CCR1 = 1000;
    }
    else if (open_cmd == OpenCar)
    {// 车上放置后 松开     600~700
         TIM9->CCR1 = 800;
    }
// 0700 下面抓放
// 0900 下面抓取
// 1200 平口开合
    else if(open_cmd == Close)
    { //越小越闭合   400~450
         TIM9->CCR1 = 550;
    }
    else if (open_cmd == OpenPan)
    { //圆盘 850~1000
        TIM9->CCR1 =  1100;
    }
}
 

void ServoTurn(int angle)
{
// 转向舵机
/*
2470      -90 真实值
1800      0 真实值
1145      90 真实值
0540      180 真实值
min = { 2470- 540 } /  180-（-90） 
*/
    if (angle == TurnPan)
    {
       TIM9->CCR2 = TurnPan;

    }
    

    if (angle > -91  && angle < 181)
    {
        TIM9->CCR2 = 2470 - (angle + 90 )*7.148;

        switch (angle)
        {
            case  90:  // 真实的 90度 
                TIM9->CCR2 =  1145; break;

            case 0:    // 真实的零度
                TIM9->CCR2 =  1800; break;
            
            case  -90: //真实的-90度
                TIM9->CCR2 =  2470; break;

            case 180:  //伪装的180  为了方便调用
                TIM9->CCR2 =   620; break;
 
            default:
                break;
        }
    }
}


#endif
