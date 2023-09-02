#include "main.h"
#include "ano.h"
/* ����V7��λ��Э��	*/
uint8_t Data_Buff[32] ={0XAA,0XFF,0XF1};		    /* ��ʾЭ��  F1��ʾ����֡ID */
uint8_t Data_Sent[32] ={0XAA,0XFF,0XE2};	        /* E1��ʾ���ݶ�ȡID */
uint8_t Data_Check[12]={0XAA,0XFF,0X00};	        /* 00��ʾУ��֡ */

uint8_t Data_Get[32];
 


void Data_send2(int32_t _a,int32_t _b,int32_t _c,int32_t _d,int32_t _e,int32_t _f,int32_t _g,int32_t _h)
{
	uint8_t i,cnt=4;	/* ���� */
	uint8_t sc=0,ac=0;	/* У�� */
	
	Data_Buff[cnt++] = BYTE0(_a);
	Data_Buff[cnt++] = BYTE1(_a);
    Data_Buff[cnt++] = BYTE2(_a);
	Data_Buff[cnt++] = BYTE3(_a);
	
	Data_Buff[cnt++] = BYTE0(_b);
	Data_Buff[cnt++] = BYTE1(_b);
	Data_Buff[cnt++] = BYTE2(_b);
	Data_Buff[cnt++] = BYTE3(_b);
	
	Data_Buff[cnt++] = BYTE0(_c);
	Data_Buff[cnt++] = BYTE1(_c);
    Data_Buff[cnt++] = BYTE2(_c);
	Data_Buff[cnt++] = BYTE3(_c);
	
	Data_Buff[cnt++] = BYTE0(_d);
	Data_Buff[cnt++] = BYTE1(_d);
    Data_Buff[cnt++] = BYTE2(_d);
	Data_Buff[cnt++] = BYTE3(_d);

    Data_Buff[cnt++] = BYTE0(_e);
	Data_Buff[cnt++] = BYTE1(_e);
    Data_Buff[cnt++] = BYTE2(_e);
	Data_Buff[cnt++] = BYTE3(_e);

    Data_Buff[cnt++] = BYTE0(_f);
	Data_Buff[cnt++] = BYTE1(_f);
    Data_Buff[cnt++] = BYTE2(_f);
	Data_Buff[cnt++] = BYTE3(_f);

    Data_Buff[cnt++] = BYTE0(_g);
	Data_Buff[cnt++] = BYTE1(_g);
    Data_Buff[cnt++] = BYTE2(_g);
	Data_Buff[cnt++] = BYTE3(_g);

    Data_Buff[cnt++] = BYTE0(_h);
	Data_Buff[cnt++] = BYTE1(_h);
    Data_Buff[cnt++] = BYTE2(_h);
	Data_Buff[cnt++] = BYTE3(_h);


	Data_Buff[3]=cnt-4;
	
	for(i=0;i<cnt;i++)	/* У��λ���� */
	{
		sc+=Data_Buff[i];
		ac+=sc;
	}
	
	Data_Buff[cnt++] = sc;
	Data_Buff[cnt++] = ac;
    
    for( i = 0 ; i < cnt; i++) 
	{
        while((USART1->SR&0X40)==0);  
        USART1->DR = Data_Buff[i]; 
	}	
}

/**************************************************************************
�������ܣ�������λ�����Ʋ��κ���
��ڲ��������͵Ĳ�����32λ��32λ��32λ��32λ��
����  ֵ����
**************************************************************************/
void Data_send(int32_t _a,int32_t _b,int32_t _c,int32_t _d)
{
	uint8_t i,cnt=4;	/* ���� */
	uint8_t sc=0,ac=0;	/* У�� */
	
	Data_Buff[cnt++] = BYTE0(_a);
	Data_Buff[cnt++] = BYTE1(_a);
    Data_Buff[cnt++] = BYTE2(_a);
	Data_Buff[cnt++] = BYTE3(_a);
	
	Data_Buff[cnt++] = BYTE0(_b);
	Data_Buff[cnt++] = BYTE1(_b);
	Data_Buff[cnt++] = BYTE2(_b);
	Data_Buff[cnt++] = BYTE3(_b);
	
	Data_Buff[cnt++] = BYTE0(_c);
	Data_Buff[cnt++] = BYTE1(_c);
    Data_Buff[cnt++] = BYTE2(_c);
	Data_Buff[cnt++] = BYTE3(_c);
	
	Data_Buff[cnt++] = BYTE0(_d);
	Data_Buff[cnt++] = BYTE1(_d);
    Data_Buff[cnt++] = BYTE2(_d);
	Data_Buff[cnt++] = BYTE3(_d);


	Data_Buff[3]=cnt-4;
	
	for(i=0;i<cnt;i++)	/* У��λ���� */
	{
		sc+=Data_Buff[i];
		ac+=sc;
	}
	
	Data_Buff[cnt++] = sc;
	Data_Buff[cnt++] = ac;
    
    for( i = 0 ; i < cnt; i++) 
	{
        while((USART1->SR&0X40)==0);  
        USART1->DR = Data_Buff[i]; 
	}	
}

