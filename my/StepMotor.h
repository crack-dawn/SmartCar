#ifndef _STEPMOTOR_H_
#define _STEPMOTOR_H_

#include "main.h"
#include "tim.h"
#include "key.h"

#include "math.h"

#define DebugF printf("pwmpulse:|%d |%d |%d|\r\n",Motor_1.pwmPulse,Motor_2.pwmPulse,Motor_3.pwmPulse) 
#define DebugFF printf("pulse:|%d |%d |%d|\r\n",Motor_1.Pulse,Motor_2.Pulse,Motor_3.Pulse) 
#define DebugCNT printf("TIM:%d |%d |%d|%d |\r\n",TIM5->CNT,TIM5->CCR1,TIM5->CCR2,TIM5->CCR3) 
#define DebugTar printf("tar:|%d |%d |%d|\r\n",Motor_1.tarPulse,Motor_2.tarPulse,Motor_3.tarPulse) 

/*模式选择 */
#define SIMPLE_MMODE    1
#define ADVANCE_MODE    2
#define COMPARE_MODE SIMPLE_MMODE

/*===================================================*/
/*==============  参数和变量定义   ===================*/
/*===================================================*/

/*步进速度的对应周期计数时长 */
//900 很慢   600比较慢   调试450  400比较合适慢   300比较合适  计时器频率为 1MHz
#define StepSpeed       600

/**细分参数 和 机械臂参数*/
#define Factor                (0.0174533) //acos(-1)/180                            //角度转弧度

#define MOTOR_FULL_STEP              0   //满步
#define MOTOR_HALF_STEP              1   //二分之一步
#define MOTOR_QUARTER_STEP           2   //四分之一步
#define MOTOR_EIGHTH_STEP            3   //八分之一步
#define MOTOR_SIXTEENTH_STEP         4   //十六分之一步 

#define L1          140
#define L1_1        140
#define L2          170
#define L2_1        20
#define L3          30
#define L3_Angle    29
#define OriginalAngle3   29
#define Bu                  0.01125     //每步的角度



/*
1  最下面的 转向  轴心
2  靠近底层的大臂 臂弯  
3  最顶层的小臂   手腕
*/
/*============= 步进电机参数列表宏定义 ================*/
typedef void (*function)(void);

#define ID_1         1  // 最下面的 转向  轴心
#define ID_2         2  // 靠近底层的大臂 臂弯  
#define ID_3         3  // 最顶层的小臂   手腕

#define Flag_doing 1
#define Flag_finish 0

#define Dir_CW  1
#define Dir_CCW 0

#define CLK_Freq (unsigned int)(1000000) //1M Hz


#define  SPEED_INCREASE 11
#define  SPEED_STABLE   22
#define  SPEED_DECREASE 33

typedef struct  
{
    char ID;
    char Flag;       //动作完成状态标志位

    char Dir;        //方向量 +-会决定转动方向
    int Pulse;       //绝对位置 向量

    int tarPulse;       //绝对数值作为目标脉冲数， 
    int pwmPulse;       //记录定时器通道发出的脉冲数

    unsigned int period;    //跳变周期 也就是速度的反映，越大越慢 这里是 200~4000 cnt of clk
    unsigned int frequency; //当前周期对应 脉冲频率           这里是 5000~500 Hz

    unsigned int speedValue; //速度值 这里取 周期的S曲线
    unsigned int Count;             //加速次数累计
    unsigned int CountTemp;         //加速次数累计 暂存值
    unsigned int AccelPulse;  //加速脉冲值
    unsigned int DecrePulse;  //减速脉冲值
    char         adjustStatus;   //调速状态

    function start; //开启通道
    function stop;  //关闭通道
    function cw;    //设置正方向
    function ccw;
 

#if(COMPARE_MODE == ADVANCE_MODE )
    float duty; //占空比  0.00 ~ 1.00
    int debug;
#endif 

}Step_Motor;


