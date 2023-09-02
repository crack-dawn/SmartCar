#include "PID.h"
#include "control.h"

//pid 绝对值函数
#define  Abs(x)   ((x) < 0 ? -(x) : (x))  //求绝对值 


//定义全局变量
/**
 * @brief pid_speed  pid_location 对应 电机1
 * @brief pid_speed2 pid_location2     电机2
 */
_pid pid_speed={0};  
_pid pid_location={0};

_pid pid_speed2={0};   
_pid pid_location2={0};

_pid pid_angle={0};

/*PID原始参数数值*/
// 0.9-0.0016 5000 0.6    1.8-2.2
// 												8.5~7.5
float Position_KP= 2.6,Position_KI=0.00,Position_KD=9.7;          /* 位置式PID系数 position for location*/
//2.6  5.2  1.3 
float Incremental_KP=2.6,Incremental_KI=5.2,Incremental_KD=1.3;    /* 增量式PID系数 increase for speed*/   
float Incremental_KP2=2.6,Incremental_KI2=5.2,Incremental_KD2=1.3;    /* 增量式PID系数 increase for speed*/   
// float Incremental_KP=2.6,Incremental_KI=7.2,Incremental_KD=0.6;    /* 增量式PID系数 increase for speed*/   
float Incremental_KP_A=2.3, Incremental_KI_A=0.02, Incremental_KD_A=2.0;		  /* 增量式PID系数 increase for angle*/
/**------------------------------状态更新-------------------------------------------**/
/*没有用到减速带， 而是采取分段式pid， 在目标位置附近，将位置环p值调小，速度环i值调小*/


// /*PID原始参数数值*/
// float Position_KP=2.0,Position_KI=0.001,Position_KD=0.6;          /* 位置式PID系数 position for location*/
// float Incremental_KP=1.6,Incremental_KI=5.00,Incremental_KD=0.6;    /* 增量式PID系数 increase for speed*/   
// float Incremental_KP_A=2.3, Incremental_KI_A=0.02, Incremental_KD_A=2.;		  /* 增量式PID系数 increase for angle*/
// /**------------------------------状态更新-------------------------------------------**/
// /*没有用到减速带， 而是采取分段式pid， 在目标位置附近，将位置环p值调小，速度环i值调小*/


/**
 * @brief 
 * 
 * @param Lout 位置环输出
 * @param Lerr 位置误差
 * @param minV 最小值speed
 * @param maxV 最大值speed
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
		#if(Motor== 1)
			pid_speed2.actual_val   =  +(short)TIM3 -> CNT;   	//TIM3   PC6  PC7  E1 右轮子| PE9   pwm1 | AN1 AN2| pid_speed2  右边   M1 
			pid_speed.actual_val  	=  -(short)TIM4 -> CNT;   	//TIM4   PD12 PD13 E2 左轮子| PE11  pwm2 | BN1 BN2| pid_speed	左边   M2
		#endif // 0

		#if(Motor== 3)
			pid_speed.actual_val  	=  -(short)TIM4 -> CNT;   	//TIM4   PD12 PD13 E2 左轮子| PE11  pwm2 | BN1 BN2| pid_speed	左边   M2
			pid_speed2.actual_val   =  +(short)TIM3 -> CNT;   	//TIM3   PC6  PC7  E1 右轮子| PE9   pwm1 | AN1 AN2| pid_speed2  右边   M1 
		#endif // 0

		TIM4 -> CNT=0; 
		TIM3 -> CNT=0; 
		
		pid_angle.actual_val = (float)RxData.angle; 
		
		pid_location2.actual_val  += pid_speed2.actual_val; 
		pid_location.actual_val   += pid_speed.actual_val; 

		if(pid_angle.param5 == PIDparam5_Angle_Control && pid_location.target_val ==pid_location2.target_val  )
		{  
			if ((Abs(pid_location2.err) < Err_disance+Err_disance || Abs(pid_location.err) < Err_disance+Err_disance ) )
			{
				pid_speed.out = pid_speed2.out;
			}
			// if (((Abs(pid_location2.err) < 700 ) || (Abs(pid_location.err) <700 )) \
			//      &&((Abs(pid_location2.err) >Err_disance   ) || (Abs(pid_location.err) >Err_disance))  \
			// 	 && pid_location2.param5 == 1 &&  pid_location.param5== 1)
			// {//每次执行动作的 接近结束位置 减速慢性
			// 	 pid_location2.param5 = 0;
			// 	 pid_location.param5= 0;
			// 	pid_speed.pre_target_val = LowSpeed;
			// 	pid_speed2.pre_target_val = LowSpeed;
			// }
		}
		PID_Calculate_Updata(&pid_location,  &pid_speed, &pid_angle);
		PID_Calculate_Updata(&pid_location2, &pid_speed2, &pid_angle);
		
		Set_Pwm(pid_speed2.out,pid_speed.out);	 // 右2为pwm1  左1为pwm2
	}
}


float Angle_adjust(float angle)
{
		// 角度值 左偏为+ //右偏为-  // 速度误差值 是原本的10倍数
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
 * @brief 此函数只根据PID参数 刷新状态，刷新输出值，||或者说，只刷新数据不做输出控制
 * 
 * @param pid_L 
 * @param pid_V 
 * @param speed //最大速度限制幅度
 */
