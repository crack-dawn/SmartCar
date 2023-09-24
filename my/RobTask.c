#include "RobTask.h"
#include "StepMotor.h"//驱动
#include "logic.h"//逻辑状态
#include "my_uart.h"//串口信息

#define  Abs(x)   ((x) < 0 ? -(x) : (x))  //求绝对值 
unsigned char arr[3]={1,2,3};

int RoTask_Speed[16]={0,100,200,300,400,500,600,700,800,900,1000,1100,1200,1300,1400,1500};

 
void StepArm_TaskTest(int Task_num)
{
     if (Task_num == Base) 
     {    
          StepMotor_Set_AnglePulse(90,30,10);
          StepMotor_Drive(1,RoTask_Speed[6]);
          while (OVER == Flag_doing);
     }
}


void StepArm_Task_ScanCode() //扫码时调整机械臂方便扫码
{
     
     StepMotor_Set_AnglePulse(0,0,Angle3_Ready);
     StepMotor_Drive(1,600);
     while (OVER==Flag_doing);
     
     while (1)
     {
          StepMotor_Set_AnglePulse(20,0,Angle3_Ready);
          StepMotor_Drive(1,1000);
          while (OVER==Flag_doing)
          {
               if(RxData.codeFlag == codeOK)
                    break;
          }
          if(RxData.codeFlag == codeOK)
               break;


          StepMotor_Set_AnglePulse(-20,0,Angle3_Ready);
          StepMotor_Drive(1,1000);
          while (OVER==Flag_doing);
          {
               if(RxData.codeFlag == codeOK)
                    break;
          }
          if(RxData.codeFlag == codeOK)
               break;

     }
     StepMotor_Set_AnglePulse(0,0,Angle3_Ready);
     StepMotor_Drive(1,600);
     while (OVER==Flag_doing);

     
}

/** 死动作 装车装物块  卸车拿物块*/
void Load(int number)         //从初始位置的小臂90放物块
{    
     StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,0,Angle3_Ready);  //转到准备上升以免打到其他物块
     StepMotor_Drive(1,RoTask_Speed[6]);
     while (OVER == Flag_doing);

     StepMotor_Set_AnglePulse(-10,0,Angle3_Ready-5);  //转到复位
     StepMotor_Drive(1,RoTask_Speed[6]);
     while (OVER == Flag_doing);

     StepArm_Task_ReInitPosition(3000,-3000,3000);

     if(number==RED)
     {
          StepMotor_Set_AnglePulse(-21,26,10);  //转到准备转舵机的位置
          StepMotor_Drive(1,RoTask_Speed[6]);
          while (OVER == Flag_doing);

          ServoTurn(-80);    //转到-90°
          HAL_Delay(600);

          StepMotor_Set_AnglePulse(-21,58,OriginalAngle3+3);  //机械臂到放物块的位置
          StepMotor_Drive(1,RoTask_Speed[8]);
          while (OVER == Flag_doing);

          ServoClaw(OpenCar);    //放置
          HAL_Delay(200);
          HAL_Delay(1000);
          StepMotor_Set_AnglePulse(-21,26,10);  //转到准备转舵机的位置
          StepMotor_Drive(1,RoTask_Speed[6]);
          while (OVER == Flag_doing);

          ServoTurn(90);           //舵机转正
          HAL_Delay(600);


          StepMotor_Set_AnglePulse(0,0,Angle3_Ready);  //转到准备阶段
          StepMotor_Drive(1,RoTask_Speed[6]);
          while (OVER == Flag_doing);
     }

     if(number==GREEN)
     {
          StepMotor_Set_AnglePulse(3.5,26,10);  //转到准备转舵机的位置
          StepMotor_Drive(1,RoTask_Speed[6]);
          while (OVER == Flag_doing);
          
          ServoTurn(-80);    //转到-90°
          HAL_Delay(600);
     
          StepMotor_Set_AnglePulse(3.5,50,OriginalAngle3+3);  //机械臂到放物块的位置
          StepMotor_Drive(1,RoTask_Speed[8]);
          while (OVER == Flag_doing);

          ServoClaw(OpenCar);    //放置
          HAL_Delay(200);

          StepMotor_Set_AnglePulse(3.5,34,5);  //防止打到物块
          StepMotor_Drive(1,RoTask_Speed[8]);
          while (OVER == Flag_doing);

          StepMotor_Set_AnglePulse(5,26,10);  //转到准备转舵机的位置
          StepMotor_Drive(1,RoTask_Speed[8]);
          while (OVER == Flag_doing);

          ServoTurn(90);           //舵机转正
          HAL_Delay(600);

          StepMotor_Set_AnglePulse(0,0,Angle3_Ready);  //转到准备阶段
          StepMotor_Drive(1,RoTask_Speed[6]);
          while (OVER == Flag_doing);

     }

     if(number==BLUE)
     {
           StepMotor_Set_AnglePulse(36,26,10);  //转到准备转舵机的位置//23
          StepMotor_Drive(1,RoTask_Speed[6]);
          while (OVER == Flag_doing);

          ServoTurn(-60);    //转到-60°
          HAL_Delay(600);

          StepMotor_Set_AnglePulse(36,52,OriginalAngle3+2.5);  //机械臂到放物块的位置
          StepMotor_Drive(1,RoTask_Speed[8]);
          while (OVER == Flag_doing);

          ServoClaw(OpenCar);    //放置
          HAL_Delay(200);

          StepMotor_Set_AnglePulse(36,33,10);  //防止打到物块
          StepMotor_Drive(1,RoTask_Speed[6]);
          while (OVER == Flag_doing);

          StepMotor_Set_AnglePulse(36,26,10);  //转到准备转舵机的位置
          StepMotor_Drive(1,RoTask_Speed[6]);
          while (OVER == Flag_doing);

          ServoTurn(90);           //舵机转正
          HAL_Delay(600);

          StepMotor_Set_AnglePulse(0,0,Angle3_Ready);  //转到准备阶段
          StepMotor_Drive(1,RoTask_Speed[6]);
          while (OVER == Flag_doing);
     }

     ServoClaw(OpenSide);
     HAL_Delay(200);
}
void Pick_Floor(int numbr_P)             //从车上抓取物块至初始位置的小臂90°
{
     StepMotor_Set_AnglePulse(-10,0,Angle3_Ready-5);  //转到准备转舵机的位置
     StepMotor_Drive(1,RoTask_Speed[6]);
     while (OVER == Flag_doing);
     StepArm_Task_ReInitPosition(3000,-3000,3000);


     if(numbr_P==RED)
     {
          StepMotor_Set_AnglePulse(-19,26,10);  //转到准备转舵机的位置
          StepMotor_Drive(1,RoTask_Speed[6]);
          while (OVER == Flag_doing);
          
          ServoClaw(OpenCar);  //舵机爪子进入卸货角度
          HAL_Delay(200);
          
          ServoTurn(-80);    //转到-90°
          HAL_Delay(600);

          StepMotor_Set_AnglePulse(-19,44,14);  //转到准备转舵机的位置
          StepMotor_Drive(1,RoTask_Speed[6]);
          while (OVER == Flag_doing);
                    
          StepMotor_Set_AnglePulse(-19,58,23);  //机械臂到放物块的位置
          StepMotor_Drive(1,RoTask_Speed[8]);
          while (OVER == Flag_doing);
          
          StepMotor_Set_AnglePulse(-19,54,OriginalAngle3+5);  //机械臂到放物块的位置
          StepMotor_Drive(1,RoTask_Speed[8]);
          while (OVER == Flag_doing);

          ServoClaw(Close);    //放置
          HAL_Delay(200);

          StepMotor_Set_AnglePulse(-19,26,10);  //转到准备转舵机的位置
          StepMotor_Drive(1,RoTask_Speed[5]);
          while (OVER == Flag_doing);
     }

     if(numbr_P==GREEN)
     {
          StepMotor_Set_AnglePulse(3.5,26,10);  //转到准备转舵机的位置
          StepMotor_Drive(1,RoTask_Speed[6]);
          while (OVER == Flag_doing);
          
          ServoClaw(OpenCar);  //舵机爪子进入卸货角度
          HAL_Delay(200);
          
          ServoTurn(-80);    //转到-90°
          HAL_Delay(600);

          StepMotor_Set_AnglePulse(3.5,40,14);  //防止
          StepMotor_Drive(1,RoTask_Speed[8]);
          while (OVER == Flag_doing);

          StepMotor_Set_AnglePulse(3.5,50,28);  //防止
          StepMotor_Drive(1,RoTask_Speed[8]);
          while (OVER == Flag_doing);
          
          StepMotor_Set_AnglePulse(3.5,42,OriginalAngle3+4);  //物块位置
          StepMotor_Drive(1,RoTask_Speed[8]);
          while (OVER == Flag_doing);

          ServoClaw(Close);    //放置
          HAL_Delay(200);

          StepMotor_Set_AnglePulse(3.5,26,10);  //转到准备转舵机的位置
          StepMotor_Drive(1,RoTask_Speed[5]);
          while (OVER == Flag_doing);
     }

     if(numbr_P==BLUE)
     {
          StepMotor_Set_AnglePulse(37,26,10);  //转到准备转舵机的位置//23
          StepMotor_Drive(1,RoTask_Speed[6]);
          while (OVER == Flag_doing);

          ServoClaw(OpenCar);
          HAL_Delay(200);
          ServoTurn(-60);    //转到-60°
          HAL_Delay(600);

          

          StepMotor_Set_AnglePulse(37,46,OriginalAngle3-8);  //机械臂到放物块的位置
          StepMotor_Drive(1,RoTask_Speed[8]);
          while (OVER == Flag_doing);

          StepMotor_Set_AnglePulse(37,56,OriginalAngle3+3);  //机械臂到放物块的位置
          StepMotor_Drive(1,RoTask_Speed[8]);
          while (OVER == Flag_doing);

          ServoClaw(Close);    //抓取
          HAL_Delay(200);

          StepMotor_Set_AnglePulse(37,26,10);  //转到准备转舵机的位置
          StepMotor_Drive(1,RoTask_Speed[6]);
          while (OVER == Flag_doing);
   
     }
}

