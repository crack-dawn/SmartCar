#include "RobTask.h"
#include "StepMotor.h"//驱动
#include "logic.h"//逻辑状态
#include "my_uart.h"//串口信息

#define  Abs(x)   ((x) < 0 ? -(x) : (x))  //求绝对值 
unsigned char arr[3]={1,2,3};

void StepArm_TaskTest(int Task_num)
{
     if (Task_num == Base) 
     {
          StepArm_Task_Pan(arr);
     }
}


void StepArm_Task_ScanCode() //扫码时调整机械臂方便扫码
{
     
     StepMotor_Set_AnglePulse(0,0,Angle3_Ready);
     StepMotor_Drive(1,300);//250
     while (OVER==Flag_doing);
     
     while (1)
     {
          StepMotor_Set_AnglePulse(20,0,Angle3_Ready);
          StepMotor_Drive(1,750);
          while (OVER==Flag_doing)
          {
               if(RxData.codeFlag == codeOK)
                    break;
          }
          if(RxData.codeFlag == codeOK)
               break;


          StepMotor_Set_AnglePulse(-20,0,Angle3_Ready);
          StepMotor_Drive(1,750);
          while (OVER==Flag_doing);
          {
               if(RxData.codeFlag == codeOK)
                    break;
          }
          if(RxData.codeFlag == codeOK)
               break;

     }

     StepMotor_Set_AnglePulse(0,0,Angle3_Ready);
     StepMotor_Drive(1,300);
     while (OVER==Flag_doing);

     
}

/** 死动作 装车装物块  卸车拿物块*/

void Load(int number)         //从初始位置的小臂90放物块
{
     ServoClaw(Close);
     HAL_Delay(600);
     
     StepMotor_Set_AnglePulse(0,0,Angle3_Ready);
     StepMotor_Drive(1,250);
     while (OVER==Flag_doing);

     if(number==RED)
     {
          //StepMotor_Set_TarPulse(-1450,950,200); 
     //     StepMotor_Set_AbsPulse(-1450,950,200);//将物块从车上放下，
         StepMotor_Set_AnglePulse(-16.5,9.5,OriginalAngle3+ 8);
         StepMotor_Drive(1,400);
         while (OVER==Flag_doing);
         ServoClaw(OpenCar);
         HAL_Delay(200);
         StepMotor_Set_AnglePulse(-16.5,0,Angle3_Ready);
         StepMotor_Drive(1,400);
         while (OVER==Flag_doing);
     }

     if(number==GREEN)
     {
     //     StepMotor_Set_AbsPulse(0,900,200); 
         StepMotor_Set_AnglePulse(0,7.5,OriginalAngle3+6);
         StepMotor_Drive(1,400);
         while (OVER==Flag_doing);
         ServoClaw(OpenCar);
         HAL_Delay(200);
         StepMotor_Set_AnglePulse(0,0,Angle3_Ready);
         StepMotor_Drive(1,400);

         while (OVER==Flag_doing);
     }

     if(number==BLUE)
     {
     //     StepMotor_Set_AbsPulse(1450,900,200); 
          StepMotor_Set_AnglePulse(16.5,9.5,OriginalAngle3+ 8);
         StepMotor_Drive(1,400);
         while (OVER==Flag_doing);
         ServoClaw(OpenCar);
         HAL_Delay(200);
         StepMotor_Set_AnglePulse(16.5,0,Angle3_Ready);
         StepMotor_Drive(1,400);
         while (OVER==Flag_doing);
     }

     ServoClaw(OpenSide);
     HAL_Delay(200);
}
void Pick_Floor(int numbr_P)             //从车上抓取物块至初始位置的小臂90°
{

     StepMotor_Set_AnglePulse(0,0,Angle3_Ready);
     StepMotor_Drive(1,250);
     while (OVER==Flag_doing);

     ServoClaw(OpenCar);
     HAL_Delay(200);

     if(numbr_P==RED)
     {
         StepMotor_Set_AnglePulse(-16.5,0,OriginalAngle3+3.5);
         StepMotor_Drive(1,450);
         while (OVER==Flag_doing);
         StepMotor_Set_AnglePulse(-16.5,9.5,OriginalAngle3+3.5);
         StepMotor_Drive(1,450);
         while (OVER==Flag_doing);

         ServoClaw(Close);
         HAL_Delay(200);

         StepMotor_Set_AnglePulse(-16.5,0,Angle3_Ready);
         StepMotor_Drive(1,450);
         while (OVER==Flag_doing);
     }

     if(numbr_P==GREEN)
     {
         StepMotor_Set_AnglePulse(0,0,OriginalAngle3+3.5);
         StepMotor_Drive(1,450);
         while (OVER==Flag_doing);
         StepMotor_Set_AnglePulse(0,9.5,OriginalAngle3+3.5);
         StepMotor_Drive(1,450);
         while (OVER==Flag_doing);

         ServoClaw(Close);
         HAL_Delay(200);

         StepMotor_Set_AnglePulse(0,0,Angle3_Ready);
         StepMotor_Drive(1,450);
         while (OVER==Flag_doing);
     }

     if(numbr_P==BLUE)
     {
          StepMotor_Set_AnglePulse(16.5,0,OriginalAngle3+3.5);
         StepMotor_Drive(1,450);
         while (OVER==Flag_doing);
         StepMotor_Set_AnglePulse(16.5,9.5,OriginalAngle3+ 3.5);
         StepMotor_Drive(1,450);
         while (OVER==Flag_doing);

         ServoClaw(Close);
         HAL_Delay(200);

         StepMotor_Set_AnglePulse(16.5,0,Angle3_Ready);
         StepMotor_Drive(1,450);
         while (OVER==Flag_doing);
     }

}

