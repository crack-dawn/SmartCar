#include "PID.h"
#include "control.h"

//pid ����ֵ����
#define  Abs(x)   ((x) < 0 ? -(x) : (x))  //�����ֵ 


//����ȫ�ֱ���
/**
 * @brief pid_speed  pid_location ��Ӧ ���1
 * @brief pid_speed2 pid_location2     ���2
 */
_pid pid_speed={0};  
_pid pid_location={0};

_pid pid_speed2={0};   
_pid pid_location2={0};

_pid pid_angle={0};

/*PIDԭʼ������ֵ*/
// 0.9-0.0016 5000 0.6    1.8-2.2
// 												8.5~7.5
float Position_KP= 2.6,Position_KI=0.00,Position_KD=9.7;          /* λ��ʽPIDϵ�� position for location*/
//2.6  5.2  1.3 
float Incremental_KP=2.6,Incremental_KI=5.2,Incremental_KD=1.3;    /* ����ʽPIDϵ�� increase for speed*/   
float Incremental_KP2=2.6,Incremental_KI2=5.2,Incremental_KD2=1.3;    /* ����ʽPIDϵ�� increase for speed*/   
// float Incremental_KP=2.6,Incremental_KI=7.2,Incremental_KD=0.6;    /* ����ʽPIDϵ�� increase for speed*/   
float Incremental_KP_A=2.3, Incremental_KI_A=0.02, Incremental_KD_A=2.0;		  /* ����ʽPIDϵ�� increase for angle*/
/**------------------------------״̬����-------------------------------------------**/
/*û���õ����ٴ��� ���ǲ�ȡ�ֶ�ʽpid�� ��Ŀ��λ�ø�������λ�û�pֵ��С���ٶȻ�iֵ��С*/


// /*PIDԭʼ������ֵ*/
// float Position_KP=2.0,Position_KI=0.001,Position_KD=0.6;          /* λ��ʽPIDϵ�� position for location*/
// float Incremental_KP=1.6,Incremental_KI=5.00,Incremental_KD=0.6;    /* ����ʽPIDϵ�� increase for speed*/   
// float Incremental_KP_A=2.3, Incremental_KI_A=0.02, Incremental_KD_A=2.;		  /* ����ʽPIDϵ�� increase for angle*/
// /**------------------------------״̬����-------------------------------------------**/
// /*û���õ����ٴ��� ���ǲ�ȡ�ֶ�ʽpid�� ��Ŀ��λ�ø�������λ�û�pֵ��С���ٶȻ�iֵ��С*/


/**
 * @brief 
 * 
 * @param Lout λ�û����
 * @param Lerr λ�����
 * @param minV ��Сֵspeed
 * @param maxV ���ֵspeed
 * @return float 
 */
float Limit_Amplitude(float Lout, float Lerr,float minV, float maxV)
{
		// float distance = Abs(Lerr);
		Lout = 0.1*Lout;
		if( Abs(Lout) > maxV) // too more
		{
			return (Lout>0)?(maxV):(-maxV);
		}
		else if( Abs(Lout) < minV ) // too min
		{
			return (Lout>0)?(minV):(-minV);
		}
		else // mormal
		{
			return Lout;
		}
}





