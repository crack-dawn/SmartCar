#ifndef _STEPMOTOR_H_
#define _STEPMOTOR_H_


#include "main.h"
#include "tim.h"
#include "key.h"

#include "math.h"
/*
1  最下面的 转向  轴心
2  靠近底层的大臂 臂弯  
3  最顶层的小臂   手腕
*/
 
#define StepMotorTIM            TIM5
#define StepMotorTIMHandle      (htim5)


/*方向pin 较之前稳定版本有改动*/
///         方向pin                      方向pin             ||    通道pin
#define StepDirGPIO     GPIOC           // 									 ||
#define StepDirPin_1    GPIO_PIN_1      //C0                 ||   PA 0    CH1
#define StepDirPin_2    GPIO_PIN_2      //C1                 ||   PA 1    CH2
#define StepDirPin_3    GPIO_PIN_3      //C2                 ||   PA 2    CH3
#define StepDirPin_4    GPIO_PIN_4      //C3                 ||   PA 2    CH3


#define Factor                (0.0174533) //acos(-1)/180                            //角度转弧度


/*步进速度*/
//900 很慢   600比较慢   调试450  400比较合适慢   300比较合适
#define StepSpeed       600


/**方向调控 */
#define MOTOR1_DIR_CW         HAL_GPIO_WritePin(StepDirGPIO,StepDirPin_1, GPIO_PIN_RESET) //顺时针
#define MOTOR1_DIR_CCW        HAL_GPIO_WritePin(StepDirGPIO,StepDirPin_1, GPIO_PIN_SET)//逆时针

#define MOTOR2_DIR_CW         HAL_GPIO_WritePin(StepDirGPIO,StepDirPin_2, GPIO_PIN_SET)//顺时针
#define MOTOR2_DIR_CCW        HAL_GPIO_WritePin(StepDirGPIO,StepDirPin_2, GPIO_PIN_RESET)//逆时针

#define MOTOR3_DIR_CW         HAL_GPIO_WritePin(StepDirGPIO,StepDirPin_3, GPIO_PIN_RESET) //顺时针
#define MOTOR3_DIR_CCW        HAL_GPIO_WritePin(StepDirGPIO,StepDirPin_3, GPIO_PIN_SET)//逆时针

//#define MOTOR1_STOP TIM5->CCR1 = 0;
//#define MOTOR2_STOP TIM5->CCR2 = 0;
//#define MOTOR3_STOP TIM5->CCR3 = 0;

//#define MOTOR1_START TIM5->CCR1 = StepSpeed-15;
//#define MOTOR2_START TIM5->CCR2 = StepSpeed+15;
//#define MOTOR3_START TIM5->CCR3 = StepSpeed   ;



/** 电机驱动*/
#define MOTOR1_STOP         do{   HAL_TIM_PWM_Stop_IT(&htim5, TIM_CHANNEL_1);HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0, GPIO_PIN_RESET);}while(0)   //失能通道1
#define MOTOR2_STOP         do{   HAL_TIM_PWM_Stop_IT(&htim5, TIM_CHANNEL_2);HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1, GPIO_PIN_RESET);}while(0)   //失能通道2
#define MOTOR3_STOP         do{   HAL_TIM_PWM_Stop_IT(&htim5, TIM_CHANNEL_3);HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2, GPIO_PIN_RESET);}while(0)   //失能通道3

#define MOTOR1_START        do{   HAL_TIM_PWM_Start_IT(&htim5, TIM_CHANNEL_1); }while(0)   //使能通道1              
#define MOTOR2_START        do{   HAL_TIM_PWM_Start_IT(&htim5, TIM_CHANNEL_2); }while(0)   //使能通道2     
#define MOTOR3_START        do{   HAL_TIM_PWM_Start_IT(&htim5, TIM_CHANNEL_3); }while(0)   //使能通道3  


 
#define MOTOR123_ALLSTOP       do{ MOTOR1_STOP;    MOTOR2_STOP;    MOTOR3_STOP;  }while(0)               

