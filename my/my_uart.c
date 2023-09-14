#include "my_uart.h"
#include  "string.h"
#include  "PID.h"

/*********************************************************
    // PA9     ------> USART1_TX 紫色
    // PA10    ------> USART1_RX 灰色
     
    PD5     ---j
***********************************************************/
UartDataMCU_RX RxData={0};
 

 unsigned char uart1_Rx[1]={0} ;
 unsigned char uart1_RxBuff[uart1_RxBuffLen]={0};

/*串口二 变量信息*/
 //给树莓派发送指令，改变运行模式
// #define SendCmdLen 		10
// #define CmdA  "#A,123456@"
// #define CmdB  "#B,123456@"
// #define CmdC  "#C,123456@"
#define SendCmdLen 		2
#define CmdA  "AA"
#define CmdB  "BB"
#define CmdC  "CC"
#define CmdD  "DD"
#define CmdE  "EE"

 unsigned char uart2_Rx[1]={0} ;
 unsigned char uart2_RxBuff[128]={0};


 unsigned char uart3_Rx[1]={0} ;
 unsigned char uart3_RxBuff[uart3_RxBuffLen]={0};

 unsigned char uart4_Rx[1]={0} ;
 unsigned char uart4_RxBuff[uart4_RxBuffLen]={0};

 unsigned char uart5_Rx[1] ={0};
 unsigned char uart5_RxBuff[uart5_RxBuffLen]={0};


 void ScanCode()//uart3
{
  while (HAL_DMA_GetState(huart3.hdmatx) != HAL_DMA_STATE_READY)
  {
    // 等待发送完成
  }
	HAL_UART_Transmit_DMA( &huart3, ScanCmd  , 9);
}

void Raspberry_ChangeMode(char cmd)//uart2 sendata
{
	while ((HAL_DMA_GetState(&hdma_usart2_tx) != HAL_DMA_STATE_READY));

	switch (cmd)
	{
		case 'C':
			HAL_UART_Transmit_DMA(&huart2, CmdC, SendCmdLen);
			break;
		case 'B':
			HAL_UART_Transmit_DMA(&huart2, CmdB, SendCmdLen);
			break;
		case 'A':
			HAL_UART_Transmit_DMA(&huart2, CmdA, SendCmdLen);
			break;
		case 'D':
			HAL_UART_Transmit_DMA(&huart2, CmdD, SendCmdLen);
			break;
		case 'E':
			HAL_UART_Transmit_DMA(&huart2, CmdE, SendCmdLen);
			break;
		default:
			// HAL_UART_Transmit_DMA( &huart2, SendCmdA, SendCmdLen);
			break;
	}	
}

void  my_USART1_GetBuffer(char Rx, char *Data_Get)//uart1 receive data
{
	static int ServoConLen  = 11;
	static int ServoNum = 0, ServoCCR=0;
	static unsigned char sta=0;      /* 帧长度，接收到长度 */ 
	
	if(sta==0 && Rx=='#'  ) /* 帧头 */
	{
		Data_Get[sta]=Rx;
		sta=1; 
		//#S,2000,1,$
		//01234567890
	}
	else if(sta >=1 &&  sta <= ServoConLen-2)           
	{
		Data_Get[sta]=Rx;
		++sta;
	}
	else if(sta==ServoConLen-1 && Rx == '$')         /* 尾部 */
	{
		Data_Get[sta]=Rx;
		++sta;
	}
	else{
		sta = 0; //校验错误 重新接收下一次信息
		printf("wrong\r\n");
	}
	if (sta == ServoConLen )
	{
		printf("right\r\n");
		sta = 0;
		if ( (Data_Get[0]== '#' &&  Data_Get[ServoConLen-1] == '$' ) )
		{
			// HAL_UART_Transmit_DMA(&huart1, uart2_RxBuff,  uart2_RxBuffLen); /*debug*/
			if (  Data_Get[1] == 'S')//巡线
			{
				sscanf(Data_Get, "#S,%d,%d,$",  &ServoCCR,  &ServoNum);	
				Servo_Pwm_Duty(ServoNum,ServoCCR);
			}
			else
			{
				Servo_Pwm_Duty(ServoNum,0);
			}
			printf("num:%d CCR:%d\r\n",ServoNum,ServoCCR);
		}
	}
}