void Top_PID_Control_CallBack_TIM6(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM6)  //TIM6 20 ms kernel
	{
		#if(Motor_REDUCE== 30)
			pid_speed2.actual_val   =  +(short)TIM3 -> CNT;   	//TIM3   PC6  PC7  E1 ������| PE9   pwm1 | AN1 AN2| pid_speed2  �ұ�   M1 
			pid_speed.actual_val  	=  -(short)TIM4 -> CNT;   	//TIM4   PD12 PD13 E2 ������| PE11  pwm2 | BN1 BN2| pid_speed	���   M2
		#endif // 0

		#if(Motor_REDUCE== 49)
			pid_speed.actual_val  	=  +(short)(TIM4 -> CNT);   	//TIM4   PD12 PD13 E2 ������| PE11  pwm2 | BN1 BN2| pid_speed	���   M2
			pid_speed2.actual_val   =  -(short)(TIM3 -> CNT);   	//TIM3   PC6  PC7  E1 ������| PE9   pwm1 | AN1 AN2| pid_speed2  �ұ�   M1 
		#endif // 0

		// printf("pwm mode|| cnt of TIM4:  %d TIM3:  %d \r\n",TIM4 -> CNT, TIM3->CNT);
 
		TIM4 -> CNT=0; 
		TIM3 -> CNT=0; 
		pid_angle.actual_val = (float)RxData.angle; 
		pid_location2.actual_val  += pid_speed2.actual_val; 
		pid_location.actual_val   += pid_speed.actual_val; 

		if(pid_angle.param5 == PIDparam5_Angle_Control && pid_location.target_val ==pid_location2.target_val  )
		{  
			if ((Abs(pid_location2.err) < Err_disance+Err_disance || Abs(pid_location.err) < Err_disance+Err_disance ) )
			{
				pid_speed.out = pid_speed2.out;//������Ϊ��׼ ֱ����ʻ����ֹͣ��ֹͣ
			}
			// if (((Abs(pid_location2.err) < 700 ) || (Abs(pid_location.err) <700 )) \
			//      &&((Abs(pid_location2.err) >Err_disance   ) || (Abs(pid_location.err) >Err_disance))  \
			// 	 && pid_location2.param5 == 1 &&  pid_location.param5== 1)
			// {//ÿ��ִ�ж����� �ӽ�����λ�� ��������
			// 	 pid_location2.param5 = 0;
			// 	 pid_location.param5= 0;
			// 	pid_speed.pre_target_val = LowSpeed;
			// 	pid_speed2.pre_target_val = LowSpeed;
			// }
		}
		PID_Calculate_Updata(&pid_location,  &pid_speed, &pid_angle);
		PID_Calculate_Updata(&pid_location2, &pid_speed2, &pid_angle);
		
		Set_Pwm(pid_speed2.out,pid_speed.out);	 // ��2Ϊpwm1  ��1Ϊpwm2
	}
}


float Angle_adjust(float angle)
{
		// �Ƕ�ֵ ��ƫΪ+ //��ƫΪ-  // �ٶ����ֵ ��ԭ����10����
	float AbsAngle = Abs(angle)*100;
	if ( AbsAngle > 45 && AbsAngle < 80 )
	{
		// return Limit(angle*3,2.5);
		return Limit(angle*3,2.5);
	}
	else if ( AbsAngle > 80 && AbsAngle < 120 )
	{
		// return Limit(angle*3, 4.0);
		return Limit(angle*3, 3.5);
	}
	else if ( AbsAngle > 120 &&  AbsAngle < 212)
	{
		// return Limit(angle*3, 5.5);
		return Limit(angle*2.4, 4);
	}
	else if ( AbsAngle > 211 &&  AbsAngle < 300)
	{
		return Limit(angle*2, 5.5);
	}
	else if (  AbsAngle >= 300)
	{
		// return Limit(angle*2,6.6);
		return Limit(angle*2,6.1);
	}
	else
	{
		return 0;
	}
	
}

/**
 * @brief �˺���ֻ����PID���� ˢ��״̬��ˢ�����ֵ��||����˵��ֻˢ�����ݲ����������
 * 
 * @param pid_L 
 * @param pid_V 
 * @param speed //����ٶ����Ʒ���
 */