extern Step_Motor Motor_1,Motor_2,Motor_3,Motor_4;
extern float S; //小臂大臂展开间距
extern float Angle1,Angle2,Angle3;

 
/*===================================================*/
/*=============  方向控制引脚#define  ================*/
/*===================================================*/
/*SHEILD V3 的使能控制*/
#define StepENableGPIO          GPIOC
#define StepENablePin           GPIO_PIN_4
#define StepPulseEN(x)           do{ ( x>0 ) ? (HAL_GPIO_WritePin(StepENableGPIO, StepENablePin, GPIO_PIN_RESET)) : (HAL_GPIO_WritePin(StepENableGPIO, StepENablePin, GPIO_PIN_SET)) ; } while (0)

/*方向pin 较之前稳定版本有改动*/
//         方向pin                      方向pin       原本pin    应该pin     ||    通道pin
#define StepDirGPIO             GPIOC           // 									 ||
#define StepDirPin_1            GPIO_PIN_0      //C0    1         0        ||   PA 0    CH1
#define StepDirPin_2            GPIO_PIN_1      //C1    2         1        ||   PA 1    CH2
#define StepDirPin_3            GPIO_PIN_2      //C2    3         2        ||   PA 2    CH3
#define StepDirPin_4            GPIO_PIN_3      //C3    4         3        ||   PA 3    CH4



/*===================================================*/
/*=============  方向驱动函数#define =================*/
/*===================================================*/
/**方向调控 */ /* 正方向 clockwise && 反方向 counter-clockwise*/
#define STEP1_DIR_CW        do{Motor_1.Dir=Dir_CW; HAL_GPIO_WritePin(StepDirGPIO, StepDirPin_1, GPIO_PIN_RESET);  }while(0) // 顺时针
#define STEP1_DIR_CCW       do{Motor_1.Dir=Dir_CCW; HAL_GPIO_WritePin(StepDirGPIO, StepDirPin_1, GPIO_PIN_SET);   }while(0)// 逆时针

#define STEP2_DIR_CW        do{Motor_2.Dir=Dir_CW; HAL_GPIO_WritePin(StepDirGPIO, StepDirPin_2, GPIO_PIN_SET);   }while(0) // 顺时针
#define STEP2_DIR_CCW       do{Motor_2.Dir=Dir_CCW; HAL_GPIO_WritePin(StepDirGPIO, StepDirPin_2, GPIO_PIN_RESET);  }while(0)// 逆时针

#define STEP3_DIR_CW        do{Motor_3.Dir=Dir_CW; HAL_GPIO_WritePin(StepDirGPIO, StepDirPin_3, GPIO_PIN_SET);   }while(0)// 顺时针
#define STEP3_DIR_CCW       do{Motor_3.Dir=Dir_CCW; HAL_GPIO_WritePin(StepDirGPIO, StepDirPin_3, GPIO_PIN_RESET);  }while(0)// 逆时针

#define STEP4_DIR_CW        do{Motor_4.Dir=Dir_CW; HAL_GPIO_WritePin(StepDirGPIO, StepDirPin_4, GPIO_PIN_RESET);  }while(0) // 顺时针
#define STEP4_DIR_CCW       do{Motor_4.Dir=Dir_CCW; HAL_GPIO_WritePin(StepDirGPIO, StepDirPin_4, GPIO_PIN_SET);   }while(0)// 逆时针

static void Step1_Dir_Cw(void) {STEP1_DIR_CW; }       static void Step1_Dir_CCw(void) {STEP1_DIR_CCW; }   
static void Step2_Dir_Cw(void) {STEP2_DIR_CW; }       static void Step2_Dir_CCw(void) {STEP2_DIR_CCW; }   
static void Step3_Dir_Cw(void) {STEP3_DIR_CW; }       static void Step3_Dir_CCw(void) {STEP3_DIR_CCW; }   
static void Step4_Dir_Cw(void) {STEP4_DIR_CW; }       static void Step4_Dir_CCw(void) {STEP4_DIR_CCW; } 