/**---------------------------*/


void StepArm_Task_Pan(unsigned char* arr1)      //从圆盘上抓取物块至放完物块
{
     SendCmdB;
     HAL_Delay(100);
     for(int i=0;i<3;i++)
     {    
          int flag_pan=0;
          //准备阶段 
          StepMotor_Set_AnglePulse(0,0,Angle3_Ready);
          StepMotor_Drive(1,RoTask_Speed[6]);
          while (OVER == Flag_doing);

          //抓子张开大小为盘的状态
          ServoClaw(OpenPan);
          HAL_Delay(100);

          //因为摄像头中心点有问题所以就偏一点点
          ServoTurn(TurnPan);
          HAL_Delay(100);

          Back:         //这个下面准备抓的时候判断静止的，回调到这
          Back1:        //这个是下面如果追踪时，大臂超过45的回调到这
          back2:        //这个是下面如果转动有问题，就回到这
          ServoClaw(OpenPan);
          StepMotor_Set_AnglePulse(110,0,Angle3_Ready);    //转到准备抓的位置
          StepMotor_Drive(1,RoTask_Speed[5]);
          while (OVER == Flag_doing);//到固定抓取位置


         
          //清除数据
          ClearRxData;                           
          RxData.B_dis[arr1[i]]+=255;
          RxData.B_ang[arr1[i]]=179;

          StartTimeCnt(12);   //设定超时判断
          while(1)// 物料颜色判断
          {    
               DebugB;
               HAL_Delay(5);
               if(  (RxData.B_ang[arr1[i]]>5 &&RxData.B_ang[arr1[i]]<175)  ||  (RxData.B_ang[arr1[i]]>190&RxData.B_ang[arr1[i]]<350)  )
               {
                    ClearTimeFlag();
                    break;
               } 

               if(ReadTimeFlag()==1)
               {
                    flag_pan=1;
                    break;
               }  
          } 
          if(flag_pan==1)
          {
               continue;
          }
          HAL_Delay(300);

          //静止判断跳出
          while (1)
          {
               
               if(RxData.B_Vector[arr1[i]]<=1&&RxData.B_Vector[arr1[i]]>=-1)          
               {
                    printf("jingzhi:%d %d\r\n",RxData.B_Accume[arr1[i]],RxData.B_Vector[arr1[i]]);
                    break;
               }     
               else              //运动
               {
                    printf("normal:%d %d\r\n",RxData.B_Accume[arr1[i]],RxData.B_Vector[arr1[i]]);
               }
               
               HAL_Delay(10);
          }              

          //水平的调节
          while (1)
          {   
               
               HAL_Delay(2);
               StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu+RxData.B_ang[arr1[i]]-90,Motor_2.Pulse*Bu,Angle3_Ready);
               StepMotor_Drive(StepALL_START, RoTask_Speed[6]);
               
               while (OVER == Flag_doing ) 
               {
                    if  ( Abs(RxData.B_ang[arr1[i]]-90) <= 4  )
                    {
                              StepMotor_Drive(StepALL_STOP, 0);
                              break; 
                    }

                    if(Motor_1.Pulse*Bu>150||Motor_1.Pulse*Bu<0)          //如果数据使他大于175就放弃动作
                    {
                         goto back2;
                    }
               }


               if  ( Abs(RxData.B_ang[arr1[i]]-90) <= 7  )
               {
                    break;
               }
               
          }    

          //距离调节
          StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,50,Angle3_Ready);
          StepMotor_Drive(1,RoTask_Speed[6]);
          RxData.B_dis[arr1[i]]+=700;
          while(OVER == Flag_doing)
          {
               if( Abs(RxData.B_dis[arr1[i]]) <= 50 )
               {
                    Motor_2.pwmPulse=0; Motor_2.Flag=Flag_finish; Motor_2.tarPulse=0;
                    break; 
               }

               if(Motor_2.Pulse*Bu>45)
               {
                    Motor_2.pwmPulse=0; Motor_2.Flag=Flag_finish; Motor_2.tarPulse=0;
                    goto Back1;
               }
          }

               //归为到起始抓的位置，等待直到出现目标物块的颜色
               // if(Pan_Home=1)
               // {

               //      // while(1)// 物料颜色判断
               //      // {
               //      //      DebugB;
               //      //      if(  (RxData.B_ang[arr1[i]]>40 &&RxData.B_ang[arr1[i]]<140)  ||  (RxData.B_ang[arr1[i]]>220&RxData.B_ang[arr1[i]]<320)  )
               //      //      {
               //      //           break;
               //      //      } 
               //      // }
               //      Pan_Home=0; 
               // }
     

          Angle_vertical(25,Calculate_DisHorizon()+8,1);    //垂直下降
          ServoClaw(Close);               
          HAL_Delay(300);

          int key=0;           
          while (1)
          {
               HAL_Delay(20);
               if(RxData.B_Vector[arr1[i]]<=10&&RxData.B_Vector[arr1[i]]>=-10)
               {
                    printf("jingzhi:%d %d\r\n",RxData.B_Accume[arr1[i]],RxData.B_Vector[arr1[i]]);
                    break;
               }     
               else
               {    
                    key++;            //如果这么多次判断都是运动则反馈回去
                    printf("normal:%d %d\r\n",RxData.B_Accume[arr1[i]],RxData.B_Vector[arr1[i]]);
                    if(key==20)
                         goto Back;
               }
          }
          
          ServoClaw(Close);
          HAL_Delay(300);

          Load(arr1[i]);  
     }
     ServoClaw(OpenCar);
}