void PID_Calculate_Updata(_pid *pid_L, _pid *pid_V, _pid *pid_A)
{
	static float adjust=0;
	//��ʼ����ǰ���趨�� Ŀ��λ�� �� Ŀ���ٶ�
    /*λ�û����㿪ʼ-----------------------*/	
	location_pid_realize( pid_L ); //�õ���ǰ �����ٶ�ֵ
    /*λ�û��������*/

	//��λ�û�������� ���ʵĵ��ڣ� �������������Ʒ��ȵȵȣ�������ٶȻ�����Ŀ���ٶ�
	pid_V->target_val= Limit_Amplitude(  pid_L->out,  pid_L->err , MinSpeed, pid_V->pre_target_val );
	


	/*�ǶȻ����㿪ʼ �������ֲ���*/  // 
	if (pid_angle.param5 == PIDparam5_Angle_Control)
	{	
		adjust = Angle_adjust(pid_angle.actual_val);
		// �Ƕ�ֵ ��ƫΪ+ //��ƫΪ-  // �ٶ����ֵ ��ԭ����10����
		if (pid_V->param1 == PIDparam1_M2_Left)
		{
			pid_V->target_val += adjust;
			// if (pid_angle.actual_val > 0 )
			// {
			// 	pid_V->target_val -= 0.8;
			// }
		}	
		else if (pid_V->param1 == PIDparam1_M1_Right)
		{
			pid_V->target_val -= adjust;			
		}
	}

	/*�ǶȻ���������*/
	else if (pid_angle.param5 == PIDparam5_Speed_Control)
	{
		pid_V->target_val = pid_V->pre_target_val; 
	}
	
    /*�ٶȻ����㿪ʼ-----------------------*/
    Increase_pid_realize(pid_V);
    /*�ٶȻ��������*/			

	/*״̬��������*/
	PID_Param_Updata(    pid_L,  pid_V  ) ;
}

void PID_Param_Updata(_pid *pid_L, _pid *pid_V)
{
	switch (pid_angle.param5)
	{
		case PIDparam5_Angle_Control:
		case PIDparam5_Local_Control:
			if (Abs(pid_L->err) < Err_disance) /* �˳����ָ���,λ�����Err������ֵֹͣpid���� */
			{
				pid_L->param4 += 1;
				if (pid_L->param4 > PIDparam4_ready_quick) // buffer time after action over
				{
					pid_L->param4 = 0;
					pid_L->pre_target_val = pid_L->target_val; // updata previous location, storage it for run to next position
					pid_L->param3 = PIDparam3_finish;		   // ���ζ�������� ��־λ����   ͳһֻ��λ�û����������Ϊ�жϱ�־
				}
				if (Abs(pid_L->err) < _1mm_to_Encoder_)
				{
					pid_V->out = 0; // ���������� 	ok
				}
				return;
			}break;
		case PIDparam5_Speed_Control:
			if (  Abs( pid_V->actual_val - pid_V->target_val  ) < 2 )
			{
				pid_V->param3 = PIDparam3_finish;
				return;
			}break;

		default:break;
	}
	
		pid_angle.param4 = 0 ;
		pid_angle.param3 = PIDparam3_doing;
		pid_L->param4 = 0 ;
		pid_L->param3 = PIDparam3_doing;
}

/*judge status  only by pid_location.param3 and  pid_location2.param3 , ignore pid_speed.param3*/
unsigned char PID_JudgeStatus()
{
	switch (pid_angle.param5)
	{
	default:
	case 		PIDparam5_Angle_Control:
	case 		PIDparam5_Local_Control:
		if(pid_location.param3==PIDparam3_finish  && pid_location2.param3==PIDparam3_finish  )
		{
			return PIDparam3_finish;
		}
		else if (pid_speed.pre_target_val ==pid_speed2.pre_target_val)
		{ //��λ����ֱ��  �ӽ�Ŀ��λ��  ���ֺ������ٶȱ���һ�£������ֽ���Ϊ��־
			if(pid_location2.param3 == PIDparam3_finish)
				return PIDparam3_finish;
		}
		break;
	case PIDparam5_Speed_Control:
		if ( pid_speed.param3==PIDparam3_finish  && pid_speed.param3==PIDparam3_finish)
		{
			return PIDparam3_finish;
		}
		break;
	}
	return PIDparam3_doing;
}



