/*
 * sw180.c
 *
 *  Created on: 2020��3��25��
 *      Author: multimicro
 */
#include"sw180.h"
#include "usart.h"
#include "buzzer.h"

void sw180_init(){

	sw180_status = SW180_VIBRATE_OFF;
	sw180_filter = 0;
	EXTI_Config();
}

void sw180_scan(){

}


static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* ����NVICΪ���ȼ���1 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	/* �����ж�Դ������1 */
	NVIC_InitStructure.NVIC_IRQChannel =  EXTI3_IRQn;
	/* ������ռ���ȼ� */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	/* ���������ȼ� */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	/* ʹ���ж�ͨ�� */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


}

/**
 * @brief  ���� IOΪEXTI�жϿڣ��������ж����ȼ�
 * @param  ��
 * @retval ��
 */
void EXTI_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	/*��������GPIO�ڵ�ʱ��*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ע��Ҫ��SYSCFGʱ��

	/* ���� NVIC �ж�*/
	NVIC_Configuration();

	/*--------------------------��������-----------------------------*/
	/* ѡ�񰴼��õ���GPIO */
	GPIO_InitStructure.GPIO_Pin =  SW180_15P_PIN;
	/* ����Ϊ�������� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN ;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	//GPIO_Initstructure.GPIO_Speed = GPIO_Speed_2MHz;

	GPIO_Init( SW180_15P_PORT, &GPIO_InitStructure);


	/* -------------ѡ��EXTI���ź�Դ -------------*/
	SYSCFG_EXTILineConfig( EXTI_PortSourceGPIOB,  GPIO_PinSource3);
	EXTI_InitStructure.EXTI_Line =  EXTI_Line3;

	/* EXTIΪ�ж�ģʽ */
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* �������ж� */
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//EXTI_Trigger_Rising;//EXTI_Trigger_Rising_Falling
	/* ʹ���ж� */
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line3);
		sw180_filter ++;
		if (sw180_filter > 10) {
			sw180_filter = 0;
			sw180_status = SW180_VIBRATE_ON;
			BUZZER_ON();
			Uart1_SendStr("WARRING: SW180���񶯣�\r\n");
			BUZZER_OFF();
		}
	}
}