void StepArm_Task_Jiagon(unsigned char* arr2)     //粗加工区 先放 后装车 然后走
{
     SendCmdC;      
     HAL_Delay(300);
     for(int i=0;i<3;i++)
     {    
          int flag_jiagon=0;
          //卸货
          Pick_Floor(arr2[i]);          
          ServoTurn(180);           //舵机转到180°准备识别圆环
          HAL_Delay(500);
          
          //小臂抬到90°
          StepMotor_Set_AnglePulse(0,0,Angle3_Ready);
          StepMotor_Drive(1,RoTask_Speed[6]);
          while (OVER == Flag_doing);

          Jiagon1:
          //先到达各个圆环的预定位置
          StepMotor_Set_AnglePulse(55,0,Angle3_Ready);
          StepMotor_Drive(1,RoTask_Speed[6]);
          while (OVER == Flag_doing);

          //清除数据
          ClearRxData;   
          RxData.C_dis[arr2[i]]+=600;
          RxData.C_ang[arr2[i]]=179;           
          HAL_Delay(400);

          if(Abs(RxData.C_dis[arr2[i]]) >=500)
          {
               StepMotor_Set_AnglePulse(95,0,Angle3_Ready);
               StepMotor_Drive(1,RoTask_Speed[5]);
               while (OVER == Flag_doing)
               {
                    if  ( Abs(RxData.C_ang[arr2[i]]-90) <= 4  )
                    {
                              StepMotor_Drive(StepALL_STOP, 0);
                              break; 
                    }
               }            
          }
          
          HAL_Delay(400);

          StartTimeCnt(12);
          while(1)// 物料颜色判断
          {
               DebugB;
               if(  (RxData.C_ang[arr2[i]]>5 &&RxData.C_ang[arr2[i]]<177)  ||  (RxData.C_ang[arr2[i]]>183&&RxData.C_ang[arr2[i]]<358)  )
               {
                    ClearTimeFlag();
                    break;
               } 

               if(ReadTimeFlag()==1)
               {
                    flag_jiagon=1;
                    break;
               }
          } 

          if(flag_jiagon==1)
               continue;

          //转向逼近
          while (1)
          {   
               DebugC;
               HAL_Delay(2);
               StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu+RxData.C_ang[arr2[i]]-90,0,Angle3_Ready);
               StepMotor_Drive(1, RoTask_Speed[9]);
			
               while (OVER == Flag_doing ) 
               {
                    DebugC;
                    if  ( Abs(RxData.C_ang[arr2[i]]-90) <=4 )
                    {
                              StepMotor_Drive(StepALL_STOP, 0);
                              break; 
                    }
                    if(Motor_1.Pulse*Bu>150||Motor_1.Pulse*Bu<0)
                    {
                         goto Jiagon1;
                    }
               }

               if  ( Abs(RxData.C_ang[arr2[i]]-90) <= 7  )
               {
                    break ;
               }
          }
          
          //前后
          StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,0,Angle3_Ready);
          StepMotor_Drive(1,RoTask_Speed[6]);
          while (OVER == Flag_doing);
          

          StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,40,Angle3_Ready);
          StepMotor_Drive(1,RoTask_Speed[8]);
          RxData.C_dis[arr2[i]]+=700;
          while(OVER == Flag_doing)
          {
            if ( Abs(RxData.C_dis[arr2[i]]) <= 50  )
            {
               Motor_2.pwmPulse=0; Motor_2.Flag=Flag_finish; Motor_2.tarPulse=0;
                break; 
            }
          }

          Angle_vertical(10,Calculate_DisHorizon(),2);
          //第二次水平
          HAL_Delay(200);
          while (1)
          {   
               HAL_Delay(2);
               StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu+RxData.C_ang[arr2[i]]-90,Motor_2.Pulse*Bu,(Motor_3.Pulse)*Bu+OriginalAngle3);
               StepMotor_Drive(1, RoTask_Speed[8]);
			
               while (OVER == Flag_doing ) 
               {
                    if  ( Abs(RxData.C_ang[arr2[i]]-90) <= 4 )
                    {
                         StepMotor_Drive(StepALL_STOP, 0);
                         break; 
                    }

                    if(Motor_1.Pulse*Bu>150||Motor_1.Pulse*Bu<0)
                         goto Jiagon1;
               }

               if  ( Abs(RxData.C_ang[arr2[i]]-90) <= 7  )
               {
                    break ;
               }
          }

          //第二次前进
          StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,Motor_2.Pulse*Bu+5,Motor_3.Pulse*Bu+OriginalAngle3);
          StepMotor_Drive(1,RoTask_Speed[10]);
          RxData.C_dis[arr2[i]]+=700;
          while(OVER == Flag_doing)
          {
            if ( Abs(RxData.C_dis[arr2[i]]) <= 10  )
            {
               Motor_2.pwmPulse=0; Motor_2.Flag=Flag_finish; Motor_2.tarPulse=0;
                break; 
            }
          }

          //舵机转向准备放取
          ServoTurn(90);
          HAL_Delay(600);
          
          //垂直下降
          Angle_vertical(-50,Calculate_DisHorizon()-10,2);

          ServoClaw(OpenSide);
          HAL_Delay(300);

          //垂直上升（拟合的死代码）
          StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,Motor_2.Pulse*Bu-15,OriginalAngle3+10);
          StepMotor_Drive(1,RoTask_Speed[5]);
          while (OVER == Flag_doing);

          StepMotor_Set_AnglePulse(0,0,Angle3_Ready);
          StepMotor_Drive(1,RoTask_Speed[5]);
          while (OVER == Flag_doing);
     }

     SendCmdB;
     HAL_Delay(300);

     for (int i = 0; i < 3; i++)
     {
           //准备阶段
          StepMotor_Set_AnglePulse(0,0,Angle3_Ready);
          StepMotor_Drive(1,RoTask_Speed[6]);
          while (OVER == Flag_doing);
          
          jiagon_liao:
          //先到达各个圆环的预定位置
          StepMotor_Set_AnglePulse(60,0,Angle3_Ready);
          StepMotor_Drive(1,RoTask_Speed[5]);
          while (OVER == Flag_doing);

          //清除数据
          ClearRxData;                        
          
          HAL_Delay(400);
          if(Abs(RxData.B_dis[arr2[i]]) > 500)
          {
               StepMotor_Set_AnglePulse(95,0,Angle3_Ready);
               StepMotor_Drive(1,RoTask_Speed[5]);
               while (OVER == Flag_doing)
               {
                    if  ( Abs(RxData.B_ang[arr2[i]]-90) <= 4  )
                    {
                              StepMotor_Drive(StepALL_STOP, 0);
                              break; 
                    }
               }
          }


          while(1)// 物料颜色判断等待
          {
               DebugB;
               if(  (RxData.B_ang[arr2[i]]>2 &&RxData.B_ang[arr2[i]]<178)  ||  (RxData.B_ang[arr2[i]]>181&RxData.B_ang[arr2[i]]<359)  )
               {
                    break;
               } 
          }

          //角度的逼近
           while (1)
          {   
               HAL_Delay(1);
               StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu+RxData.B_ang[arr2[i]]-90,0,Angle3_Ready);
               StepMotor_Drive(StepALL_START, RoTask_Speed[7]);
               while (OVER == Flag_doing ) 
               {
                    if  ( Abs(RxData.B_ang[arr2[i]]-90) <= 4  )
                    {
                              StepMotor_Drive(StepALL_STOP, 0);
                              break; 
                    }

                    if(Motor_1.Pulse*Bu>150)
                         goto jiagon_liao;
               }

               if  ( Abs(RxData.B_ang[arr2[i]]-90) <= 7  )
               {
                    break ;
               }
          }

          //距离的逼近
          StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,0,Angle3_Ready);
          StepMotor_Drive(1,RoTask_Speed[5]);
          while (OVER == Flag_doing);
          
          StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,30,Angle3_Ready);
          StepMotor_Drive(1,RoTask_Speed[8]);
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
          Angle_vertical(-50,Calculate_DisHorizon()-5,2);

          //抓取
          ServoClaw(Close);
          HAL_Delay(300);
          Load(arr2[i]);
     }
     
}