/**************************************************************************
�������ܣ�����PID������  �ǶȻ� �ǶȲ��ٿ���
��ڲ�����ʵ��ֵ��Ŀ��ֵ
����  ֵ�����������ֵ
��������ʽ��ɢPID��ʽ 
out+=Kp[e��k��-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)��������ƫ�� 
e(k-1)������һ�ε�ƫ��  �Դ����� 
out�����������
**************************************************************************/
float Increase_pid_realize_angle(_pid *pid)
{
//	pid->actual_val =actual_val;//ʵ��ֵ  
//  pid->target_val =target_val;//����ֵ  

  pid->err=pid->target_val - pid->actual_val; /*��ǰ���*/ /*����Ŀ��ֵ��ʵ��ֵ�����*/

  /*PID�㷨ʵ��*/
  if (Abs(pid->err) < Err_angle)//��ֹ P��ʹ�ٶȹ���
  {
			pid->out += pid->Kp * (pid->err - pid->err_last)					   /* �������� */
						+ 0.002 * (pid->err)										   /* ���ֻ��� */
						+ pid->Kd * (pid->err - 2 * pid->err_last + pid->err_pre); /* ΢�ֻ��� */
  }
 
  else
  {
			pid->out += pid->Kp * (pid->err - pid->err_last)					   /* �������� */
						+ pid->Ki * (pid->err)									   /* ���ֻ��� */
						+ pid->Kd * (pid->err - 2 * pid->err_last + pid->err_pre); /* ΢�ֻ��� */
  }

  /*����*/
  pid->err_pre = pid->err_last; // �������ϴ����
  pid->err_last = pid->err;		// �����ϴ����

  /*���ص�ǰʵ��ֵ PID���ں�����ֵ Ϊ�ٶ�ռ�ձ�*/
  return pid->out;
}

/**************************************************************************
�������ܣ�����PID������  �ٶȻ�
��ڲ�����ʵ��ֵ��Ŀ��ֵ
����  ֵ�����PWM
��������ʽ��ɢPID��ʽ 
out+=Kp[e��k��-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)��������ƫ�� 
e(k-1)������һ�ε�ƫ��  �Դ����� 
out�����������
**************************************************************************/
float Increase_pid_realize(_pid *pid)
{
//	pid->actual_val =actual_val;//ʵ��ֵ  
//  pid->target_val =target_val;//����ֵ  

  pid->err=pid->target_val - pid->actual_val; /*��ǰ���*/ /*����Ŀ��ֵ��ʵ��ֵ�����*/

  /*PID�㷨ʵ��*/
  if (Abs(pid->err) < Err_speed)//��ֹ P��ʹ�ٶȹ���
  {
			pid->out += pid->Kp * (pid->err - pid->err_last)					   /* �������� */
						+ 2.2 * (pid->err)										   /* ���ֻ��� */
						+ pid->Kd * (pid->err - 2 * pid->err_last + pid->err_pre); /* ΢�ֻ��� */
  }
 
  else
  {
			pid->out += pid->Kp * (pid->err - pid->err_last)					   /* �������� */
						+ pid->Ki * (pid->err)									   /* ���ֻ��� */
						+ pid->Kd * (pid->err - 2 * pid->err_last + pid->err_pre); /* ΢�ֻ��� */
  }

  /*����*/
  pid->err_pre = pid->err_last; // �������ϴ����
  pid->err_last = pid->err;		// �����ϴ����

  /*���ص�ǰʵ��ֵ PID���ں�����ֵ Ϊ�ٶ�ռ�ձ�*/
  return pid->out;
}

