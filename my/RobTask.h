#ifndef _ROBTASK_H_
#define _ROBTASK_H_

#include "main.h"
#include "my_uart.h"

#include "StepMotor.h"
#include "Servo.h"


#define DebugA printf("A dis|%5f turn|%5f\r\n",RxData.distance, RxData.turn )      //巡线
#define DebugB printf("B R|%5f G|%5f B|%5f\r\n",RxData.B_ang[1], RxData.B_ang[2],RxData.B_ang[3]) //色块
#define DebugC printf("C R|%5f G|%5f B|%5f\r\n",RxData.C_ang[1], RxData.C_ang[2],RxData.C_ang[3]) //色环
#define DebugT printf("task:%d, do_cnt:%d, Rx_Flag:%d\r\n",task,do_cnt,RxData.codeFlag)         //任务
   
#define DebugCode printf("Rx_Flag:%d,code:%s\r\n",RxData.codeFlag, RxData.code)
 
/**color 颜色编号 确认无误 RGB 123!*/
#define RED         1
#define GREEN       2
#define BLUE        3

#define Angle3_Ready      0

void Load(int number) ;


void StepArm_TaskTest(int Task_num);        //测试函数
 
void Pick_Floor(int numbr_P);             //从车上抓取物块

void StepArm_Task_InitPosition();     //初始化位置

void StepArm_Task_ScanCode(); //扫码时调整机械臂方便扫码

void StepArm_Task_Pan(unsigned char* arr1) ;   //原料区
void StepArm_Task_Jiagon(unsigned char* arr2); //粗加工区

void StepArm_Task_Over(unsigned char* arr3);//细加工区


#endif