void StepArm_Task_Over(unsigned char* arr3)   //细加工区 放置物块 然后走
{    
     SendCmdC;
     HAL_Delay(300);
     for(int i=0;i<3;i++)
     {    
          int flag_over=0;
          //卸货
          Pick_Floor(arr3[i]);          
          ServoTurn(180);           //舵机转到-10°准备识别圆环
          HAL_Delay(500);
          
          //小臂抬到90°
          StepMotor_Set_AnglePulse(0,0,Angle3_Ready);
          StepMotor_Drive(1,RoTask_Speed[6]);
          while (OVER == Flag_doing);

          Over1:
          //先到达各个圆环的预定位置
          StepMotor_Set_AnglePulse(55,0,Angle3_Ready);
          StepMotor_Drive(1,RoTask_Speed[6]);
          while (OVER == Flag_doing);

          //清除数据
          ClearRxData;   
          RxData.C_dis[arr3[i]]+=600;
          RxData.C_ang[arr3[i]]=179;           
          HAL_Delay(400);
          if(Abs(RxData.C_dis[arr3[i]]) >=500)
          {
               StepMotor_Set_AnglePulse(95,0,Angle3_Ready);
               StepMotor_Drive(1,RoTask_Speed[5]);
               while (OVER == Flag_doing)
               {
                    if  ( Abs(RxData.C_ang[arr3[i]]-90) <= 4  )
                    {
                              StepMotor_Drive(StepALL_STOP, 0);
                              break; 
                    }
               }            
          }
          
          HAL_Delay(400);

          StartTimeCnt(12);
          while(1)// 物料颜色判断
          {
               printf("%f\r\n",RxData.C_ang[arr3[i]]);
               if(  (RxData.C_ang[arr3[i]]>1 &&RxData.C_ang[arr3[i]]<178)  ||  (RxData.C_ang[arr3[i]]>183&RxData.C_ang[arr3[i]]<358)  )
               {
                    ClearTimeFlag();
                    break;
               } 
               if(ReadTimeFlag()==1)
               {
                    flag_over=1;
                    break;
               }
          } 
          if(flag_over==1)
               continue;


          //转向逼近
          while (1)
          {   
               
               HAL_Delay(2);
               StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu+RxData.C_ang[arr3[i]]-90,0,Angle3_Ready);
               StepMotor_Drive(1, RoTask_Speed[9]);
			
               while (OVER == Flag_doing ) 
               {
                    if  ( Abs(RxData.C_ang[arr3[i]]-90) <= 4  )
                    {
                              StepMotor_Drive(StepALL_STOP, 0);
                              break; 
                    }
                    if(Motor_1.Pulse>180/Bu)
                    {
                         goto Over1;
                    }
               }

               if  ( Abs(RxData.C_ang[arr3[i]]-90) <= 7  )
               {
                         break ;
               }
          }

          //水平
          StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,0,Angle3_Ready);
          StepMotor_Drive(1,RoTask_Speed[6]);
          while (OVER == Flag_doing);
          
          StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,30,Angle3_Ready);
          StepMotor_Drive(1,RoTask_Speed[8]);
          RxData.C_dis[arr3[i]]+=700;
          while(OVER == Flag_doing)
          {
            if  ( Abs(RxData.C_dis[arr3[i]]) <= 50  )
            {
               Motor_2.pwmPulse=0; Motor_2.Flag=Flag_finish; Motor_2.tarPulse=0;
                break; 
            }
          }

          Angle_vertical(10,Calculate_DisHorizon(),2);
          //第二次水平
          HAL_Delay(200);
          while (1)
          {   
               HAL_Delay(2);
               StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu+RxData.C_ang[arr3[i]]-90,Motor_2.Pulse*Bu,(Motor_3.Pulse)*Bu+OriginalAngle3);
               StepMotor_Drive(1, RoTask_Speed[10]);
			
               while (OVER == Flag_doing ) 
               {
                    if  ( Abs(RxData.C_ang[arr3[i]]-90) <= 4 )
                    {
                         StepMotor_Drive(StepALL_STOP, 0);
                         break; 
                    }

                    if(Motor_1.Pulse*Bu>150||Motor_1.Pulse*Bu<0) 
                         goto Over1;
               }

               if  ( Abs(RxData.C_ang[arr3[i]]-90) <= 7  )
               {
                         break ;
               }
          }

           //第二次前进
          StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,Motor_2.Pulse*Bu+5,Motor_3.Pulse*Bu+OriginalAngle3);
          StepMotor_Drive(1,RoTask_Speed[10]);
          RxData.C_dis[arr3[i]]+=700;
          while(OVER == Flag_doing)
          {
            if ( Abs(RxData.C_dis[arr3[i]]) <= 10  )
            {
               Motor_2.pwmPulse=0; Motor_2.Flag=Flag_finish; Motor_2.tarPulse=0;
                break; 
            }
          }

          //舵机转向准备放置
          ServoTurn(90);
          HAL_Delay(600);
          
          //垂直下落
          Angle_vertical(-50,Calculate_DisHorizon()-5,2);
          

          ServoClaw(OpenSide);
          HAL_Delay(300);

          //垂直上升
          StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,Motor_2.Pulse*Bu-15,OriginalAngle3+10);
          StepMotor_Drive(1,RoTask_Speed[6]);
          while (OVER == Flag_doing);

          StepMotor_Set_AnglePulse(0,0,Angle3_Ready);
          StepMotor_Drive(1,RoTask_Speed[6]);
          while (OVER == Flag_doing);
     }

}