/**---------------------------*/


void StepArm_Task_Pan(unsigned char* arr1)      //从圆盘上抓取物块至放完物块
{
     for(int i=0;i<3;i++)
     {
          StepMotor_Set_AnglePulse(0,0,Angle3_Ready);
          StepMotor_Drive(1,300);
          while (OVER == Flag_doing);//

          ServoClaw(OpenPan);
          HAL_Delay(100);
          

          // StepMotor_Set_AnglePulse(90,0,74.5);
          StepMotor_Set_AnglePulse(90,0,Angle3_Ready);
          StepMotor_Drive(1,200);
          while (OVER == Flag_doing);//到固定抓取位置

          //清除数据
          ClearRxData;                         
          RxData.B_dis[arr1[i]]+=255;
          RxData.B_ang[arr1[i]]=179;
          
          while(1)// 物料颜色判断
          {
               DebugB;
               if(  (RxData.B_ang[arr1[i]]>40 &&RxData.B_ang[arr1[i]]<140)  ||  (RxData.B_ang[arr1[i]]>220&RxData.B_ang[arr1[i]]<320)  )
               {
                    break;
               } 
          } 

         while(1)
         {     
               if(Abs(RxData.B_ang[arr1[i]]-90) > 10)
               {
                    while(1)// 物料颜色判断
                    {

                         if(  (RxData.B_ang[arr1[i]]>40 &&RxData.B_ang[arr1[i]]<140)  ||  (RxData.B_ang[arr1[i]]>220&RxData.B_ang[arr1[i]]<320)  )
                         {
                              break;
                         } 
                    } 
                    while (1)
                    {   
                         
                         HAL_Delay(2);
                         StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu+RxData.B_ang[arr1[i]]-90,Motor_2.Pulse*Bu,Angle3_Ready);
                         StepMotor_Drive(StepALL_START, 900);
                         
                         while (OVER == Flag_doing ) 
                         {
                              if  ( Abs(RxData.B_ang[arr1[i]]-90) <= 4  )
                              {
                                        StepMotor_Drive(StepALL_STOP, 0);
                                        break; 
                              }
                         }

                         if  ( Abs(RxData.B_ang[arr1[i]]-90) <= 7  )
                         {
                                   break ;
                         }
                    }
                    RxData.B_ang[arr1[i]]=90;     
               }

               StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,Motor_2.Pulse*Bu,Angle3_Ready);
               StepMotor_Drive(1,250);
               while (OVER == Flag_doing);
               
               StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,50,Angle3_Ready);
               StepMotor_Drive(1,900);
               RxData.B_dis[arr1[i]]+=700;
               while(OVER == Flag_doing)
               {
                    if  ( Abs(RxData.B_dis[arr1[i]]) <= 50  )
                    {
                         Motor_2.pwmPulse=0; Motor_2.Flag=Flag_finish; Motor_2.tarPulse=0;
                         break; 
                    }
               }
               if(Motor_2.Pulse*Bu>30)
               {
                    StepMotor_Set_AnglePulse(90,0,Angle3_Ready);
                    StepMotor_Drive(1,250);
                    while (OVER == Flag_doing);
                    ClearRxData;   
               }
               

          
         }

          
          Load(arr1[i]);  
     }
     ServoClaw(OpenCar);
}

