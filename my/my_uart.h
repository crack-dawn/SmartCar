#ifndef _MY_UART_H_
#define _MY_UART_H_

#include "main.h"

#include "stdio.h"
#include "usart.h"

#include "ano.h"


typedef struct uart_data__rx
{
    /*扫码需要*/  //  扫码信息 123+321 之类的
    char  code[32];
    unsigned char codeFlag;
    char  mode;
    unsigned char Task1[3]; //第一圈任务编号顺序
    unsigned char Task2[3]; //第二圈任务编号顺序

    /*巡线需要*///   ##+123.4,456.7,c$$
    float distance;  
    float angle;
    char  turn;   

    /*物块识别需要*/  //直接用序号 取用数据 1红 2绿 3蓝
    char uart2DataBuffer[64];
    float B_dis[4];  
    float B_ang[4];
    char  B_col[4];    
   
    /*检测目标物块运动状态*/  //主要思路 定时器一定 350 ms 判定
    int   B_cnt[4];
    int   B_cntMax;
    int     B_Accume[4];//本次累计位移
    int     B_Vector[4];//历史累计位移
    float   B_dis_H[4];  
    float   B_ang_H[4]; 


    /*色环识别需要*/
    float C_dis[4];
    float C_ang[4];
    char  C_col[4];  
    
}UartDataMCU_RX;

 
#define turnDo          ('%')  //巡线转弯标志
#define turnNotDo	    ('2')  //巡线不转弯
 
#define codeOK          (100)  //扫码发回正常

extern UartDataMCU_RX RxData;
 

void Raspberry_ChangeMode(char cmd);// A就巡线 B就物块 C就色环 树莓派切换延迟1.5s~3s左右
#define SendCmdA Raspberry_ChangeMode('A')  //A 巡线
#define SendCmdB Raspberry_ChangeMode('B')  //B 物块
#define SendCmdC Raspberry_ChangeMode('C')  //C 色环
#define SendCmdD Raspberry_ChangeMode('D')  //D
#define SendCmdE Raspberry_ChangeMode('E')  //E





// 给扫码模块发送的扫码指令 和 扫码模块的回传标志数据
const static unsigned char ScanCmd[10] ={0x7E, 0x00, 0x08, 0x01, 0x00, 0x02, 0x01, 0xAB, 0xCD};
const static unsigned char ScanFlag[7] ={0x02, 0x00, 0x00, 0x01, 0x00, 0x33, 0x31};
void ScanCode();


//清除色环 色块  巡线 历史数据 
void RxDataClear(char mode_data); //A 巡线； B物块|| C巡线；一起清除 || D全部清除
#define ClearRxData do{ RxDataClear('D');  }while (0) //清除全部数据， 恢复默认值179 999
 
 



#define uart1_RxBuffLen  64
extern unsigned char uart1_Rx[1] ;
extern unsigned char uart1_RxBuff[uart1_RxBuffLen];

#define uart2_RxBuffLen 20
extern unsigned char uart2_Rx[1] ;
extern  unsigned char uart2_RxBuff[128];

#define uart3_RxBuffLen 64
extern unsigned char uart3_Rx[1] ;
extern unsigned char uart3_RxBuff[uart3_RxBuffLen];


#define uart4_RxBuffLen 64
extern unsigned char uart4_Rx[1] ;
extern unsigned char uart4_RxBuff[uart4_RxBuffLen];
void UART4_LCD_UpdataDisplay( );



#define uart5_RxBuffLen 64
extern unsigned char uart5_Rx[1] ;
extern unsigned char uart4_RxBuff[uart5_RxBuffLen];
 


#endif // _MY_UART_H_

 


/* hal 库串口收发*/
/*https://blog.csdn.net/as480133937/article/details/99073783?ops_request_misc=&request_id=&biz_id=102&utm_term=stm32hal%E5%BA%93%E4%B8%B2%E5%8F%A3%E9%80%9A%E4%BF%A1%E5%AE%9E%E4%BE%8B&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduweb~default-0-99073783.142^v91^insertT0,239^v12^insert_chatgpt&spm=1018.2226.3001.4187*/

 

// HAL_UART_Transmit(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size, uint32_t Timeout);
// HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout)

// HAL_UART_Transmit_DMA(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size)
// HAL_UART_Receive_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)


