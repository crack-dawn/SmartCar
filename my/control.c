
#include "control.h"
#include "StepMotor.h"
#include "RobTask.h"

unsigned short task = 0;  /*小车的任务动作*/
unsigned short do_cnt = 0;

short 	 clawTask=0;      /*机械臂的任务动作*/
short 	 clawDo_cnt=0;

#define Abs(x)   ((x) < 0 ? -(x) : (x))     


void Other_Actions()
{
  // if (do_cnt == do_Arm )
  // {
  //     HAL_Delay(2000);
  //     ReturnToCarRun;
  // }
  // else if (task == ScanCode_1 && do_cnt == do_ScanCode)
  // {
  //   HAL_Delay(500);
  //     do_cnt = finish_Scancode;
  // }
 
  if (task == ScanCode_1 && do_cnt == do_ScanCode)
  {//第一圈扫码
        ScanCode();
        LED1_ON;
        StepArm_Task_ScanCode(); //扫码时调整机械臂方便扫码

        if (RxData.codeFlag == codeOK)
        {
          do_cnt = finish_Scancode;
        }
  }
  else if (task == RunToRawMaterial_1_Pick && do_cnt == do_Arm)
  {//第一圈装物料
    SendCmdB;  //HAL_Delay(200);//识别物块模式

    StepArm_Task_Pan(RxData.Task1) ; //原料区
    ReturnToCarRun;
  }

  else if (task == RunToRoughArea_1 && do_cnt == do_Arm)
  {//第一圈粗加工
    SendCmdC; //HAL_Delay(200);   //识别圆环模式

    StepArm_Task_Jiagon(RxData.Task1); //粗加工区
    ReturnToCarRun;
  }

  else if(task == RunToFineArea_1 && do_cnt == do_Arm)
  {
    //第一圈细加工
    SendCmdC; //HAL_Delay(200);  //识别圆环模式

    StepArm_Task_Over(RxData.Task1);  //细加工区
    ReturnToCarRun;
  }
}















int   rpm=0;  	 
float num=0; 	 

 
//113cm * 135cm 论间距280
#define Length 1320
#define Width  1120
#define TURN_90 570
#define TURN_180 1180
#define TargetV 50
 
 