void  my_USART2_GetBuffer(char Rx, char *Data_Get)//uart2 receive data
{
	static unsigned char number =0;
	static unsigned char sta=0;      /* 帧长度，接收到长度 */ 
	
	if(sta==0 && (Rx=='#')     ) /* 帧头 */
	{
			Data_Get[sta]=Rx;
			sta=1;
	}
	else if(sta >=1 &&  sta <= uart2_RxBuffLen-1-1)           
	{
		Data_Get[sta]=Rx;
		++sta;
	}
	else if(sta==uart2_RxBuffLen-1 && Rx == '$')         /* 尾部 */
	{
		Data_Get[sta]=Rx;
		++sta;
	}
	else{
		sta = 0; //校验错误 重新接收下一次信息
	}
	if (sta == uart2_RxBuffLen )
	{
		sta = 0;
		if ( (Data_Get[0]== '#' &&  Data_Get[uart2_RxBuffLen-1] == '$' ) )
		{
			// HAL_UART_Transmit_DMA(&huart1, uart2_RxBuff,  uart2_RxBuffLen); /*debug*/
			if (  Data_Get[1] == 'A')//巡线
			{
				sscanf(Data_Get, "#A,%f,%f,%c,$", &RxData.distance,  &RxData.angle,  &RxData.turn);	
			}

			else if( Data_Get[1] == 'B')//物块
			{
				number = Data_Get[uart2_RxBuffLen-3] - '0';
				if (number > 0 && number < 4)
				{
					RxData.B_dis_H[number] = RxData.B_dis[number];//更新历史
					sscanf(Data_Get, "#B,%f,%f,%c,$", &RxData.B_dis[number] , &RxData.B_ang[number],   &RxData.B_col[number]);//更新数据
					++ RxData.B_cnt[number];//更新累计帧数
					RxData.B_Accume[number] += RxData.B_dis_H[number]-RxData.B_dis[number]; //累计位移  历史-当前  靠近为+  远离为-

					if (RxData.B_cnt[number] == RxData.B_cntMax) //记满 B_cntMax 次，计算位移 //树莓派一帧数据连发三次
					{
						RxData.B_Vector[number] = RxData.B_Accume[number];

						RxData.B_Accume[number] = 0;//累计数清零

						RxData.B_cnt[number] = 0 ;//帧数还愿 重新计算向量

						
					}

					// if (RxData.B_cnt[number] == 0)//第一帧 记录为历史
					// {
					// 	RxData.B_dis_H[number] = RxData.B_dis[number];
					// }
					// sscanf(Data_Get, "#B,%f,%f,%c,$", &RxData.B_dis[number] , &RxData.B_ang[number],   &RxData.B_col[number]);	
					// ++ RxData.B_cnt[number];//串口接收到一次 帧数+1
					// if (RxData.B_cnt[number] == RxData.B_cntMax+RxData.B_cntMax ) //记满 B_cntMax 次，计算位移 //树莓派一帧数据连发三次
					// {
					// 	RxData.B_Vector[number] =  RxData.B_dis[number] - RxData.B_dis_H[number];
					// 	RxData.B_cnt[number] = 0 ;//帧数还愿 重新计算向量
					// }
				}
				
				
				// switch (Data_Get[uart2_RxBuffLen-3])
				// {
				// 	case '1':sscanf(Data_Get, "#B,%f,%f,%c,$", &RxData.B_dis[1] , &RxData.B_ang[1],   &RxData.B_col[1]);	
						
				// 		break;

				// 	case '2':sscanf(Data_Get, "#B,%f,%f,%c,$", &RxData.B_dis[2] , &RxData.B_ang[2],   &RxData.B_col[2]);	
						
				// 		break;

				// 	case '3':sscanf(Data_Get, "#B,%f,%f,%c,$", &RxData.B_dis[3] , &RxData.B_ang[3],   &RxData.B_col[3]);	
						
				// 		break;

				// 	default:
				// 		break;
				// }
			}

			else if (Data_Get[1] == 'C')//色环
			{
				switch (Data_Get[uart2_RxBuffLen-3])
				{
					case '1':sscanf(Data_Get, "#C,%f,%f,%c,$", &RxData.C_dis[1] , &RxData.C_ang[1],   &RxData.C_col[1]);	
						break;

					case '2':sscanf(Data_Get, "#C,%f,%f,%c,$", &RxData.C_dis[2] , &RxData.C_ang[2],   &RxData.C_col[2]);	
						break;

					case '3':sscanf(Data_Get, "#C,%f,%f,%c,$", &RxData.C_dis[3] , &RxData.C_ang[3],   &RxData.C_col[3]);	
						break;

					default:
						break;
				}
			}
			 
		}
	}
}

 