void PID_Calculate_Updata(_pid *pid_L, _pid *pid_V, _pid *pid_A)
{
	static float adjust=0;
	//开始控制前先设定好 目标位置 和 目标速度
    /*位置环计算开始-----------------------*/	
	location_pid_realize( pid_L ); //得到当前 期望速度值
    /*位置环计算结束*/

	//对位置环输出进行 合适的调节， 比例缩减，限制幅度等等，输入给速度环当做目标速度
	pid_V->target_val= Limit_Amplitude(  pid_L->out,  pid_L->err , MinSpeed, pid_V->pre_target_val );
	


	/*角度环计算开始 增加两轮差速*/  // 
	if (pid_angle.param5 == PIDparam5_Angle_Control)
	{	
		adjust = Angle_adjust(pid_angle.actual_val);
		// 角度值 左偏为+ //右偏为-  // 速度误差值 是原本的10倍数
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

	/*角度换调整结束*/
	else if (pid_angle.param5 == PIDparam5_Speed_Control)
	{
		pid_V->target_val = pid_V->pre_target_val; 
	}
	
    /*速度环计算开始-----------------------*/
    Increase_pid_realize(pid_V);
    /*速度环计算结束*/			

	/*状态参数调整*/
	PID_Param_Updata(    pid_L,  pid_V  ) ;
}

void PID_Param_Updata(_pid *pid_L, _pid *pid_V)
{
	switch (pid_angle.param5)
	{
		case PIDparam5_Angle_Control:
		case PIDparam5_Local_Control:
			if (Abs(pid_L->err) < Err_disance) /* 滤除部分干扰,位置相差Err个编码值停止pid调节 */
			{
				pid_L->param4 += 1;
				if (pid_L->param4 > PIDparam4_ready_quick) // buffer time after action over
				{
					pid_L->param4 = 0;
					pid_L->pre_target_val = pid_L->target_val; // updata previous location, storage it for run to next position
					pid_L->param3 = PIDparam3_finish;		   // 本次动作完成了 标志位置零   统一只管位置环这个参数作为判断标志
				}
				if (Abs(pid_L->err) < _1mm_to_Encoder_)
				{
					pid_V->out = 0; // 电机输出置零 	ok
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
		{ //定位置走直线  接近目标位置  左轮和右轮速度保持一致，以右轮结束为标志
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
函数功能：增量PID控制器  角度环 角度差速控制
入口参数：实际值，目标值
返回  值：电机差速数值
根据增量式离散PID公式 
out+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  以此类推 
out代表增量输出
**************************************************************************/
float Increase_pid_realize_angle(_pid *pid)
{
//	pid->actual_val =actual_val;//实际值  
//  pid->target_val =target_val;//期望值  

  pid->err=pid->target_val - pid->actual_val; /*当前误差*/ /*计算目标值与实际值的误差*/

  /*PID算法实现*/
  if (Abs(pid->err) < Err_angle)//防止 P项使速度过调
  {
			pid->out += pid->Kp * (pid->err - pid->err_last)					   /* 比例环节 */
						+ 0.002 * (pid->err)										   /* 积分环节 */
						+ pid->Kd * (pid->err - 2 * pid->err_last + pid->err_pre); /* 微分环节 */
  }
 
  else
  {
			pid->out += pid->Kp * (pid->err - pid->err_last)					   /* 比例环节 */
						+ pid->Ki * (pid->err)									   /* 积分环节 */
						+ pid->Kd * (pid->err - 2 * pid->err_last + pid->err_pre); /* 微分环节 */
  }

  /*误差传递*/
  pid->err_pre = pid->err_last; // 更新上上次误差
  pid->err_last = pid->err;		// 更新上次误差

  /*返回当前实际值 PID调节后的输出值 为速度占空比*/
  return pid->out;
}

/**************************************************************************
函数功能：增量PID控制器  速度环
入口参数：实际值，目标值
返回  值：电机PWM
根据增量式离散PID公式 
out+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  以此类推 
out代表增量输出
**************************************************************************/
float Increase_pid_realize(_pid *pid)
{
//	pid->actual_val =actual_val;//实际值  
//  pid->target_val =target_val;//期望值  

  pid->err=pid->target_val - pid->actual_val; /*当前误差*/ /*计算目标值与实际值的误差*/

  /*PID算法实现*/
  if (Abs(pid->err) < Err_speed)//防止 P项使速度过调
  {
			pid->out += pid->Kp * (pid->err - pid->err_last)					   /* 比例环节 */
						+ 2.2 * (pid->err)										   /* 积分环节 */
						+ pid->Kd * (pid->err - 2 * pid->err_last + pid->err_pre); /* 微分环节 */
  }
 
  else
  {
			pid->out += pid->Kp * (pid->err - pid->err_last)					   /* 比例环节 */
						+ pid->Ki * (pid->err)									   /* 积分环节 */
						+ pid->Kd * (pid->err - 2 * pid->err_last + pid->err_pre); /* 微分环节 */
  }

  /*误差传递*/
  pid->err_pre = pid->err_last; // 更新上上次误差
  pid->err_last = pid->err;		// 更新上次误差

  /*返回当前实际值 PID调节后的输出值 为速度占空比*/
  return pid->out;
}

/**************************************************************************
函数功能：位置式PID控制器
入口参数：实际位置，目标位置
返回  值：期望速度
根据位置式离散PID公式 
pwm=Kp*e(k)+Ki*∑e(k)+Kd[e（k）-e(k-1)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  
∑e(k)代表e(k)以及之前的偏差的累积和;其中k为1,2,...,k;
pwm代表输出
**************************************************************************/
float location_pid_realize(_pid *pid)   
{
//    pid->actual_val =actual_val;//实际值 实际的位置
//    pid->target_val =target_val;//期望值 期望的位置

		/*计算目标值与实际值的误差*/
    pid->err= pid->target_val  -  pid->actual_val; //当前误差 P  比例项
  
    pid->integral += pid->err;          //  历史累加误差 I 积分项

    if(pid->integral> 500) pid->integral = 500;   /* 积分限幅 */
    if(pid->integral<-500) pid->integral =-500;

		/*PID算法实现*/
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
		/*误差传递*/
    pid->err_last=pid->err;
    
		/*返回控制量*/
    return pid->out;
}

/*--------------------------------------参数初始化----------------------------------------------------------------*/
void PID_Struct_Init(_pid *pid,float p, float i, float d,unsigned char param1,unsigned char param2,unsigned char param3)
{
    pid->target_val=0;				
		pid->actual_val=0;
    pid->err=0;
    pid->err_last=0;
    pid->err_pre=0;
    pid->integral=0;
		pid->out = 0;

    pid->Kp = p;    // 设置比例系数 P
		pid->Ki = i;    // 设置积分系数 I
		pid->Kd = d;    // 设置微分系数 D
    pid->param1 = param1; // 电机序号
    pid->param2 = param2; // 位置环 or 速度环？
    pid->param3 = param3; // 当前执行完成状态  
}

void PID_param_init()
{
	PID_Struct_Init(&pid_angle,Incremental_KP_A,Incremental_KI_A,Incremental_KD_A,PIDparam1_An,PIDparam2_I,PIDparam3_finish);
	pid_angle.param5 = PIDparam5_Local_Control;
   /***********电机1 PID结构体**********************/
	/* 位置相关初始化参数 */
	PID_Struct_Init(&pid_location,Position_KP,Position_KI,Position_KD,PIDparam1_M2_Left,PIDparam2_L,PIDparam3_finish);
	/* 速度相关初始化参数 */
	PID_Struct_Init(&pid_speed,Incremental_KP,Incremental_KI,Incremental_KD,PIDparam1_M2_Left,PIDparam2_I,PIDparam3_finish);

	/***********电机2 PID结构体**********************/
	/* 位置相关初始化参数 */
	PID_Struct_Init(&pid_location2,Position_KP,Position_KI,Position_KD,PIDparam1_M1_Right,PIDparam2_L,PIDparam3_finish);
	/* 速度相关初始化参数 */
	PID_Struct_Init(&pid_speed2,Incremental_KP2,Incremental_KI2,Incremental_KD2,PIDparam1_M1_Right,PIDparam2_I,PIDparam3_finish);
}

// void set_pid_target(_pid *pid, float temp_val)
// {
//   pid->target_val = temp_val;    // 设置当前的目标值
// }

// float get_pid_target(_pid *pid)
// {
//   return pid->target_val;    // 设置当前的目标值
// }

// void set_p_i_d(_pid *pid, float p, float i, float d)
// {
//   	pid->Kp = p;    // 设置比例系数 P
// 		pid->Ki = i;    // 设置积分系数 I
// 		pid->Kd = d;    // 设置微分系数 D
// }







 






