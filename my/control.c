
#include "control.h"
#include "StepMotor.h"
#include "RobTask.h"

int task = 0;  /*С����������*/
int do_cnt = 0;
unsigned char B_updataFlag = 0;

short 	 clawTask=0;      /*��е�۵�������*/
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
  {//��һȦɨ��
        ScanCode();
        LED1_ON;
        StepArm_Task_ScanCode(); //ɨ��ʱ������е�۷���ɨ��

        if (RxData.codeFlag == codeOK)
        {
          do_cnt = finish_Scancode;
        }
  }
  else if (task == RunToRawMaterial_1_Pick && do_cnt == do_Arm)
  {//��һȦװ����
    SendCmdB;  //HAL_Delay(200);//ʶ�����ģʽ

    StepArm_Task_Pan(RxData.Task1) ; //ԭ����
    ReturnToCarRun;
  }

  else if (task == RunToRoughArea_1 && do_cnt == do_Arm)
  {//��һȦ�ּӹ�
    SendCmdC; //HAL_Delay(200);   //ʶ��Բ��ģʽ

    StepArm_Task_Jiagon(RxData.Task1); //�ּӹ���
    ReturnToCarRun;
  }

  else if(task == RunToFineArea_1 && do_cnt == do_Arm)
  {
    //��һȦϸ�ӹ�
    SendCmdC; //HAL_Delay(200);  //ʶ��Բ��ģʽ

    StepArm_Task_Over(RxData.Task1);  //ϸ�ӹ���
    ReturnToCarRun;
  }
}















int   rpm=0;  	 
float num=0; 	 

 
//113cm * 135cm �ۼ��280
#define Length 1320
#define Width  1120
#define TURN_90 570
#define TURN_180 1180
#define TargetV 50
 
 