#define CODE_LENGTH 7
//处理接受到的 扫码模块数据
void  my_USART3_GetBuffer(char Rx,unsigned char *Data_Get)//uart3 scan code
{
  
/**
 * 
 * 注：在命令触发模式下，触发扫描的串口指令为：7E 00 08 01 00 02 01 AB CD
	   识读模块收到触发指令后，会先输出七个字节的回应信息并同步启动扫描
		（回应信息内容：02 00 00  01 00 33 31

 * 先发送 7E 00 08 01 00 02 01 AB CD   9位
 * 再接收 02 00 00 01 00 33 31  7位
 */
	static unsigned char sta=0,datacnt=0, rx_Flag=0;      /* 帧长度，接收到长度 */ 
	
	if (Rx == 0x02)
	{
		rx_Flag =0;
		sta = 0;
	}
	if (rx_Flag  == 0)
	{
		if (sta >=0 && sta < 7)
		{
			if (Rx == ScanFlag[sta])
			{
				++sta;
			}
		}
		if (sta == 7)
		{
			rx_Flag = 1;
			sta =0;
			
		}
	}
	else if (rx_Flag)
	{
		if ( sta >=0 && sta < CODE_LENGTH )
		{
			Data_Get[datacnt] = Rx;
			++sta;  ++datacnt;
		}
		else if ( sta == CODE_LENGTH ) //接受到的 123+321 进行处理
		{
			for (sta = 0; sta < datacnt;  ++sta)
			{
				 RxData.code[sta] = Data_Get[sta];
			}
			RxData.code[sta+1] = '\0';
			sta  = 0;		datacnt = 0;   

			RxData.Task1[0] = RxData.code[0]-'0';
			RxData.Task1[1] = RxData.code[1]-'0';
			RxData.Task1[2] = RxData.code[2]-'0';
			
			RxData.Task2[0] = RxData.code[4]-'0';
			RxData.Task2[1] = RxData.code[5]-'0';
			RxData.Task2[2] = RxData.code[6]-'0';
			
			RxData.codeFlag = codeOK;
		}
	}
}
 

void UART4_LCD_UpdataDisplay( )
{
	static char senddata[64]={0};
	sprintf(senddata, "#Z,%s,%d,%d,$",RxData.code, task, do_cnt);
	if (HAL_DMA_GetState(huart4.hdmatx) == HAL_DMA_STATE_READY)
	{
 
		printf("%s\r\n", senddata);
		printf("ccc: %s\r\n", RxData.code);
		// HAL_UART_Transmit(&huart4, senddata, strlen(senddata),20);
		HAL_UART_Transmit_DMA(&huart4, senddata, strlen(senddata));
	}
	
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)                   /*debug*/
    {
		
        HAL_UART_Receive_IT(&huart1,( u8*)uart1_Rx, 1 );
		my_USART1_GetBuffer(uart1_Rx[0],uart1_RxBuff);
        // USART1 ->DR = uart1_Rx[0];  //回传数据  匿名上位机需要
        // Ano_GetByte(uart1_Rx[0]);
    }
    if (huart->Instance == USART2)                   /*carmera for oneside distance*/
    {
		HAL_UART_Receive_IT(&huart2, (u8 *)uart2_Rx, sizeof(u8)*1);
		my_USART2_GetBuffer(uart2_Rx[0],uart2_RxBuff);
	}
    if (huart->Instance == USART3)                  /*for scan QRcode*/
    {
		HAL_UART_Receive_IT(&huart3,( u8*)uart3_Rx, sizeof(u8)*1 ); 
		my_USART3_GetBuffer(uart3_Rx[0], uart3_RxBuff);                
    }
	if (huart->Instance == UART4)                  /*for display tft uart*/
    {
		HAL_UART_Receive_IT(&huart4,( u8*)uart4_Rx, sizeof(u8)*1 ); 
		my_USART3_GetBuffer(uart4_Rx[0], uart4_RxBuff);                
    }
}






