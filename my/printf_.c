#include "stdio.h"
#include "main.h"

#pragma import(__use_no_semihosting_swi) //取消semihost模式
#pragma import(__use_no_semihosting) //取消semihost模式
struct __FILE  { 
    int handle; 
}; 

FILE __stdout;
FILE __stdin;

//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while( (USART1->SR&0X40)==0 );      //循环发送,直到发送完毕   
    USART1->DR = (uint8_t) ch;      
	return ch;
}


//该函数可以防止报_sys_exit is referenced错误
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//fputc,fgetc,ferror是为了防止报sys_open is referenced错误
int fgetc(FILE *f) {
  return 0;
}


int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}
//为了防止报_ttywrch is referenced错误
void _ttywrch(int ch)
{
    ;
}

// #include "stdio.h"
// #pragma import(__use_no_semihosting)             
// //标准库需要的支持函数                 
// struct __FILE {
// 	int handle; 
// }; 
// FILE __stdout;       
// //定义_sys_exit()以避免使用半主机模式    
// _sys_exit(int x){ 
// 	x = x; 
// } 
// //重定义fputc函数 
// int fputc(int ch, FILE *f){      
// 	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
//     USART1->DR = (u8) ch;      
// 	return ch;
// }