/**************************************************************************
�������ܣ�λ��ʽPID������
��ڲ�����ʵ��λ�ã�Ŀ��λ��
����  ֵ�������ٶ�
����λ��ʽ��ɢPID��ʽ 
pwm=Kp*e(k)+Ki*��e(k)+Kd[e��k��-e(k-1)]
e(k)��������ƫ�� 
e(k-1)������һ�ε�ƫ��  
��e(k)����e(k)�Լ�֮ǰ��ƫ����ۻ���;����kΪ1,2,...,k;
pwm�������
**************************************************************************/
float location_pid_realize(_pid *pid)   
{
//    pid->actual_val =actual_val;//ʵ��ֵ ʵ�ʵ�λ��
//    pid->target_val =target_val;//����ֵ ������λ��

		/*����Ŀ��ֵ��ʵ��ֵ�����*/
    pid->err= pid->target_val  -  pid->actual_val; //��ǰ��� P  ������
  
    pid->integral += pid->err;          //  ��ʷ�ۼ���� I ������

    if(pid->integral> 500) pid->integral = 500;   /* �����޷� */
    if(pid->integral<-500) pid->integral =-500;

		/*PID�㷨ʵ��*/
	// if( Abs(pid->err) < Err_disance*4 )
	// {
	// 	pid->out =      1.6*pid->err
	// 					+pid->Ki*pid->integral
	// 					+pid->Kd*(pid->err-pid->err_last);
	// }
	// else
	{
		pid->out = pid->Kp*pid->err
			+pid->Ki*pid->integral
			+pid->Kd*(pid->err-pid->err_last);
	}
		/*����*/
    pid->err_last=pid->err;
    
		/*���ؿ�����*/
    return pid->out;
}

/*--------------------------------------������ʼ��----------------------------------------------------------------*/
void PID_Struct_Init(_pid *pid,float p, float i, float d,unsigned char param1,unsigned char param2,unsigned char param3)
{
    pid->target_val=0;				
		pid->actual_val=0;
    pid->err=0;
    pid->err_last=0;
    pid->err_pre=0;
    pid->integral=0;
		pid->out = 0;

    pid->Kp = p;    // ���ñ���ϵ�� P
		pid->Ki = i;    // ���û���ϵ�� I
		pid->Kd = d;    // ����΢��ϵ�� D
    pid->param1 = param1; // ������
    pid->param2 = param2; // λ�û� or �ٶȻ���
    pid->param3 = param3; // ��ǰִ�����״̬  
}

void PID_param_init()
{
	PID_Struct_Init(&pid_angle,Incremental_KP_A,Incremental_KI_A,Incremental_KD_A,PIDparam1_An,PIDparam2_I,PIDparam3_finish);
	pid_angle.param5 = PIDparam5_Local_Control;
   /***********���1 PID�ṹ��**********************/
	/* λ����س�ʼ������ */
	PID_Struct_Init(&pid_location,Position_KP,Position_KI,Position_KD,PIDparam1_M2_Left,PIDparam2_L,PIDparam3_finish);
	/* �ٶ���س�ʼ������ */
	PID_Struct_Init(&pid_speed,Incremental_KP,Incremental_KI,Incremental_KD,PIDparam1_M2_Left,PIDparam2_I,PIDparam3_finish);

	/***********���2 PID�ṹ��**********************/
	/* λ����س�ʼ������ */
	PID_Struct_Init(&pid_location2,Position_KP,Position_KI,Position_KD,PIDparam1_M1_Right,PIDparam2_L,PIDparam3_finish);
	/* �ٶ���س�ʼ������ */
	PID_Struct_Init(&pid_speed2,Incremental_KP2,Incremental_KI2,Incremental_KD2,PIDparam1_M1_Right,PIDparam2_I,PIDparam3_finish);
}

// void set_pid_target(_pid *pid, float temp_val)
// {
//   pid->target_val = temp_val;    // ���õ�ǰ��Ŀ��ֵ
// }

// float get_pid_target(_pid *pid)
// {
//   return pid->target_val;    // ���õ�ǰ��Ŀ��ֵ
// }

// void set_p_i_d(_pid *pid, float p, float i, float d)
// {
//   	pid->Kp = p;    // ���ñ���ϵ�� P
// 		pid->Ki = i;    // ���û���ϵ�� I
// 		pid->Kd = d;    // ����΢��ϵ�� D
// }







 






