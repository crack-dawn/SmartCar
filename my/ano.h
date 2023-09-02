#ifndef __ANO_H
#define __ANO_H

#include "main.h"
#include "control.h"
#include "usart.h"



/* ���ݲ�ֺ궨�壬�ڷ��ʹ���1�ֽڵ���������ʱ������int16��float�ȣ���Ҫ�����ݲ�ֳɵ����ֽڽ��з��� */

#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp) + 0) ) 
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )

void Data_send2(int32_t _a,int32_t _b,int32_t _c,int32_t _d,int32_t _e,int32_t _f,int32_t _g,int32_t _h);
void Data_send(int32_t _a,int32_t _b,int32_t _c,int32_t _d);
void Ano_SentPar(uint16_t id,int32_t data);
void Ano_Anl(uint8_t *Data_Get);
void Ano_GetByte(uint8_t data);
void Ano_SentCheck(uint8_t id,uint8_t _sc,uint8_t _ac);





#endif