void StepArm_Task_Jiagon(unsigned char* arr2)     //粗加工区 先放 后装车 然后走
{
     
     for(int i=0;i<3;i++)
     {    
          
          //卸货
          Pick_Floor(arr2[i]);          
          ServoTurn(0);
          HAL_Delay(600);
          
          //小臂抬到90°
          StepMotor_Set_AnglePulse(0,0,Angle3_Ready);
          StepMotor_Drive(1,300);
          while (OVER == Flag_doing);

          //先到达各个圆环的预定位置
          if(arr2[i]==GREEN)
          {
               StepMotor_Set_AnglePulse(45,0,Angle3_Ready);
               StepMotor_Drive(1,200);
               while (OVER == Flag_doing);
          }

          else if (arr2[i]==RED)
          {
               StepMotor_Set_AnglePulse(90,0,Angle3_Ready);
               StepMotor_Drive(1,200);
               while (OVER == Flag_doing);
          }

          else if (arr2[i]==BLUE)
          {
               StepMotor_Set_AnglePulse(120,0,Angle3_Ready);
               StepMotor_Drive(1,200);
               while (OVER == Flag_doing);
          }

          //清除数据
          ClearRxData;                        
          RxData.C_dis[arr2[i]]+=700;
          RxData.C_ang[arr2[i]]=179;

          while (1)
          {   
               
               HAL_Delay(2);
               StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu+RxData.C_ang[arr2[i]]-90,0,Angle3_Ready);
               StepMotor_Drive(StepALL_START, 250);
			
               while (OVER == Flag_doing ) 
               {
                    if  ( Abs(RxData.C_ang[arr2[i]]-90) <= 4  )
                    {
                              StepMotor_Drive(StepALL_STOP, 0);
                              break; 
                    }
               }

               if  ( Abs(RxData.C_ang[arr2[i]]-90) <= 7  )
               {
                         break ;
               }
          }

          StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,0,OriginalAngle3);
          StepMotor_Drive(1,250);
          while (OVER == Flag_doing);
          
          StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,30,OriginalAngle3);
          StepMotor_Drive(1,350);
          RxData.C_dis[arr2[i]]+=700;
          while(OVER == Flag_doing)
          {
            if  ( Abs(RxData.C_dis[arr2[i]]) <= 50  )
            {
               Motor_2.pwmPulse=0; Motor_2.Flag=Flag_finish; Motor_2.tarPulse=0;
                break; 
            }
          }


          //舵机转向准备抓取
          ServoTurn(90);
          HAL_Delay(1000);
          
          //垂直下降
          if(arr2[i]==BLUE)
          {
               StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,Motor_2.Pulse*Bu+23.25,OriginalAngle3-16);
               StepMotor_Drive(1,300);
               while (OVER == Flag_doing);
          }

          else if (arr2[i]==RED)
          {
               StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,Motor_2.Pulse*Bu+23.5,OriginalAngle3-15.5);
               StepMotor_Drive(1,300);
               while (OVER == Flag_doing);
          }

          else if (arr2[i]==GREEN)
          {
               StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,Motor_2.Pulse*Bu+23.5,OriginalAngle3-16);
               StepMotor_Drive(1,300);
               while (OVER == Flag_doing);
          }

          ServoClaw(OpenSide);
          HAL_Delay(300);

          //垂直上升
          StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,Motor_2.Pulse*Bu-15,OriginalAngle3+10);
          StepMotor_Drive(1,250);
          while (OVER == Flag_doing);

          StepMotor_Set_AnglePulse(0,0,Angle3_Ready);
          StepMotor_Drive(1,250);
          while (OVER == Flag_doing);
     }

     for (int i = 0; i < 3; i++)
     {
          //准备阶段
          
          StepMotor_Set_AnglePulse(0,0,Angle3_Ready);
          StepMotor_Drive(1,300);
          while (OVER == Flag_doing);
          

          //先到达各个圆环的预定位置
          if(arr2[i]==GREEN)
          {
               StepMotor_Set_AnglePulse(45,0,Angle3_Ready);
               StepMotor_Drive(1,200);
               while (OVER == Flag_doing);
          }

          else if (arr2[i]==RED)
          {
               StepMotor_Set_AnglePulse(90,0,Angle3_Ready);
               StepMotor_Drive(1,200);
               while (OVER == Flag_doing);
          }

          else if (arr2[i]==BLUE)
          {
               StepMotor_Set_AnglePulse(120,0,Angle3_Ready);
               StepMotor_Drive(1,200);
               while (OVER == Flag_doing);
          }

          //转向和距离的逼近
          ClearRxData;                     //清除数据
          RxData.B_dis[arr2[i]]+=700;
          RxData.B_ang[arr2[i]]=179;



           while (1)
          {   
               
               HAL_Delay(2);
               StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu+RxData.B_ang[arr2[i]]-90,0,Angle3_Ready);
               StepMotor_Drive(StepALL_START, 300);
			
               while (OVER == Flag_doing ) 
               {
                    if  ( Abs(RxData.B_ang[arr2[i]]-90) <= 4  )
                    {
                              StepMotor_Drive(StepALL_STOP, 0);
                              break; 
                    }
               }

               if  ( Abs(RxData.B_ang[arr2[i]]-90) <= 7  )
               {
                         break ;
               }
          }


          StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,0,OriginalAngle3+10);
          StepMotor_Drive(1,300);
          while (OVER == Flag_doing);
          
          StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,30,OriginalAngle3);
          StepMotor_Drive(1,350);
          RxData.B_dis[arr2[i]]+=700;
          while(OVER == Flag_doing)
          {
            if  ( Abs(RxData.B_dis[arr2[i]]) <= 50  )
            {
               Motor_2.Flag=Flag_finish; 
                break; 
            }
          }


          //垂直下降
          StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,Motor_2.Pulse*Bu+23,OriginalAngle3-14.5);
          StepMotor_Drive(1,300);
          while (OVER == Flag_doing);

          //抓取
          Load(arr2[i]);
     }
     
}