void Top_Action_Select_CallBack_TIM7(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM7)  //TIM7 40ms kernel
	{	   
		//DebugB;
		unsigned char status =PID_JudgeStatus();
		switch (task)
		{// 8.2->9.8
			case Stop:{
				if (do_cnt== do_00){
					CAR_RUN(Wait,runSpeed,0,				runSpeed,0,		0);
					++do_cnt;
				}
			}break;

			case BackOut_1:{//出库			
						
					if (do_cnt == do_00 && (status == PIDparam3_finish)){
						SendCmdA;
						CAR_RUN(Zero_GO,LowSpeed,-65,					LowSpeed,-65,					0);
						++do_cnt;
					}
					else if (do_cnt == do_01 && (status == PIDparam3_finish)){
						CAR_RUN(Zero_GO,runSpeed*0.6,-290*0.6,    	runSpeed,-290,				0);
						++do_cnt;
					}			
					else if (do_cnt == do_02 && (status == PIDparam3_finish)){
						++do_cnt;
						CAR_RUN(Zero_GO,runSpeed,-290,				runSpeed*0.6,-290*0.6,	0);
					}
					else if (do_cnt == do_03 && (status == PIDparam3_finish)){
						++do_cnt;
						CAR_RUN(Zero_GO, 30, 45, 30,45,	0);
					}						
					else if (do_cnt == do_04 && (status == PIDparam3_finish || RxData.turn == turnDo)  ){//RxData.turn
						++do_cnt;
						CAR_RUN(Zero_GO,runSpeed/2, 9.8,				runSpeed,475,					 0);
					}
					else if (do_cnt == do_05 && (status == PIDparam3_finish))
					{
						ScanCode();
						task=ScanCode_1;
						do_cnt = 0; //出库任务结束
					}
			}break;

	/******************************************************************************************/
			case ScanCode_1:{//走到扫码点停下, 扫码之后结束
					if (do_cnt == do_00 && status == PIDparam3_finish ){
						ScanCode();
						CAR_RUN(Correct_GO  , CorrectSpeed,520, CorrectSpeed,520,	 0);
						++do_cnt;
					}//reach  接了下来扫码
					else if(do_cnt == do_01 && status == PIDparam3_finish)
					{
						 do_cnt = do_ScanCode;
					}
					else if(do_cnt == finish_Scancode && status == PIDparam3_finish )
					{
						task=RunToRawMaterial_1_Pick;
						do_cnt = 0; //扫码任务结束
					}
			}break;

			case RunToRawMaterial_1_Pick:{//走到 原料区， 拿起物料 ，然后转弯走入下一个直道
					if (do_cnt == do_00 && status == PIDparam3_finish){ 
						CAR_RUN(Correct_GO  , runSpeed,540, runSpeed,540,	 0);//现在 -250
						do_cnt=do_01;
					}//接下来 捡起物料
					else if (do_cnt == do_01  &&  status == PIDparam3_finish)
					{
						do_cnt=do_Arm;
					}
					else if (do_cnt == finish_Arm && status == PIDparam3_finish){ 
						CAR_RUN(Correct_GO  , runSpeed,360, runSpeed,360,	 0);//--------------------
						do_cnt = do_02;
					}	
					else if ( (do_cnt == do_02)   && ( status == PIDparam3_finish || RxData.turn == turnDo)   ){//RxData.turn 直行完了 或者有转弯标志 直接转弯
						CAR_RUN(Zero_GO,runSpeed/2, 9.8,				runSpeed,475,					 0);
						do_cnt=do_03;
					}	
					else if(do_cnt == do_03 && status == PIDparam3_finish)
					{ 
						task=RunToRoughArea_1;
						do_cnt = 0; //原料区，捡起物料任务结束
					}	
			}break;

	/******************************************************************************************/
			case RunToRoughArea_1:{//跑到粗加工域 放置，抓取，转弯进入下一个直道
					if (do_cnt == do_00 && status == PIDparam3_finish){ 
						CAR_RUN(Correct_GO  , runSpeed,780, runSpeed,780,	 0);
						do_cnt=do_01;
					}
					else if (do_cnt == do_01 && status == PIDparam3_finish)
					{
						do_cnt=do_Arm;
					}
					else if (do_cnt == finish_Arm && status == PIDparam3_finish){ 
						CAR_RUN(Correct_GO  , runSpeed,395, runSpeed,395,	 0);//--------------------
						do_cnt=do_02;
					}	
					else if (do_cnt == do_02  && ( status == PIDparam3_finish  || RxData.turn == turnDo)  ){//RxData.turn 直行完了 或者有转弯标志 直接转弯
						CAR_RUN(Zero_GO,runSpeed/2, 9.8,				runSpeed,475,					 0);
						do_cnt = do_03;
					}
					else if(do_cnt == do_03 && status == PIDparam3_finish)
					{
						task = RunToFineArea_1;
						do_cnt = 0; //粗加工 结束
					}
			}break;

			case RunToFineArea_1:{//跑到精加工域 放置，转弯进入下一个直道
					if (do_cnt == do_00 && status == PIDparam3_finish){ 
						CAR_RUN(Correct_GO  , runSpeed,660, runSpeed,660,	 0);
						do_cnt=do_01;
					}//reach
					else if (do_cnt == do_01 && status == PIDparam3_finish){ 
						do_cnt = do_Arm;
					}
					else if (do_cnt == finish_Arm && status == PIDparam3_finish){ 
						CAR_RUN(Correct_GO  , runSpeed,730, runSpeed,730,	 0);
						do_cnt = do_02;
					}	
					else if ( do_cnt == do_02 && ( status == PIDparam3_finish  || RxData.turn == turnDo)  ){//RxData.turn 直行完了 或者有转弯标志 直接转弯
						CAR_RUN(Zero_GO,runSpeed/2, 9.8,				runSpeed,475,					 0);
						++do_cnt;
					}	
					else if(do_cnt == do_03 && status == PIDparam3_finish)
					{
						task = RunToCircle2_Start;
						do_cnt = 0; //捡起物料任务结束
					}
			}break;

			case RunToCircle2_Start:{ //回到第二圈起始点
				if (do_cnt == do_00 && status == PIDparam3_finish){ 
					CAR_RUN(Correct_GO  , runSpeed,310, runSpeed,310,	 0);
					do_cnt = do_01;
				}//reach turn 
				else if (do_cnt == do_01 &&   status == PIDparam3_finish   ){//RxData.turn 直行完了 或者有转弯标志 直接转弯
					CAR_RUN(Correct_GO  , runSpeed,900, runSpeed,900,	 0);
					++do_cnt;
				}//reach start 
				else if (do_cnt == do_02 && ( RxData.turn == turnDo ||  status == PIDparam3_finish) )
				{
					CAR_RUN(Zero_GO,runSpeed/2, 9.8,				runSpeed,475,					 0);
					++do_cnt;
				}
				else if(do_cnt == do_03 && status == PIDparam3_finish)
				{
					task = RunToRawMaterial_2_Pick;
					do_cnt = 0;  
				}
			}break;
			
			case RunToRawMaterial_2_Pick:{//第二圈，原料区，抓取装载物料，装车，转弯进入下一个直道
					if (do_cnt == do_00 && status == PIDparam3_finish ){
						CAR_RUN(Correct_GO  , CorrectSpeed,1170, CorrectSpeed,1170,	 0);
						do_cnt = do_01;
					}//reach 扫码点附近
					else if (do_cnt == do_01  &&  status == PIDparam3_finish)
					{
						do_cnt=do_Arm;
					}
					else if (do_cnt == finish_Arm && status == PIDparam3_finish){ 
						CAR_RUN(Correct_GO  , runSpeed,260, runSpeed,260,	 0);//--------------------
						do_cnt=do_02;
					}	
					else if ( (do_cnt == do_02)   && ( status == PIDparam3_finish || RxData.turn == turnDo)   ){//RxData.turn 直行完了 或者有转弯标志 直接转弯
						CAR_RUN(Zero_GO,runSpeed/2, 9.8,				runSpeed,475,					 0);
						++do_cnt;
					}	
					else if(do_cnt == do_03 && status == PIDparam3_finish)
					{ 
						task=RunToRoughArea_2;
						do_cnt = 0; //原料区，捡起物料任务结束
					}	
			}break;

			case RunToRoughArea_2:{//第二圈 跑到粗加工域 放置，抓取，转弯进入下一个直道
					if (do_cnt == do_00 && status == PIDparam3_finish){ 
						CAR_RUN(Correct_GO  , runSpeed,700, runSpeed,700,	 0);
						do_cnt=do_01;
					}
					else if (do_cnt == do_01 && status == PIDparam3_finish)
					{
						do_cnt=do_Arm;
					}
					else if (do_cnt == finish_Arm && status == PIDparam3_finish){ 
						CAR_RUN(Correct_GO  , runSpeed,525, runSpeed,525,	 0);//--------------------
						do_cnt=do_02;
					}	
					else if (do_cnt == do_02  && ( status == PIDparam3_finish  || RxData.turn == turnDo)  ){//RxData.turn 直行完了 或者有转弯标志 直接转弯
						CAR_RUN(Zero_GO,runSpeed/2, 9.8,				runSpeed,475,					 0);
						do_cnt=do_03;
					}	
					else if(do_cnt == do_03 && status == PIDparam3_finish)
					{
						task = RunToFineArea_2;
						do_cnt = 0; //粗加工 结束
					}
			}break;

			case RunToFineArea_2:{//跑到精加工域 放置，转弯进入下一个直道
					if (do_cnt == do_00 && status == PIDparam3_finish){ 
						CAR_RUN(Correct_GO  , runSpeed,565, runSpeed,565,	 0);
						do_cnt=do_01;
					}//reach
					else if (do_cnt == do_01 && status == PIDparam3_finish){ 
						do_cnt = do_Arm;
					}
					else if (do_cnt == finish_Arm && status == PIDparam3_finish){ 
						CAR_RUN(Correct_GO  , runSpeed,875, runSpeed,875,	 0);
						do_cnt = do_02;
					}	
					else if ( do_cnt == do_02 && ( status == PIDparam3_finish  || RxData.turn == turnDo)  ){//RxData.turn 直行完了 或者有转弯标志 直接转弯
						CAR_RUN(Zero_GO,runSpeed/2, 9.8,				runSpeed,475,					 0);
						++do_cnt;
					}	
					else if(do_cnt == do_03 && status == PIDparam3_finish)
					{
						task = RunToRest;
						do_cnt = 0; //捡起物料任务结束
					}
			}break;

			case RunToRest:{//走到最后停车点
					if (do_cnt == do_00 && status == PIDparam3_finish){ 
						CAR_RUN(Correct_GO  , runSpeed,300, runSpeed,300,	 0);
						++do_cnt;
					}
					else if (do_cnt == do_01 && status == PIDparam3_finish){ 
						CAR_RUN(Correct_GO  , runSpeed,890, runSpeed,890,	 0);
						++do_cnt;
					}
					else if (do_cnt == do_02 && ( status == PIDparam3_finish  || RxData.turn == turnDo)  ){//RxData.turn 直行完了 或者有转弯标志 直接转弯
						CAR_RUN(Zero_GO,LowSpeed,260,              LowSpeed*0.3,260*0.3,  0);
						++do_cnt;
					}								
					else if (do_cnt == do_03 && (status == PIDparam3_finish)){
						CAR_RUN(Zero_GO,LowSpeed*0.3,260*0.3,	    LowSpeed,260,			0);
						++do_cnt;
					}			
					else if (do_cnt == do_04 && (status == PIDparam3_finish)){
						CAR_RUN(Zero_GO,LowSpeed , 185,	    LowSpeed, 185,			0);
						++do_cnt;
					}	
					else if(do_cnt == do_05 && status == PIDparam3_finish)
					{
						task = Stop;
						do_cnt = 0; //捡起物料任务结束
					}
			}break;
			
			default:
				break;



			// case Speed:{
			// 	if ( status == PIDparam3_finish){//RxData.turn
			// 		CAR_RUN(Abs_GO,rpm,num,				rpm,num,				0);
			// 	}
			// }break;

			// case Debug:{
			// 		CAR_RUN(speed_debug,runSpeed,0,runSpeed,0,				0);
			// }break;
		}
	}
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	Top_PID_Control_CallBack_TIM6( htim);//20ms
	Top_Action_Select_CallBack_TIM7( htim); //20ms
	StepMotor_CallBack_TIM5( htim); // 2ms   TIM5
}