void StepArm_Task_OverClay(unsigned char* arr4)   //最后码垛
{
     SendCmdB;
     HAL_Delay(300);
     for(int i=0;i<3;i++)
     {    
          int flag_Clay=0;
          //卸货
          Pick_Floor(arr4[i]);          
          ServoTurn(180);           //舵机转到-10°准备识别圆环
          HAL_Delay(500);
          
          //小臂抬到90°
          StepMotor_Set_AnglePulse(0,0,Angle3_Ready);
          StepMotor_Drive(1,RoTask_Speed[6]);
          while (OVER == Flag_doing);

          Clay1:
          //先到达各个圆环的预定位置
          StepMotor_Set_AnglePulse(55,0,Angle3_Ready);
          StepMotor_Drive(1,RoTask_Speed[6]);
          while (OVER == Flag_doing);

          //清除数据
          ClearRxData;   
          RxData.B_dis[arr4[i]]+=600;
          RxData.B_ang[arr4[i]]=179;           
          HAL_Delay(400);
          if(Abs(RxData.B_dis[arr4[i]]) >=500)
          {
               StepMotor_Set_AnglePulse(95,0,Angle3_Ready);
               StepMotor_Drive(1,RoTask_Speed[5]);
               while (OVER == Flag_doing)
               {
                    if  ( Abs(RxData.B_ang[arr4[i]]-90) <= 4  )
                    {
                              StepMotor_Drive(StepALL_STOP, 0);
                              break; 
                    }
               }     
          }

          HAL_Delay(400);

          StartTimeCnt(12);
          while(1)// 物料颜色判断
          {
               printf("%f\r\n",RxData.B_ang[arr4[i]]);
               if(  (RxData.B_ang[arr4[i]]>5 &&RxData.B_ang[arr4[i]]<177)  ||  (RxData.B_ang[arr4[i]]>183&RxData.B_ang[arr4[i]]<358)  )
               {
                    ClearTimeFlag();
                    break;
               } 
               if(ReadTimeFlag()==1)
               {
                    flag_Clay=1;
                    break;
               }
          } 
          if(flag_Clay==1)
               continue;

          //转向逼近
          while (1)
          {   
               
               HAL_Delay(2);
               StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu+RxData.B_ang[arr4[i]]-90,0,Angle3_Ready);
               StepMotor_Drive(1, RoTask_Speed[9]);
			
               while (OVER == Flag_doing ) 
               {
                    if  ( Abs(RxData.B_ang[arr4[i]]-90) <= 4  )
                    {
                              StepMotor_Drive(StepALL_STOP, 0);
                              break; 
                    }
                    if(Motor_1.Pulse>180/Bu)
                    {
                         goto Clay1;
                    }
               }

               if  ( Abs(RxData.B_ang[arr4[i]]-90) <= 7  )
               {
                         break ;
               }
          }

          //水平
          StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,0,Angle3_Ready);
          StepMotor_Drive(1,RoTask_Speed[6]);
          while (OVER == Flag_doing);
          
          StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,30,Angle3_Ready);
          StepMotor_Drive(1,RoTask_Speed[8]);
          RxData.B_dis[arr4[i]]+=700;
          while(OVER == Flag_doing)
          {
            if  ( Abs(RxData.B_dis[arr4[i]]) <= 50  )
            {
               Motor_2.pwmPulse=0; Motor_2.Flag=Flag_finish; Motor_2.tarPulse=0;
                break; 
            }
          }

          Angle_vertical(50,Calculate_DisHorizon(),2);
          //第二次水平
          HAL_Delay(200);
          while (1)
          {   
               HAL_Delay(2);
               StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu+RxData.B_ang[arr4[i]]-90,Motor_2.Pulse*Bu,(Motor_3.Pulse)*Bu+OriginalAngle3);
               StepMotor_Drive(1, RoTask_Speed[8]);
			
               while (OVER == Flag_doing ) 
               {
                    if  ( Abs(RxData.B_ang[arr4[i]]-90) <= 4 )
                    {
                         StepMotor_Drive(StepALL_STOP, 0);
                         break; 
                    }

                    if(Motor_1.Pulse*Bu>150)
                         goto Clay1;
               }

               if  ( Abs(RxData.B_ang[arr4[i]]-90) <= 7  )
               {
                         break ;
               }
          }

          //第二次前进
          StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,Motor_2.Pulse*Bu+5,Motor_3.Pulse*Bu+OriginalAngle3);
          StepMotor_Drive(1,RoTask_Speed[10]);
          RxData.C_dis[arr4[i]]+=700;
          while(OVER == Flag_doing)
          {
            if ( Abs(RxData.C_dis[arr4[i]]) <= 10  )
            {
               Motor_2.pwmPulse=0; Motor_2.Flag=Flag_finish; Motor_2.tarPulse=0;
                break; 
            }
          }
          //舵机转向准备放置
          ServoTurn(90);
          HAL_Delay(600);
          
          //垂直下落
          Angle_vertical(30,Calculate_DisHorizon()-8,2);
          

          ServoClaw(OpenSide);
          HAL_Delay(300);

          //垂直上升
          StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,Motor_2.Pulse*Bu-15,Motor_1.Pulse+OriginalAngle3+10);
          StepMotor_Drive(1,RoTask_Speed[6]);
          while (OVER == Flag_doing);

          StepMotor_Set_AnglePulse(0,0,Angle3_Ready);
          StepMotor_Drive(1,RoTask_Speed[6]);
          while (OVER == Flag_doing);
     }
}

