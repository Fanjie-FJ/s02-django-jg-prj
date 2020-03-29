#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include <stm32l1xx.h>
#define u8 int8_t
#define u16 int16_t
#define u32 int32_t

//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���
#define	UART2_REC_MAX_LEN	30
char RxCounter,RxBuffer[100];     //���ջ���,���USART_REC_LEN���ֽ�.
char RxCounter1,RxBuffer1[100];     //���ջ���,���USART_REC_LEN���ֽ�.
char RxCounter2,RxBuffer2[UART2_REC_MAX_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.

void uart_init(u32 bound);
void uart2_init(u32 bound);
void uart3_init(u32 bound);
void Uart1_SendStr(char*SendBuf);
#endif


