
#include "control.h"
#include "StepMotor.h"
#include "RobTask.h"
#include "tim.h"

unsigned int cnt_MiaoFlag=0;

int task = 0;   
int do_cnt = 0;
unsigned char B_updataFlag = 0;


short 	 clawTask=0;       
short 	 clawDo_cnt=0;

#define Abs(x)   ((x) < 0 ? -(x) : (x))     


void Other_Actions()
{
//   if (do_cnt == do_Arm )   //debug 跑图用  
//   {
//       HAL_Delay(1500);
//       ReturnToCarRun;
//   }
//   else if (task == ScanCode_1 && do_cnt == do_ScanCode)
//   {
//     	HAL_Delay(1500);
//         do_cnt = finish_Scancode;
//   }
  

  /***********  跑图穿插 机械臂动作             ****/
	//第一圈
  if (task == ScanCode_1 && do_cnt == do_ScanCode)
  {
	 
		ScanCode();
        StepArm_Task_ScanCode(); 

        if (RxData.codeFlag == codeOK)
        {
          do_cnt = finish_Scancode;
        }
  }
  else if (task == RunToRawMaterial_1_Pick && do_cnt == do_Arm)
  { 
    SendCmdB;   
    StepArm_Task_Pan(RxData.Task1) ;  
    ReturnToCarRun;
  }

  else if (task == RunToRoughArea_1 && do_cnt == do_Arm)
  { 
    SendCmdC;  
    StepArm_Task_Jiagon(RxData.Task1);  
    ReturnToCarRun;
  }

  else if(task == RunToFineArea_1 && do_cnt == do_Arm)
  {
     
    SendCmdC; 
    StepArm_Task_Over(RxData.Task1);  
    ReturnToCarRun;
  }



//第二圈
  else if (task == RunToRawMaterial_2_Pick && do_cnt == do_Arm)
  { 
    SendCmdB;  
    StepArm_Task_Pan(RxData.Task2) ; 
    ReturnToCarRun;
  }

  else if (task == RunToRoughArea_2 && do_cnt == do_Arm)
  { 
    SendCmdC;  
    StepArm_Task_Jiagon(RxData.Task2);  
    ReturnToCarRun;
  }

  else if(task == RunToFineArea_2 && do_cnt == do_Arm)
  {
    SendCmdC;  
    StepArm_Task_OverPlace(RxData.Task2);  
    ReturnToCarRun;
  }
}


int   rpm=0;  	 
float num=0; 	 

 
 
#define Length 1430
#define Width  1130
#define TURN_90 570
#define TURN_180 1180
#define TargetV 50
 
 