#define MOTOR123_ALLSTART      do{ MOTOR1_START;   MOTOR2_START;   MOTOR3_START; }while(0)   



/**细分参数 和 机械臂参数*/

#define MOTOR_FULL_STEP              0   //满步
#define MOTOR_HALF_STEP              1   //二分之一步
#define MOTOR_QUARTER_STEP           2   //四分之一步
#define MOTOR_EIGHTH_STEP            3   //八分之一步
#define MOTOR_SIXTEENTH_STEP         4   //十六分之一步 

#define L1                  14           //直线下降臂长1   大臂长
#define L2                  19           //直线下降臂长2   小臂长
#define OriginalAngle3      40          // 小臂和重力方向的夹角 
#define Bu                  0.01125     //每步的角度
#define H                   5           //高度
//define TRS                0.62


/*
1  最下面的 转向  轴心
2  靠近底层的大臂 臂弯  
3  最顶层的小臂   手腕
*/

#define ID_1 1  // 最下面的 转向  轴心
#define ID_2 2  // 靠近底层的大臂 臂弯  
#define ID_3 3  // 最顶层的小臂   手腕

#define Dir_Z   1
#define Dir_N   0

#define Flag_doing 1
#define Flag_finish 0

typedef struct  
{
    int ID;
    
    int Pulse;      //绝对位置 向量

    int Flag;       //完成状态标志位

    int Dir;        //方向量

    int tarPulse; //目标脉冲数！！ abs 绝对值量，

    int pwmPulse; // 记录定时器通道发出的脉冲数

    int debug;


}Step_Motor;

#define OVER              (StepMotor_Isover())



extern Step_Motor Motor_1,Motor_2,Motor_3;
extern float S; //小臂大臂展开间距
extern float Angle1,Angle2,Angle3;


/*===================================================*/
/*===============       算法函数     =================*/
/*===================================================*/


void StepMotor_HorizonMove(float dis); //水平移动
// void SetMotor_Vertical(uint8_t Dis_H,uint8_t Con_Num);  //垂直下落，效果不好









/*===================================================*/
/*===============       底层驱动     =================*/
/*===================================================*/
void StepMotor_Set_AnglePulse(float Angle1_,float Angle2_,float Angle3_); //绝对角度   以初始位置为参考

int  StepMotor_Isover(void);// 判断是否电机在当前任务执行完毕
void StepMotor_Set_Speed(unsigned int speed); //  300~600 底层 设置电机转速
void StepMotor_Set_TarPulse(int tar1, int tar2,int tar3);//底层 设置参数 要走的脉冲数  只设定目标值, +顺时针 -逆时针
void StepMotor_Set_AbsPulse(int tar1, int tar2,int tar3); //绝对脉冲数， 以初始位置为参考


#define StepALL_STOP        0
#define StepALL_START       1
#define Step1_STOP          11
#define Step2_STOP          22
#define Step3_STOP          33
/**
 * @brief 
 * 
 * @param Con            电机驱动指令
 * @param speedPeriod     在  StepALL_STOP 时不生效,不会改变速度,可以随意填, 不会改变定时器设置,其他状态均会改变速度
 */
void StepMotor_Drive(int Con, int speedPeriod);  //驱动开关     控制全部电机的启动，或者单个的停止，参数如上
 








/*初始化 与周期回调函数 */
void StepMotor_Init(void);//初始化
void StepMotor_UpdataPulse(int StepMotorNum); // 周期性调用，更新电机的脉冲数  pwmPluse是通道产生的脉冲数  tarpLuse是要走到的脉冲数
void StepMotor_CallBack_TIM5(TIM_HandleTypeDef *htim);//定时器回调任务 刷新脉冲数























// void Step_Calculate_AngleToPulse(float m, float n, float z); // 设置角度，计算脉冲，并设置方向
// void Motor_SetAngle2(float dis);                // 状态2的机械抓平移角度计算
// float Step_Calculate_UpdataS();

void Top_StepMotor_UpdataStatus_CallBack_TIM5(TIM_HandleTypeDef *htim);//回调函数


#endif // _STEPMOTOR_H_