// void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) //DMA
//{
//	unsigned short i=0;
//	unsigned char  dataCount=0;
//	if (huart == &huart2) 
//	{
//		HAL_UART_DMAStop(&huart2);
//		dataCount= sizeof(uart2_RxBuff) - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);
//		
//     	// 0~64   20 40 60
//		HAL_UART_Transmit_DMA(&huart1, uart2_RxBuff, dataCount); /*debug*/

//		for (  i = 0; i < 44; i++)
//		{
//			 my_USART2_GetBuffer(uart2_RxBuff[i], RxData.uart2DataBuffer );
//		}
//		
//		

//		HAL_UARTEx_ReceiveToIdle_DMA(&huart2, uart2_RxBuff, 64);/*restart next receive DMA action*/
//		__HAL_DMA_DISABLE_IT(&hdma_usart2_rx,DMA_IT_HT);//close DMA half finished  interrupt
//	}
//}




void RxDataClear(char mode_data)
{
	if ( mode_data == 'A')
	{
		RxData.distance = 0;
		RxData.angle = 0;
	}
	else if (mode_data == 'B' || mode_data == 'C'){
		for (int i = 1; i < 4; i++)
		{
			RxData.B_ang[i] = 179;
			RxData.B_dis[i] = 999;
			
			RxData.C_ang[i] = 179;
			RxData.C_dis[i] = 999;
		}
	}
	else if (mode_data == 'D')
	{
		for (int i = 1; i < 4; i++)
		{
			RxData.B_ang[i] = 179;
			RxData.B_dis[i] = 999;
			
			RxData.C_ang[i] = 179;
			RxData.C_ang[i] = 999;
		}
		RxData.distance = 0;
		RxData.angle = 0;
	}
}


void Correspond_Init()
{
  RxData.code[0] = 'C';
  RxData.code[1] = 'h';
  RxData.code[2] = 'i';
  RxData.code[3] = 'n';
  RxData.code[4] = 'a';
  RxData.code[5] = '!';
  RxData.code[6] = '_';
  
  RxData.B_cntMax = 26;

  HAL_Delay(2);
  HAL_UART_Receive_IT(&huart1,( uint8_t*)uart1_Rx, sizeof(u8)*1 );
  // HAL_UART_Receive_DMA(&huart1,uart1_DataBuff, uart1_RxBuffLen);
  
  // __HAL_UART_DISABLE_IT(&huart2, UART_IT_RXNE);//close rx   single    interrupt
  // __HAL_UART_DISABLE_IT(&huart2, UART_IT_TC); //close  tx   finished  interrupt
  // HAL_UARTEx_ReceiveToIdle_DMA(&huart2, uart2_RxBuff, 64);/*raspberry*/
  // __HAL_DMA_DISABLE_IT(&hdma_usart2_rx,DMA_IT_HT);//close DMA half finished  interrupt
  HAL_Delay(2);
  HAL_UART_Receive_IT(&huart2,(uint8_t *)uart2_Rx, 1);

  HAL_Delay(2);
  HAL_UART_Receive_IT(&huart3,(uint8_t *)uart3_Rx, 1);  

  HAL_Delay(2);
  HAL_UART_Receive_IT(&huart4,(uint8_t *)uart4_Rx, 1);  

  HAL_Delay(2);
  HAL_UART_Receive_IT(&huart5,(uint8_t *)uart5_Rx, 1);  
}