void Top_Action_Select_CallBack_TIM7(TIM_HandleTypeDef *htim)
{
	static  unsigned char status;
	static  unsigned char cnt=0;
	if(htim->Instance == TIM7)  //TIM7 40ms kernel
	{	
		/*------- LCDˢ�� ------------*/
		++cnt;
		if (cnt == 25){
			cnt = 0;
			UART4_LCD_UpdataDisplay();
		}
		/*------- LCDˢ�� ------------*/

		/*------- ������� -----------*/
		status =PID_JudgeStatus();
		switch (task)
		{// 8.2->9.8
			case Stop:{
				if (do_cnt== do_00){
					CAR_RUN(Wait,runSpeed,0,				runSpeed,0,		0);
					++do_cnt;
				}
			}break;

			case BackOut_1:{//����			
						
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
						do_cnt = 0; //�����������
					}
			}break;

	/******************************************************************************************/
			case ScanCode_1:{//�ߵ�ɨ���ͣ��, ɨ��֮�����
					if (do_cnt == do_00 && status == PIDparam3_finish ){
						ScanCode();
						CAR_RUN(Correct_GO  , CorrectSpeed,520, CorrectSpeed,520,	 0);
						++do_cnt;
					}//reach  ��������ɨ��
					else if(do_cnt == do_01 && status == PIDparam3_finish)
					{
						 do_cnt = do_ScanCode;
					}
					else if(do_cnt == finish_Scancode && status == PIDparam3_finish )
					{
						task=RunToRawMaterial_1_Pick;
						do_cnt = 0; //ɨ���������
					}
			}break;

			case RunToRawMaterial_1_Pick:{//�ߵ� ԭ������ �������� ��Ȼ��ת��������һ��ֱ��
					if (do_cnt == do_00 && status == PIDparam3_finish){ 
						CAR_RUN(Correct_GO  , runSpeed,540, runSpeed,540,	 0);//���� -250
						do_cnt=do_01;
					}//������ ��������
					else if (do_cnt == do_01  &&  status == PIDparam3_finish)
					{
						do_cnt=do_Arm;
					}
					else if (do_cnt == finish_Arm && status == PIDparam3_finish){ 
						CAR_RUN(Correct_GO  , runSpeed,360, runSpeed,360,	 0);//--------------------
						do_cnt = do_02;
					}	
					else if ( (do_cnt == do_02)   && ( status == PIDparam3_finish || RxData.turn == turnDo)   ){//RxData.turn ֱ������ ������ת���־ ֱ��ת��
						CAR_RUN(Zero_GO,runSpeed/2, 9.8,				runSpeed,475,					 0);
						do_cnt=do_03;
					}	
					else if(do_cnt == do_03 && status == PIDparam3_finish)
					{ 
						task=RunToRoughArea_1;
						do_cnt = 0; //ԭ���������������������
					}	
			}break;

	/******************************************************************************************/
			case RunToRoughArea_1:{//�ܵ��ּӹ��� ���ã�ץȡ��ת�������һ��ֱ��
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
					else if (do_cnt == do_02  && ( status == PIDparam3_finish  || RxData.turn == turnDo)  ){//RxData.turn ֱ������ ������ת���־ ֱ��ת��
						CAR_RUN(Zero_GO,runSpeed/2, 9.8,				runSpeed,475,					 0);
						do_cnt = do_03;
					}
					else if(do_cnt == do_03 && status == PIDparam3_finish)
					{
						task = RunToFineArea_1;
						do_cnt = 0; //�ּӹ� ����
					}
			}break;

			case RunToFineArea_1:{//�ܵ����ӹ��� ���ã�ת�������һ��ֱ��
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
					else if ( do_cnt == do_02 && ( status == PIDparam3_finish  || RxData.turn == turnDo)  ){//RxData.turn ֱ������ ������ת���־ ֱ��ת��
						CAR_RUN(Zero_GO,runSpeed/2, 9.8,				runSpeed,475,					 0);
						++do_cnt;
					}	
					else if(do_cnt == do_03 && status == PIDparam3_finish)
					{
						task = RunToCircle2_Start;
						do_cnt = 0; //���������������
					}
			}break;

			case RunToCircle2_Start:{ //�ص��ڶ�Ȧ��ʼ��
				if (do_cnt == do_00 && status == PIDparam3_finish){ 
					CAR_RUN(Correct_GO  , runSpeed,310, runSpeed,310,	 0);
					do_cnt = do_01;
				}//reach turn 
				else if (do_cnt == do_01 &&   status == PIDparam3_finish   ){//RxData.turn ֱ������ ������ת���־ ֱ��ת��
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
			
			case RunToRawMaterial_2_Pick:{//�ڶ�Ȧ��ԭ������ץȡװ�����ϣ�װ����ת�������һ��ֱ��
					if (do_cnt == do_00 && status == PIDparam3_finish ){
						CAR_RUN(Correct_GO  , CorrectSpeed,1170, CorrectSpeed,1170,	 0);
						do_cnt = do_01;
					}//reach ɨ��㸽��
					else if (do_cnt == do_01  &&  status == PIDparam3_finish)
					{
						do_cnt=do_Arm;
					}
					else if (do_cnt == finish_Arm && status == PIDparam3_finish){ 
						CAR_RUN(Correct_GO  , runSpeed,260, runSpeed,260,	 0);//--------------------
						do_cnt=do_02;
					}	
					else if ( (do_cnt == do_02)   && ( status == PIDparam3_finish || RxData.turn == turnDo)   ){//RxData.turn ֱ������ ������ת���־ ֱ��ת��
						CAR_RUN(Zero_GO,runSpeed/2, 9.8,				runSpeed,475,					 0);
						++do_cnt;
					}	
					else if(do_cnt == do_03 && status == PIDparam3_finish)
					{ 
						task=RunToRoughArea_2;
						do_cnt = 0; //ԭ���������������������
					}	
			}break;

			case RunToRoughArea_2:{//�ڶ�Ȧ �ܵ��ּӹ��� ���ã�ץȡ��ת�������һ��ֱ��
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
					else if (do_cnt == do_02  && ( status == PIDparam3_finish  || RxData.turn == turnDo)  ){//RxData.turn ֱ������ ������ת���־ ֱ��ת��
						CAR_RUN(Zero_GO,runSpeed/2, 9.8,				runSpeed,475,					 0);
						do_cnt=do_03;
					}	
					else if(do_cnt == do_03 && status == PIDparam3_finish)
					{
						task = RunToFineArea_2;
						do_cnt = 0; //�ּӹ� ����
					}
			}break;

			case RunToFineArea_2:{//�ܵ����ӹ��� ���ã�ת�������һ��ֱ��
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
					else if ( do_cnt == do_02 && ( status == PIDparam3_finish  || RxData.turn == turnDo)  ){//RxData.turn ֱ������ ������ת���־ ֱ��ת��
						CAR_RUN(Zero_GO,runSpeed/2, 9.8,				runSpeed,475,					 0);
						++do_cnt;
					}	
					else if(do_cnt == do_03 && status == PIDparam3_finish)
					{
						task = RunToRest;
						do_cnt = 0; //���������������
					}
			}break;

			case RunToRest:{//�ߵ����ͣ����
					if (do_cnt == do_00 && status == PIDparam3_finish){ 
						CAR_RUN(Correct_GO  , runSpeed,300, runSpeed,300,	 0);
						++do_cnt;
					}
					else if (do_cnt == do_01 && status == PIDparam3_finish){ 
						CAR_RUN(Correct_GO  , runSpeed,890, runSpeed,890,	 0);
						++do_cnt;
					}
					else if (do_cnt == do_02 && ( status == PIDparam3_finish  || RxData.turn == turnDo)  ){//RxData.turn ֱ������ ������ת���־ ֱ��ת��
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
						do_cnt = 0; //���������������
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
}