void StepArm_Task_Over(unsigned char* arr3)   //细加工区 放置物块 然后走
{
     for(int i=0;i<3;i++)
     {    
          //卸货
          Pick_Floor(arr3[i]);          
          ServoTurn(0);
          HAL_Delay(600);

          StepMotor_Set_AnglePulse(0,0,Angle3_Ready);
          StepMotor_Drive(1,300);
          while (OVER == Flag_doing);

          //先到达各个圆环的预定位置
          if(arr3[i]==GREEN)
          {
               StepMotor_Set_AnglePulse(45,0,Angle3_Ready);
               StepMotor_Drive(1,200);
               while (OVER == Flag_doing);
          }

          else if (arr3[i]==RED)
          {
               StepMotor_Set_AnglePulse(90,0,Angle3_Ready);
               StepMotor_Drive(1,200);
               while (OVER == Flag_doing);
          }

          else if (arr3[i]==BLUE)
          {
               StepMotor_Set_AnglePulse(120,0,Angle3_Ready);
               StepMotor_Drive(1,200);
               while (OVER == Flag_doing);
          }


          ClearRxData;                        //清除数据
          RxData.C_dis[arr3[i]]+=700;
          RxData.C_ang[arr3[i]]=179;

          //转向到指定位置
          while (1)
          {   
               
               HAL_Delay(2);
               StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu+RxData.C_ang[arr3[i]]-90,0,Angle3_Ready);
               StepMotor_Drive(StepALL_START, 250);
			
               while (OVER == Flag_doing ) 
               {
                    if  ( Abs(RxData.C_ang[arr3[i]]-90) <= 4  )
                    {
                              StepMotor_Drive(StepALL_STOP, 0);
                              break; 
                    }
               }

               if  ( Abs(RxData.C_ang[arr3[i]]-90) <= 7  )
               {
                         break ;
               }
          }


          StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,0,OriginalAngle3);
          StepMotor_Drive(1,250);
          while (OVER == Flag_doing);
          
          StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,30,OriginalAngle3);
          StepMotor_Drive(1,250);
          RxData.C_dis[arr3[i]]+=700;
          while(OVER == Flag_doing)
          {
            if  ( Abs(RxData.C_dis[arr3[i]]) <= 50  )
            {
               Motor_2.pwmPulse=0; Motor_2.Flag=Flag_finish; Motor_2.tarPulse=0;
                break; 
            }
          }



          //舵机转向准备抓取
          ServoTurn(90);
          HAL_Delay(600);
          
          //垂直下落
          if(arr3[i]==BLUE)
          {
               StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,Motor_2.Pulse*Bu+23.25,OriginalAngle3-16);
               StepMotor_Drive(1,300);
               while (OVER == Flag_doing);
          }

          else if (arr3[i]==RED)
          {
               StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,Motor_2.Pulse*Bu+23.5,OriginalAngle3-16.5);
               StepMotor_Drive(1,300);
               while (OVER == Flag_doing);
          }

          else if (arr3[i]==GREEN)
          {
               StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,Motor_2.Pulse*Bu+23,OriginalAngle3-16);
               StepMotor_Drive(1,300);
               while (OVER == Flag_doing);
          }
          

          ServoClaw(OpenSide);
          HAL_Delay(300);

          //垂直上升
          StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,Motor_2.Pulse*Bu-15,OriginalAngle3+10);
          StepMotor_Drive(1,300);
          while (OVER == Flag_doing);

          StepMotor_Set_AnglePulse(0,0,Angle3_Ready);
          StepMotor_Drive(1,250);
          while (OVER == Flag_doing);
     }

}