void Top_Action_Select_CallBack_TIM7(TIM_HandleTypeDef *htim)
{
	static  unsigned char status;
	static  unsigned char cnt=0;
	if(htim->Instance == TIM7)  //TIM7 40ms kernel
	{	
		/*------- LCD display ------------*/
		++cnt;
		if (cnt == 25){
			cnt = 0;
			UART_LCD_UpdataDisplay(&huart5);
		}
		/*-------   ------------*/

		/*-------   -----------*/
		status =PID_JudgeStatus();
		switch (task)
		{
			case Stop:{
				if (do_cnt== do_00){
					CAR_RUN(Wait,runSpeed,0,				runSpeed,0,		0);
					++do_cnt;
				}
			}break;

			case BackOut_1:{//			
						
					if (do_cnt == do_00 && (status == PIDparam3_finish)){
						SendCmdA;
						CAR_RUN(Zero_GO,LowSpeed,-55,					LowSpeed,-55,					0);
						++do_cnt;
					}
					else if (do_cnt == do_01 && (status == PIDparam3_finish)){
						CAR_RUN(Zero_GO,runSpeed ,0,            	    LowSpeed,-60,				  0);
						++do_cnt;
					}
					else if (do_cnt == do_02 && (status == PIDparam3_finish)){
						++do_cnt;//480
						CAR_RUN(Zero_GO,runSpeed,-465,                   runSpeed, -465,	   			  0);
					} 
					else if (do_cnt == do_03 && (status == PIDparam3_finish)){
						++do_cnt;
						CAR_RUN(Zero_GO, LowSpeed ,-60,					runSpeed,0,						0);
					} 
					else if (do_cnt == do_04 && (status == PIDparam3_finish)){
						++do_cnt;
						CAR_RUN(Zero_GO, LowSpeed, 130, LowSpeed, 130,	0);
					}						
					else if (do_cnt == do_05 && (status == PIDparam3_finish || RxData.turn == turnDo)  ){//RxData.turn
						++do_cnt;
						CAR_RUN(Zero_GO,40.11, 12.0,				runSpeed,475.0,					 0);
					}
					else if (do_cnt == do_06 && status == PIDparam3_finish )
					{
						task=ScanCode_1;
						do_cnt = 0; // 
					}
			}break;
		 
	/******************************************************************************************/
			case ScanCode_1:{// x= 1415
					if (do_cnt == do_00 && status == PIDparam3_finish ){
						CAR_RUN(Correct_GO  , CorrectSpeed,500, CorrectSpeed,500,	 0);
						++do_cnt;
					}
					else if(do_cnt == do_01 && status == PIDparam3_finish)
					{
						 do_cnt = do_ScanCode;
					}
					else if(do_cnt == finish_Scancode && status == PIDparam3_finish )
					{
						task=RunToRawMaterial_1_Pick;
						do_cnt = 0; // 
					}
			}break;

			case RunToRawMaterial_1_Pick:{// x=1415
					if (do_cnt == do_00 && status == PIDparam3_finish){ 
						CAR_RUN(Correct_GO  , runSpeed,590, runSpeed,590,	 0);// 
						do_cnt=do_01;
					}// 
					else if (do_cnt == do_01  &&  status == PIDparam3_finish)
					{
						do_cnt=do_Arm;
					}
					else if (do_cnt == finish_Arm && status == PIDparam3_finish){ 
						CAR_RUN(Correct_GO  , runSpeed,325, runSpeed,325,	 0);//--------------------
						do_cnt = do_02;
					}	
					else if ( (do_cnt == do_02)   && ( status == PIDparam3_finish || RxData.turn == turnDo)   ){//RxData.turn  
						CAR_RUN(Zero_GO,40.11, 12.0,				runSpeed,475.0,					 0);
						do_cnt=do_03;
					}	
					else if(do_cnt == do_03 && status == PIDparam3_finish)
					{ 
						
						task=RunToRoughArea_1;
						do_cnt = 0; // 
					}	
			}break;
 
	/******************************************************************************************/
			case RunToRoughArea_1:{ //y=1215
					if (do_cnt == do_00 && status == PIDparam3_finish){ 
						
						CAR_RUN(Correct_GO  , runSpeed,780, runSpeed,780,	 0);
						do_cnt=do_01;
					}
					else if (do_cnt == do_01 && status == PIDparam3_finish)
					{
						do_cnt=do_Arm;
					}
					else if (do_cnt == finish_Arm && status == PIDparam3_finish){ 
						CAR_RUN(Correct_GO  , runSpeed,435, runSpeed,435,	 0);//--------------------
						do_cnt=do_02;
					}	
					else if (do_cnt == do_02  && ( status == PIDparam3_finish  || RxData.turn == turnDo)  ){//RxData.turn  
						CAR_RUN(Zero_GO,40.11, 12.0,				runSpeed,475.0,					 0);
						do_cnt = do_03;
					}
					else if(do_cnt == do_03 && status == PIDparam3_finish)
					{
						
						task = RunToFineArea_1;
						do_cnt = 0; //   
					}
			}break;

			case RunToFineArea_1:{// x= 1410
					if (do_cnt == do_00 && status == PIDparam3_finish){ 
						
						CAR_RUN(Correct_GO  , runSpeed,660, runSpeed,660,	 0);
						do_cnt=do_01;
					}//reach
					else if (do_cnt == do_01 && status == PIDparam3_finish){ 
						do_cnt = do_Arm;
					}
					else if (do_cnt == finish_Arm && status == PIDparam3_finish){ 
						CAR_RUN(Correct_GO  , runSpeed,750, runSpeed,750,	 0);
						do_cnt = do_02;
					}	
					else if ( do_cnt == do_02 && ( status == PIDparam3_finish  || RxData.turn == turnDo)  ){//RxData.turn  
						CAR_RUN(Zero_GO,40.11, 12.0,				runSpeed,475.0,					 0);
						++do_cnt;
					}	
					else if(do_cnt == do_03 && status == PIDparam3_finish)
					{
						
						task = RunToCircle2_Start;
						do_cnt = 0; // 
					}
			}break;

			case RunToCircle2_Start:{ // y=1215
				if (do_cnt == do_00 && status == PIDparam3_finish){ 
					
					CAR_RUN(Correct_GO  , runSpeed,310, runSpeed,310,	 0);
					do_cnt = do_01;
				}//reach turn 
				else if (do_cnt == do_01 &&   status == PIDparam3_finish   ){//RxData.turn  
					CAR_RUN(Correct_GO  , runSpeed,905, runSpeed,905,	 0);
					++do_cnt;
				}//reach start 
				else if (do_cnt == do_02 && ( RxData.turn == turnDo ||  status == PIDparam3_finish) )
				{
					CAR_RUN(Zero_GO,40.11, 12.0,				runSpeed,475.0,					 0);
					++do_cnt;
				}
				else if(do_cnt == do_03 && status == PIDparam3_finish)
				{
					
					task = RunToRawMaterial_2_Pick;
					do_cnt = 0;  
				}
			}break;
			
			case RunToRawMaterial_2_Pick:{// x=1410
					if (do_cnt == do_00 && status == PIDparam3_finish ){
						
						CAR_RUN(Correct_GO  , CorrectSpeed,1170, CorrectSpeed,1170,	 0);
						do_cnt = do_01;
					}//reach  
					else if (do_cnt == do_01  &&  status == PIDparam3_finish)
					{
						do_cnt=do_Arm;
					}
					else if (do_cnt == finish_Arm && status == PIDparam3_finish){ 
						CAR_RUN(Correct_GO  , runSpeed,240, runSpeed,240,	 0);//--------------------
						do_cnt=do_02;
					}	
					else if ( (do_cnt == do_02)   && ( status == PIDparam3_finish || RxData.turn == turnDo)   ){//RxData.turn      
						CAR_RUN(Zero_GO,40.11, 12.0,				runSpeed,475.0,					 0);
						++do_cnt;
					}	
					else if(do_cnt == do_03 && status == PIDparam3_finish)
					{ 
						
						task=RunToRoughArea_2;
						do_cnt = 0; // 
					}	
			}break;

			case RunToRoughArea_2:{//   y=1215
					if (do_cnt == do_00 && status == PIDparam3_finish){ 
						
						CAR_RUN(Correct_GO  , runSpeed,700, runSpeed,700,	 0);
						do_cnt=do_01;
					}
					else if (do_cnt == do_01 && status == PIDparam3_finish)
					{
						do_cnt=do_Arm;
					}
					else if (do_cnt == finish_Arm && status == PIDparam3_finish){ 
						CAR_RUN(Correct_GO  , runSpeed,515, runSpeed,515,	 0);//--------------------
						do_cnt=do_02;
					}	
					else if (do_cnt == do_02  && ( status == PIDparam3_finish  || RxData.turn == turnDo)  ){//RxData.turn      
						CAR_RUN(Zero_GO,40.11, 12.0,				runSpeed,475.0,					 0);
						do_cnt=do_03;
					}	
					else if(do_cnt == do_03 && status == PIDparam3_finish)
					{
						task = RunToFineArea_2;
						do_cnt = 0; //   
					}
			}break;

			case RunToFineArea_2:{//   x= 1410
 
					if (do_cnt == do_00 && status == PIDparam3_finish){ 
						
						CAR_RUN(Correct_GO  , runSpeed,660, runSpeed,660,	 0);
						do_cnt=do_01;
					}//reach
					else if (do_cnt == do_01 && status == PIDparam3_finish){ 
						do_cnt = do_Arm;
					}
					else if (do_cnt == finish_Arm && status == PIDparam3_finish){ 
						CAR_RUN(Correct_GO  , runSpeed,765, runSpeed,765,	 0);
						do_cnt = do_02;
					}	
					else if ( do_cnt == do_02 && ( status == PIDparam3_finish  || RxData.turn == turnDo)  ){//RxData.turn      
						CAR_RUN(Zero_GO,40.11, 12.0,				runSpeed,475.0,					 0);
						++do_cnt;
					}	
					else if(do_cnt == do_03 && status == PIDparam3_finish)
					{
						
						task = RunToRest;
						do_cnt = 0; // 
					}
			}break;

			case RunToRest:{// y =1215 + 
					if (do_cnt == do_00 && status == PIDparam3_finish){ 
						CAR_RUN(Correct_GO  , runSpeed,300, runSpeed,300,	 0);
						++do_cnt;
					}
					else if (do_cnt == do_01 && status == PIDparam3_finish){ 
						CAR_RUN(Correct_GO  , runSpeed,890, runSpeed,890,	 0);
						++do_cnt;
					}
					else if (do_cnt == do_02 && ( status == PIDparam3_finish  || RxData.turn == turnDo)  ){//RxData.turn 
						CAR_RUN(Zero_GO,LowSpeed,275,              LowSpeed*0.3,275*0.3,  0);
						++do_cnt;
					}								
					else if (do_cnt == do_03 && (status == PIDparam3_finish)){
						CAR_RUN(Zero_GO,LowSpeed*0.3,275*0.3,	    LowSpeed,275,			0);
						++do_cnt;
					}			
					else if (do_cnt == do_04 && (status == PIDparam3_finish)){
						CAR_RUN(Zero_GO,LowSpeed , 165,	    LowSpeed, 165,			0);
						++do_cnt;
					}	
					else if(do_cnt == do_05 && status == PIDparam3_finish)
					{
						task = Stop;
						do_cnt = 0; //
					}
			}break;
			
			default:
				break;
		}
	}	
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	Top_PID_Control_CallBack_TIM6( htim);//20ms
	Top_Action_Select_CallBack_TIM7( htim); //20ms

	if(htim->Instance == TIM10)
	{
		TimeCntFlag = 0;
    	++TimeCnt100ms ;
		if (TimeCnt100ms > cntMax)
		{
			HAL_TIM_Base_Stop_IT(&htim10);
			 __HAL_TIM_SET_COUNTER(&htim10,0);
			 TimeCntFlag = 1;
			 TimeCnt100ms = 0;
		}
	}
}

int TimeCntFlag ;
int TimeCnt100ms ;
int cntMax;
/**
 * @brief 设定计数秒数，到达指定秒数； ReadTimeFlag()返回1
 * 
 * @param s 秒数
 */
void StartTimeCnt(int s)
{
    TIM10->CNT = 0;
    TimeCntFlag = 0;
    TimeCnt100ms = 0;
	cntMax = s*10;
    __HAL_TIM_SET_COUNTER(&htim10,0);
    HAL_TIM_Base_Start_IT(&htim10);
}

char ReadTimeFlag()
{
    if (TimeCntFlag == 1)
    {
		HAL_TIM_Base_Stop_IT(&htim10);
        TIM10->CNT = 0;
        TimeCntFlag = 0;
        TimeCnt100ms = 0;
        __HAL_TIM_SET_COUNTER(&htim10,0);
		
        return 1;
    }
    else 
    {
        return 0;      
    }
}

void ClearTimeFlag()
{
		HAL_TIM_Base_Stop_IT(&htim10);
        TIM10->CNT = 0;
        TimeCntFlag = 0;
        TimeCnt100ms = 0;
        __HAL_TIM_SET_COUNTER(&htim10,0);
}