/*===================================================*/
/*=============  脉冲输出引脚#define  ================*/
/*===================================================*/
/*脉冲pin 新增*/
#define StepMotorTIM            TIM5
#define StepMotorTIMHandle      (htim5)
#define StepPulsePinGPIO        GPIOA
#define StepPulsePin_1          GPIO_PIN_0
#define StepPulsePin_2          GPIO_PIN_1
#define StepPulsePin_3          GPIO_PIN_2
#define StepPulsePin_4          GPIO_PIN_3
/*脉冲脚对应的定时器通道*/
#define StepPulseCHannel_1      TIM_CHANNEL_1   
#define StepPulseCHannel_2      TIM_CHANNEL_2
#define StepPulseCHannel_3      TIM_CHANNEL_3
#define StepPulseCHannel_4      TIM_CHANNEL_4


/*===================================================*/
/*=============  脉冲驱动函数#define =================*/
/*===================================================*/
/** 电机驱动 **/
#define STEP1_STOP         do{  HAL_TIM_OC_Stop_IT(&StepMotorTIMHandle, StepPulseCHannel_1);  Motor_1.pwmPulse = 0; Motor_1.tarPulse = 0; Motor_1.Flag = Flag_finish;    /*HAL_GPIO_WritePin(StepPulsePinGPIO, StepPulsePin_1, GPIO_PIN_RESET);*/}while(0)   //失能通道1
#define STEP2_STOP         do{  HAL_TIM_OC_Stop_IT(&StepMotorTIMHandle, StepPulseCHannel_2);  Motor_2.pwmPulse = 0; Motor_2.tarPulse = 0; Motor_2.Flag = Flag_finish;    /*HAL_GPIO_WritePin(StepPulsePinGPIO, StepPulsePin_2, GPIO_PIN_RESET);*/}while(0)   //失能通道2
#define STEP3_STOP         do{  HAL_TIM_OC_Stop_IT(&StepMotorTIMHandle, StepPulseCHannel_3);  Motor_3.pwmPulse = 0; Motor_3.tarPulse = 0; Motor_3.Flag = Flag_finish;    /*HAL_GPIO_WritePin(StepPulsePinGPIO, StepPulsePin_3, GPIO_PIN_RESET);*/}while(0)   //失能通道3
#define STEP4_STOP         do{  HAL_TIM_OC_Stop_IT(&StepMotorTIMHandle, StepPulseCHannel_4);  Motor_4.pwmPulse = 0; Motor_4.tarPulse = 0; Motor_4.Flag = Flag_finish;    /*HAL_GPIO_WritePin(StepPulsePinGPIO, StepPulsePin_4, GPIO_PIN_RESET);*/}while(0)   //失能通道3

#define STEP1_START        do{  Motor_1.Flag = Flag_doing; Motor_1.pwmPulse = 0;  HAL_TIM_OC_Start_IT(&StepMotorTIMHandle, StepPulseCHannel_1); __HAL_TIM_SET_COMPARE(&StepMotorTIMHandle, StepPulseCHannel_1, __HAL_TIM_GET_COUNTER(&StepMotorTIMHandle)+1200); }while(0)   //使能通道1              
#define STEP2_START        do{  Motor_2.Flag = Flag_doing; Motor_2.pwmPulse = 0;  HAL_TIM_OC_Start_IT(&StepMotorTIMHandle, StepPulseCHannel_2); __HAL_TIM_SET_COMPARE(&StepMotorTIMHandle, StepPulseCHannel_2, __HAL_TIM_GET_COUNTER(&StepMotorTIMHandle)+1200); }while(0)   //使能通道2     
#define STEP3_START        do{  Motor_3.Flag = Flag_doing; Motor_3.pwmPulse = 0;  HAL_TIM_OC_Start_IT(&StepMotorTIMHandle, StepPulseCHannel_3); __HAL_TIM_SET_COMPARE(&StepMotorTIMHandle, StepPulseCHannel_3, __HAL_TIM_GET_COUNTER(&StepMotorTIMHandle)+1200); }while(0)   //使能通道3  
#define STEP4_START        do{  Motor_4.Flag = Flag_doing; Motor_4.pwmPulse = 0;  HAL_TIM_OC_Start_IT(&StepMotorTIMHandle, StepPulseCHannel_4); __HAL_TIM_SET_COMPARE(&StepMotorTIMHandle, StepPulseCHannel_4, __HAL_TIM_GET_COUNTER(&StepMotorTIMHandle)+1200); }while(0)   //使能通道3  

