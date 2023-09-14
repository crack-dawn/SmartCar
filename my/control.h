#ifndef __CONTROL_H
#define __CONTROL_H
#include "main.h"
 
#include "ano.h"
#include "my_uart.h"

#include "PID.h"
#include "motor.h"
#include "act_motor.h"
#include "logic.h"

#include "StepMotor.h"
#include "Servo.h"

 

extern int task  ; //�������ʾ���
extern int do_cnt  ;//���������� ������ʶ���
extern unsigned char B_updataFlag;
#define ReturnToCarRun    do{ SendCmdA;  HAL_Delay(15); ClearRxData; do_cnt=finish_Arm;    }while(0) //�ӻ�е�۶����ص�Ѳ��ģʽ

void Other_Actions();


/*PID���ز��� �� ���غ���   

debug PID ���������

ϵͳͳһת��Ϊ���������д����������ֱ��ʸߣ����ƾ��ȸ� */
extern int rpm;   //�ٶ�   5~15~35~45������ȫ����
extern float num; //������  9000


#endif








// extern int Target_Velocity,Reality_Velocity;   /* Ŀ���ٶȣ�ʵ���ٶ� */
// extern int Target_Position,Reality_Position;   /* Ŀ��λ�ã�ʵ��λ�� */
/*---------------------------------------------------------------*/
/* �㷨�ο�ԭ��*/
/* PID�㷨�ο�ԭ��*/
//int Position_PID(int reality,int target);			//λ��ʽ �ο�ԭ��
//int Incremental_PID(int reality,int target); 	//����ʽ �ο�ԭ��  

// /**************************************************************************
// �������ܣ�λ��ʽPID������
// ��ڲ�����ʵ��λ�ã�Ŀ��λ��
// ����  ֵ�����PWM
// ����λ��ʽ��ɢPID��ʽ 
// λ�û�out=    Kp*e(k)+          Ki*��e(k)+   Kd[e(k)-e(k-1)]
// e(k)������ƫ�� 
// e(k-1)������һ�ε�ƫ��  
// ��e(k)����e(k)�Լ�֮ǰ��ƫ����ۻ���;����kΪ1,2,...,k;
// pwm�������
// **************************************************************************/
// int Position_PID(int reality,int target)
// { 	
//     static float Bias,Pwm,Last_Bias,Integral_bias=0;
    
//     Bias=target-reality;                            /* ����ƫ�� */
//     Integral_bias+=Bias;	                        	/* ƫ���ۻ� */
    
//     if(Integral_bias> 5000) Integral_bias = 5000;   /* �����޷� */
//     if(Integral_bias<-5000) Integral_bias =-5000;
    
//     Pwm = (Position_KP*Bias)                        /* �������� */
//          +(Position_KI*Integral_bias)               /* ���ֻ��� */
//          +(Position_KD*(Bias-Last_Bias));           /* ΢�ֻ��� */
    
//     Last_Bias=Bias;                                 /* �����ϴ�ƫ�� */
//     return Pwm;                                     /* ������ */
// }

// /**************************************************************************
// �������ܣ�����PID������
// ��ڲ�����ʵ��ֵ��Ŀ��ֵ
// ����  ֵ�����PWM
// ��������ʽ��ɢPID��ʽ 
// �ٶȻ�out=    Kp[e(k)-e(k-1)]+  Ki*e(k) +   Kd[e(k)-2e(k-1)+e(k-2)]
// e(k)������ƫ�� 
// e(k-1)������һ�ε�ƫ��  �Դ����� 
// pwm�����������
// **************************************************************************/
// int Incremental_PID(int reality,int target)
// { 	
// 	 static float Bias,Pwm,Last_bias=0,Prev_bias=0;
    
// 	 Bias=target-reality;                                   /* ����ƫ�� */
    
// 	 Pwm += (Incremental_KP*(Bias-Last_bias))               	/* �������� */
//            +(Incremental_KI*Bias)                           /* ���ֻ��� */
//            +(Incremental_KD*(Bias-2*Last_bias+Prev_bias));  /* ΢�ֻ��� */ 
    
// 	 Prev_bias=Last_bias;       /* �������ϴ�ƫ�� */
// 	 Last_bias=Bias;	        /* ������һ��ƫ�� */
    
// 	 return Pwm; 		/* ������ */
// }