void StepArm_Task_OverPlace(unsigned char* arr5)    //最后放在上层
{
     SendCmdC;
     HAL_Delay(300);
     for(int i=0;i<3;i++)
     {    
          int flag_Place=0;
          //卸货
          Pick_Floor(arr5[i]);          
          ServoTurn(180);           //舵机转到-10°准备识别圆环
          HAL_Delay(500);
          
          //小臂抬到90°
          StepMotor_Set_AnglePulse(0,0,Angle3_Ready);
          StepMotor_Drive(1,RoTask_Speed[6]);
          while (OVER == Flag_doing);

          Place1:
          //先到达各个圆环的预定位置
          StepMotor_Set_AnglePulse(55,30,10);
          StepMotor_Drive(1,RoTask_Speed[6]);
          while (OVER == Flag_doing);

          //清除数据
          ClearRxData;   
          RxData.C_dis[arr5[i]]+=600;
          RxData.C_ang[arr5[i]]=179;           
          HAL_Delay(800);

          if(Abs(RxData.C_dis[arr5[i]]) >=500)
          {
               StepMotor_Set_AnglePulse(95,0,Angle3_Ready);
               StepMotor_Drive(1,RoTask_Speed[5]);
               while (OVER == Flag_doing)
               {
                    if  ( Abs(RxData.C_ang[arr5[i]]-90) <= 4  )
                    {
                              StepMotor_Drive(StepALL_STOP, 0);
                              break; 
                    }
               }            
          }
          
          HAL_Delay(400);
          StartTimeCnt(12);
          while(1)// 物料颜色判断
          {
               printf("%f\r\n",RxData.C_ang[arr5[i]]);
               if(  (RxData.C_ang[arr5[i]]>1 &&RxData.C_ang[arr5[i]]<177)  ||  (RxData.C_ang[arr5[i]]>183&RxData.C_ang[arr5[i]]<358)  )
               {    
                    ClearTimeFlag();
                    break;
               } 
               if(ReadTimeFlag()==1)
               {
                    flag_Place=1;
                    break;
               }
          } 
          if(flag_Place==1)
               continue;


          //转向逼近
          while (1)
          {   
               
               HAL_Delay(2);
               StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu+RxData.C_ang[arr5[i]]-90,30,10);
               StepMotor_Drive(1, RoTask_Speed[9]);
			
               while (OVER == Flag_doing ) 
               {
                    if  ( Abs(RxData.C_ang[arr5[i]]-90) <= 4  )
                    {
                              StepMotor_Drive(StepALL_STOP, 0);
                              break; 
                    }
                    if(Motor_1.Pulse>180/Bu)
                    {
                         goto Place1;
                    }
               }

               if  ( Abs(RxData.C_ang[arr5[i]]-90) <= 7  )
               {
                         break ;
               }
          }
          
          //水平
          StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,30,10);
          StepMotor_Drive(1,RoTask_Speed[6]);
          while (OVER == Flag_doing);
          
          StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,40,13);
          StepMotor_Drive(1,RoTask_Speed[8]);
          RxData.C_dis[arr5[i]]+=700;
          while(OVER == Flag_doing)
          {
            if  ( Abs(RxData.C_dis[arr5[i]]) <= 50  )
            {
               Motor_2.pwmPulse=0; Motor_2.Flag=Flag_finish; Motor_2.tarPulse=0;
                break; 
            }
          }

          Angle_vertical(10,Calculate_DisHorizon(),2);
          //第二次水平
          HAL_Delay(200);
          while (1)
          {   
               HAL_Delay(2);
               StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu+RxData.C_ang[arr5[i]]-90,Motor_2.Pulse*Bu,(Motor_3.Pulse)*Bu+OriginalAngle3);
               StepMotor_Drive(1, RoTask_Speed[8]);
			
               while (OVER == Flag_doing ) 
               {
                    if  ( Abs(RxData.C_ang[arr5[i]]-90) <= 4 )
                    {
                         StepMotor_Drive(StepALL_STOP, 0);
                         break; 

                         if(Motor_1.Pulse*Bu>175)
                              goto Place1;
                    }
               }

               if  ( Abs(RxData.C_ang[arr5[i]]-90) <= 7  )
               {
                         break ;
               }
          }

          HAL_Delay(200);
          //第二次前进
          StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,Motor_2.Pulse*Bu+2 ,Motor_3.Pulse*Bu+OriginalAngle3);
          StepMotor_Drive(1,RoTask_Speed[10]);
          RxData.C_dis[arr5[i]]+=700;
          while(OVER == Flag_doing)
          {
            if ( Abs(RxData.C_dis[arr5[i]]) <= 10  )
            {
               Motor_2.pwmPulse=0; Motor_2.Flag=Flag_finish; Motor_2.tarPulse=0;
                break; 
            }
          }
          //舵机转向准备放取
          ServoTurn(90);
          HAL_Delay(600);
          
          //垂直下降
          Angle_vertical(-50,Calculate_DisHorizon(),2);

          ServoClaw(OpenSide);
          HAL_Delay(300);

          //垂直上升（拟合的死代码）
          StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,Motor_2.Pulse*Bu-15,Motor_1.Pulse+OriginalAngle3+10);
          StepMotor_Drive(1,RoTask_Speed[5]);
          while (OVER == Flag_doing);

          StepMotor_Set_AnglePulse(0,0,Angle3_Ready);
          StepMotor_Drive(1,RoTask_Speed[5]);
          while (OVER == Flag_doing);
     }
}
     //---------------------转向逼近----------------
     //   if (Task_num == Base)
     //   {    
     //      
     //        StepMotor_Set_AnglePulse(90,0,74.5);
     //        StepMotor_Drive(1,600);
     //        while (OVER == Flag_doing);
     //        HAL_Delay(850);

     //        while (1)
     //        {
     //           // HAL_Delay(250);
     //           StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu+RxData.B_ang[1]-90,0,80);
     //           StepMotor_Set_Speed(1500);
     //           StepMotor_Drive(1);
							 
     //           while (OVER == Flag_doing ) //correct 
     //           {
     //                printf("correcting \r\n");
     //                if  ( Abs(RxData.B_ang[1]-90) <= 5  )
     //                {
     //                          printf("finish err : %f \r\n",RxData.B_ang[1]-90 ); 
     //                          StepMotor_Drive(0);
     //                          break; 
     //                }
     //           }
     //           if  ( Abs(RxData.B_ang[1]-90) <= 7  )
     //           {
     //                     break ;
     //           }
     //        }

     //      /*------------------距离逼近--------------------*/
          
     //      HAL_Delay(200);
     //      StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,0,OriginalAngle3-3);//
     //      StepMotor_Set_Speed(600);
     //      StepMotor_Drive(1);
     //      while (OVER == Flag_doing);

     //      RxData.B_dis[1]+=700;
     //      int Lever_Close=0;

     //       while (1)
     //        {
               
     //           // HAL_Delay(250);
     //           if(RxData.B_dis[1]>=200)
     //                Lever_Close+=5;
     //           else if (RxData.B_dis[1]>175&&RxData.B_dis[1]<200)
     //                Lever_Close+=3;
     //           else 
     //                Lever_Close++; 
               
     //           StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,Lever_Close,OriginalAngle3-3);
     //           StepMotor_Set_Speed(1250);
     //           StepMotor_Drive(1);
               
							 
     //           while (OVER == Flag_doing ) //correct 
     //           {
     //                printf("correcting \r\n");
     //                if  ( Abs(RxData.B_dis[1]) <= 150  )
     //                {
     //                          printf("finish err : %f \r\n",RxData.B_dis[1]); 
     //                          StepMotor_Drive(0);
     //                          break; 
     //                }
     //           }
     //           if  ( Abs(RxData.B_dis[1]) <= 150  )
     //           {
     //                break ;
     //           }
     //        }
     //   }

     //-------------转180找物块,后距离强停--------------
          // StepMotor_Set_AnglePulse(0,0,74.5);
          // StepMotor_Drive(1,850);
          // while (OVER == Flag_doing);
          // RxData.B_dis[1]+=700;
          // RxData.B_ang[1]=179;

          // StepMotor_Set_AnglePulse(180,0,74.5);
          // StepMotor_Drive(1,1000);

          // while (OVER == Flag_doing)
          // {
          //      if(RxData.B_ang[1]>87&&RxData.B_ang[1]<=93)
          //      {
          //           Motor_1.pwmPulse=0; Motor_1.Flag=Flag_finish; Motor_1.tarPulse=0;
          //      }     
               
          // }
          // StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,0,OriginalAngle3);
          // StepMotor_Drive(1,850);
          // while (OVER == Flag_doing);
          
          // StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,30,OriginalAngle3);
          // StepMotor_Drive(1,1000);
          // RxData.B_dis[1]+=700;
          // while(OVER == Flag_doing)
          // {
          //   if  ( Abs(RxData.B_dis[1]) <= 50  )
          //   {
          //      Motor_2.pwmPulse=0; Motor_2.Flag=Flag_finish; Motor_2.tarPulse=0;
          //       break; 
          //   }
          // }



          // if((RxData.B_ang[RED]>60&&RxData.B_ang[RED]<90)||(RxData.B_ang[RED]>270&&RxData.B_ang[RED]<300))
          // {
          //      StepMotor_Set_AnglePulse(60,0,74.5);
          //      StepMotor_Drive(1,300);
          //      while (OVER == Flag_doing)
          //      {
          //           if(RxData.B_ang[RED]>87&&RxData.B_ang[RED]<93)
          //           {
          //                StepMotor_Drive(0);
          //                printf("finish");
          //           }     
          //      }
          // }
          // if((RxData.B_ang[RED]>90&&RxData.B_ang[RED]<120)||(RxData.B_ang[RED]>240&&RxData.B_ang[RED]<270))
          // {
          //      StepMotor_Set_AnglePulse(120,0,74.5);
          //      StepMotor_Drive(1,300);
          //      while (OVER == Flag_doing)
          //      {
          //           if(RxData.B_ang[RED]>87&&RxData.B_ang[RED]<93)
          //           {
          //                StepMotor_Drive(0);
          //                break;
          //           }     
          //      }
          // }

 

 

 