static void Step1_Stop(void) { STEP1_STOP; };       static void Step1_Start(void) { STEP1_START; };
static void Step2_Stop(void) { STEP2_STOP; };       static void Step2_Start(void) { STEP2_START; };
static void Step3_Stop(void) { STEP3_STOP; };       static void Step3_Start(void) { STEP3_START; };
static void Step4_Stop(void) { STEP4_STOP; };       static void Step4_Start(void) { STEP4_START; };


#define STEP1234_ALLSTOP       do{ STEP1_STOP;    STEP2_STOP;    STEP3_STOP;   STEP4_STOP; }while(0)               
#define STEP1234_ALLSTART      do{ STEP1_START;   STEP2_START;   STEP3_START;  STEP4_START; }while(0)   



/*===================================================*/
/*===============       算法函数     =================*/
/*===================================================*/

float Calculate_DisHorizon();                             //计算水平距离
void Angle_vertical(float set_H,float set_S,int mod);             //垂直下落
void StepMotor_Set_AnglePulse(float Angle1_,float Angle2_,float Angle3_); //绝对角度   以初始位置为参考
void StepArm_Task_ReInitPosition(int X, int Y, int Z);





/*===================================================*/
/*===============       底层驱动     =================*/
/*===================================================*/
#define OVER              (StepMotor_Isover())
char StepMotor_Isover(void);// 判断是否电机在当前任务执行完毕
void StepMotor_AdVanceDrive(int period1,int period2,int period3);   //调速


/*设置电机方向 Forward顺时针*/
void StepMotor_Set_Dir(Step_Motor*  g_Motor, int dir);                      // 设定方向
void StepMotor_Set_Dirs(int dir1, int dir2, int dir3);       // 设定方向

/*设置电机转速*/
void StepMotor_Set_Speed(Step_Motor*  g_Motor, unsigned int period);         //  300~600 底层 设置电机转速
void StepMotor_Set_Speeds(unsigned int period1,unsigned int period2,unsigned int period3);
 

/*值设定目标值*/ 
//目标值的正负 代表电机转向的正反
void StepMotor_Set_TarPulse(Step_Motor*  g_Motor, int tarPulse);             // 值设定目标值
void StepMotor_Set_TarPulses(int tar1, int tar2,int tar3); //  底层 设置参数 要走的脉冲数  只设定目标值, +顺时针 -逆时针

/*其他函数*/
void StepMotor_Set_AbsPulse(int tar1, int tar2,int tar3); //  绝对脉冲数， 以初始位置为参考
void Limit_Arm_InitPosition_INPUT();
// void StepMotor_Set_AbsPulse(int tar1, int tar2, int tar3);

#define StepALL_STOP        0
#define StepALL_START       1
#define Step1_STOP          11
#define Step2_STOP          22
#define Step3_STOP          33

void StepMotor_Drive(int Con, int speedPeriod);  //驱动开关     控制全部电机的启动，或者单个的停止，参数如上


/*初始化 与周期回调函数 */
void StepMotor_Init(void);//初始化
void StepMotor_UpdataPulse(Step_Motor* g_Motor); // 周期性调用，更新电机的脉冲数  pwmPluse是通道产生的脉冲数  tarpLuse是要走到的脉冲数






















// void Step_Calculate_AngleToPulse(float m, float n, float z); // 设置角度，计算脉冲，并设置方向
// void Motor_SetAngle2(float dis);                // 状态2的机械抓平移角度计算
// float Step_Calculate_UpdataS();

void Top_StepMotor_UpdataStatus_CallBack_TIM5(TIM_HandleTypeDef *htim);//回调函数


#endif // _STEPMOTOR_H_