/**************************************************************************
�������ܣ��������ݣ��ڲ�����
��ڲ�����id������
����  ֵ����
**************************************************************************/
void Ano_SentPar(uint16_t id,int32_t data)
{
    uint8_t cnt=4,i=0,sc=0,ac=0;
    
    Data_Sent[cnt++] = BYTE0(id);
	Data_Sent[cnt++] = BYTE1(id);

	
	Data_Sent[cnt++] = BYTE0(data);
	Data_Sent[cnt++] = BYTE1(data);
	Data_Sent[cnt++] = BYTE2(data);
	Data_Sent[cnt++] = BYTE3(data);
    
    Data_Sent[3]=cnt-4;
    
    for(i=0;i<cnt;i++)		/* У��λ���� */
	{
		sc+=Data_Sent[i];
		ac+=sc;
	}
	
	Data_Sent[cnt++] = sc;
	Data_Sent[cnt++] = ac;
    
    for( i = 0 ; i < cnt; i++) 
	{
        while((USART1->SR&0X40)==0);  
        USART1->DR = Data_Sent[i]; 
	}   
}

/**************************************************************************
�������ܣ����ظ���λ����У�����ݣ����ܵ���λ������д��ʱͨ��ʹ�ã��ڲ�����
��ڲ�����id����У�飬����У��
����  ֵ����
**************************************************************************/
void Ano_SentCheck(uint8_t id,uint8_t _sc,uint8_t _ac)  /* У��֡ */
{
    uint8_t cnt=4,i=0,sc=0,ac=0;
    
    Data_Check[cnt++] = id;
	Data_Check[cnt++] = _sc;
    Data_Check[cnt++] = _ac;
    
    Data_Check[3]=cnt-4;
    
    for(i=0;i<cnt;i++)		/* У��λ���� */
	{
		sc+=Data_Check[i];
		ac+=sc;
	}
	
	Data_Check[cnt++] = sc;
	Data_Check[cnt++] = ac;
    
    for( i = 0 ; i < cnt; i++) 
	{
        while((USART1->SR&0X40)==0);  
        USART1->DR = Data_Check[i]; 
	}    
}

/**************************************************************************
�������ܣ�����Э�����ݽ�������
��ڲ��������ݰ�
id   �� ��1    2   3   4
��    ����KP   KI  KD  num
�� �� ֵ����
**************************************************************************/
void Ano_Anl(uint8_t *Data_Pack)          /* ���ݽ��� */
{
    uint8_t sc = 0,ac = 0;
    
    for(uint8_t i=0;i<Data_Pack[3] + 4;i++)
    {
        sc += Data_Pack[i];
        ac += sc;
    }
    if(sc != Data_Pack[Data_Pack[3]+4] || ac!= Data_Pack[Data_Pack[3]+5])
    {
        return;
    } 
    
    if(Data_Pack[2] == 0xE1) /* ������ȡID 0XE1 */
    {
        uint16_t id = Data_Pack[4] + ((uint16_t)Data_Pack[5]<<8);
        switch(id)
        {            
            case 1: 
                Ano_SentPar(id,rpm);        /* �ٶ� */
            break;
            
            case 2:
                Ano_SentPar(id,num*10);     /* Ȧ���Ŵ�10������ */
            break;
               
            default:
                Ano_SentPar(id,0);
            break;
        }
    }
    
    if(Data_Pack[2] == 0xE2) /* ����д��ID 0XE2�����յ���������*/
    {
        uint16_t id = Data_Pack[4] + ((uint16_t)Data_Pack[5]<<8);
        switch(id)
        {
            case 1:
                rpm = *(int *)(&Data_Pack[6]);
                break;
            
            case 2:
                num = (float)*(int *)(&Data_Pack[6]);
                break;
                
            case 3:
                task = *(int *)(&Data_Pack[6]);
                do_cnt =0;
                break;   

            case 4:
               
                break;   
        }
    }
    Ano_SentCheck(Data_Pack[2],Data_Pack[Data_Pack[3]+4],Data_Pack[Data_Pack[3]+5]); /* ����У�� */
}

/**************************************************************************
�������ܣ�����Э������У��
��ڲ������յ���һ֡����
����  ֵ����
**************************************************************************/
void Ano_GetByte(uint8_t data)
{
    static uint8_t sta=0,datalen=0,datacnt=0;      /* ֡���ȣ����յ����� */ 
    
    if(sta==0)
    {
        datalen=0;
        datacnt=0;
        
        Data_Get[0]=data;
        if(data==0xAA)       /* ֡ͷ */
            sta=1;
    }
    else if(sta==1)          /* ��ַ */
    {
        Data_Get[1]=data;
        sta=2;
    }
    else if(sta==2)          /* ID */
    {
        Data_Get[2]=data;
        sta=3;
    }
    else if(sta==3)          /* ���ݳ��� */
    {
        sta=4;
        Data_Get[3]=data;
        datalen=data;
    }
    else if(sta==4)         /* ���ݻ��� */
    {
        Data_Get[4+datacnt++]=data;
        if(datacnt >= datalen)
        {
            sta=5;
        }
    }
    else if(sta==5)         /* ��У�� */
    {
        Data_Get[4+datacnt++]=data;
        sta=6;
    }
    else if(sta==6)         /* ����У�� */
    {
        sta=0;
        Data_Get[4+datacnt++]=data;
        
        Ano_Anl(Data_Get);  /* ���ݽ��� */
    }     
}