// void StepArm_Task_Pan(unsigned char* arr1)      //从圆盘上抓取物块至放完物块
// {
//      for(int i=0;i<3;i++)
//      {
//           int flag_pan=0;
//           StepMotor_Set_AnglePulse(0,0,74.5);
//           StepMotor_Drive(1,250);
//           while (OVER == Flag_doing);

//           ServoClaw(OpenPan);
//           HAL_Delay(100);
          

//           // StepMotor_Set_AnglePulse(90,0,74.5);
//           StepMotor_Set_AnglePulse(90,0,74.5);
//           StepMotor_Drive(1,200);
//           while (OVER == Flag_doing);//到固定抓取位置

//           StepMotor_Set_AnglePulse(90,0,50);
//           StepMotor_Drive(1,250);
//           while (OVER == Flag_doing);//到固定抓取位置

//           ClearRxData;                         //清除数据
//           RxData.B_dis[arr1[i]]+=700;
//           RxData.B_ang[arr1[i]]=179;

          
//           while(1)// 物料颜色判断
//           {
//                DebugB;
//                if(  (RxData.B_ang[arr1[i]]>30 &&RxData.B_ang[arr1[i]]<150)  ||  (RxData.B_ang[arr1[i]]>210&RxData.B_ang[arr1[i]]<330)  )
//                {
//                     break;
//                } 
//           } 

//           while (1)
//             {
//                if(RxData.B_ang[arr1[i]]>=180&&RxData.B_ang[arr1[i]]<360)
//                {
//                     break;
//                     flag_pan=1;
//                }     
//                HAL_Delay(2);
//                StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu+RxData.B_ang[arr1[i]]-90,0,50);
//                StepMotor_Set_Speed(1500);
//                StepMotor_Drive(1);
			

//                while (OVER == Flag_doing ) 
//                {
//                     if  ( Abs(RxData.B_ang[arr1[i]]-90) <= 5  )
//                     {
//                               StepMotor_Drive(0);
//                               break; 
//                     }
//                }
//                if  ( Abs(RxData.B_ang[arr1[i]]-90) <= 7  )
//                {
//                          break ;
//                }
//             }

//           if(flag_pan==1)
//           {
//                StepMotor_Set_AnglePulse(90,0,50);
//                StepMotor_Drive(1,250);
//                while (OVER == Flag_doing);
//                while (1)
//                {

//                     HAL_Delay(2);
//                     StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu+RxData.B_ang[arr1[i]]-90,0,50);
//                     StepMotor_Set_Speed(1500);
//                     StepMotor_Drive(1);
                    

//                     while (OVER == Flag_doing ) 
//                     {
//                          if  ( Abs(RxData.B_ang[arr1[i]]-90) <= 5  )
//                          {
//                               StepMotor_Drive(0);
//                               break; 
//                          }
//                     }
//                     if  ( Abs(RxData.B_ang[arr1[i]]-90) <= 7  )
//                     {
//                          break ;
//                     }
//                }

//           }
          

//           StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,0,OriginalAngle3+5.5);
//           StepMotor_Drive(1,300);
//           while (OVER == Flag_doing);
          
//           StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,30,OriginalAngle3+5.5);
//           StepMotor_Drive(1,400);
//           RxData.B_dis[arr1[i]]+=700;
//           while(OVER == Flag_doing)
//           {
//                DebugB;
//                if  ( Abs(RxData.B_dis[arr1[i]]) <= 60  )
//                {
//                     Motor_2.pwmPulse=0; Motor_2.Flag=Flag_finish; Motor_2.tarPulse=0;
//                     break; 
//                }
//             printf("%f\r\n",RxData.B_dis[arr1[i]]);
//           }
//           StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,Motor_1.Pulse*Bu,OriginalAngle3+2.5);
//           StepMotor_Drive(1,300);
//           while (OVER == Flag_doing);

//           HAL_Delay(200);

//           Load(arr1[i]);  
//      }
//      ServoClaw(OpenCar);
// }

// void StepArm_Task_Jiagon(unsigned char* arr2)     //粗加工区 先放 后装车 然后走
// {
     
//      for(int i=0;i<3;i++)
//      {    
          
//           //卸货
//           Pick_Floor(arr2[i]);          
//           ServoTurn(0);
//           HAL_Delay(1000);
          
//           ClearRxData;                        //清除数据
//           StepMotor_Set_AnglePulse(0,0,74.5);
//           StepMotor_Drive(1,350);
//           while (OVER == Flag_doing);
//           RxData.C_dis[arr2[i]]+=700;
//           RxData.C_ang[arr2[i]]=179;

//           //转向和距离的逼近
//           StepMotor_Set_AnglePulse(180,0,74.5);
//           StepMotor_Drive(1,600);

//           while (OVER == Flag_doing)
//           {
//                if((RxData.C_ang[arr2[i]]>=85&&RxData.C_ang[arr2[i]]<=95)||(RxData.C_ang[arr2[i]]>265&&RxData.C_ang[arr2[i]]<=275))
//                {
//                     Motor_1.Flag=Flag_finish; 
//                }
//           }     
//           StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,0,OriginalAngle3);
//           StepMotor_Drive(1,850);
//           while (OVER == Flag_doing);
          
//           StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,30,OriginalAngle3);
//           StepMotor_Drive(1,1000);
//           RxData.C_dis[arr2[i]]+=700;
//           while(OVER == Flag_doing)
//           {
//             if  ( Abs(RxData.C_dis[arr2[i]]) <= 50  )
//             {
//                Motor_2.pwmPulse=0; Motor_2.Flag=Flag_finish; Motor_2.tarPulse=0;
//                 break; 
//             }
//           }

//           //舵机转向准备抓取
//           ServoTurn(90);
//           HAL_Delay(1000);
          
//           //垂直下降
//           StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,Motor_2.Pulse*Bu+28.5,OriginalAngle3-18.5);
//           StepMotor_Drive(1,850);
//           while (OVER == Flag_doing);

//           ServoClaw(OpenSide);
//           HAL_Delay(300);

//           //垂直上升
//           StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,Motor_2.Pulse*Bu-15,OriginalAngle3+10);
//           StepMotor_Drive(1,350);
//           while (OVER == Flag_doing);

//           StepMotor_Set_AnglePulse(0,0,73.5);
//           StepMotor_Drive(1,350);
//           while (OVER == Flag_doing);
//      }

//      for (int i = 0; i < 3; i++)
//      {
//           //准备阶段
          
//           StepMotor_Set_AnglePulse(0,0,74.5);
//           StepMotor_Drive(1,350);
//           while (OVER == Flag_doing);
          
          

//           //转向和距离的逼近
//           ClearRxData;                     //清除数据
//           RxData.C_dis[arr2[i]]+=700;
//           RxData.C_ang[arr2[i]]=179;

//           StepMotor_Set_AnglePulse(180,0,74.5);
//           StepMotor_Drive(1,600);

//           while (OVER == Flag_doing)
//           {
//                if((RxData.C_ang[arr2[i]]>=85&&RxData.C_ang[arr2[i]]<=95)||(RxData.C_ang[arr2[i]]>265&&RxData.C_ang[arr2[i]]<=275))
//                {
//                     Motor_1.Flag=Flag_finish; 
//                }
//           }     
//           StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,0,OriginalAngle3+10);
//           StepMotor_Drive(1,850);
//           while (OVER == Flag_doing);
          
//           StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,30,OriginalAngle3);
//           StepMotor_Drive(1,1000);
//           RxData.C_dis[arr2[i]]+=700;
//           while(OVER == Flag_doing)
//           {
//             if  ( Abs(RxData.C_dis[arr2[i]]) <= 50  )
//             {
//                Motor_2.Flag=Flag_finish; 
//                 break; 
//             }
//           }

//           //垂直下降
//           StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,Motor_2.Pulse*Bu+27.5,OriginalAngle3-17.5);
//           StepMotor_Drive(1,850);
//           while (OVER == Flag_doing);

//           //抓取
//           Load(arr2[i]);
//      }
     
// }

// void StepArm_Task_Over(unsigned char* arr3)   //细加工区 放置物块 然后走
// {
//      for(int i=0;i<3;i++)
//      {    
//           //卸货
//           Pick_Floor(arr3[i]);          
//           ServoTurn(0);
//           HAL_Delay(1000);

//           StepMotor_Set_AnglePulse(0,0,74.5);
//           StepMotor_Drive(1,350);
//           while (OVER == Flag_doing); 
//           ClearRxData;                        //清除数据
//           RxData.C_dis[arr3[i]]+=700;
//           RxData.C_ang[arr3[i]]=179;

//           //转向和距离的逼近
//           StepMotor_Set_AnglePulse(180,0,74.5);
//           StepMotor_Drive(1,600);

//           while (OVER == Flag_doing)
//           {
//                if((RxData.C_ang[arr3[i]]>=85&&RxData.C_ang[arr3[i]]<=95)||(RxData.C_ang[arr3[i]]>265&&RxData.C_ang[arr3[i]]<=275))
//                {
//                     Motor_1.Flag=Flag_finish; 
//                }
//           }     
//           StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,0,OriginalAngle3);
//           StepMotor_Drive(1,850);
//           while (OVER == Flag_doing);
          
//           StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,30,OriginalAngle3);
//           StepMotor_Drive(1,1000);
//           RxData.C_dis[arr3[i]]+=700;
//           while(OVER == Flag_doing)
//           {
//             if  ( Abs(RxData.C_dis[arr3[i]]) <= 50  )
//             {
//                Motor_2.pwmPulse=0; Motor_2.Flag=Flag_finish; Motor_2.tarPulse=0;
//                 break; 
//             }
//           }

//           //舵机转向准备抓取
//           ServoTurn(90);
//           HAL_Delay(1000);
          
//           //垂直下降
//           StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,Motor_2.Pulse*Bu+28.5,OriginalAngle3-18.5);
//           StepMotor_Drive(1,850);
//           while (OVER == Flag_doing);

//           ServoClaw(OpenSide);
//           HAL_Delay(300);

//           //垂直上升
//           StepMotor_Set_AnglePulse(Motor_1.Pulse*Bu,Motor_2.Pulse*Bu-15,OriginalAngle3+10);
//           StepMotor_Drive(1,350);
//           while (OVER == Flag_doing);

//           StepMotor_Set_AnglePulse(0,0,73.5);
//           StepMotor_Drive(1,350);
//           while (OVER == Flag_doing);
//      }

// }