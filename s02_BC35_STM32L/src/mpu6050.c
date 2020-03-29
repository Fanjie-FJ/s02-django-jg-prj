/*
 * mpu6050.c
 *
 *  Created on: 2020��3��24��
 *      Author: multimicro
 */

#include "mpu6050.h"
#include "usart.h"
#include "string.h"
#include "sw180.h"
#include "key.h"
#include "BC35.h"

unsigned char temp_buf[11];
unsigned char mpu6050_data[100];

float a[3],w[3],angle[3],T;

extern unsigned char bc35_send_data[50];

void mpu6050_init(){
	mpu6050_rec_date_complete = 0;
	mpu6050_rec_date_start = 0;
}

void mpu6050_get_data(){

	if(mpu6050_rec_date_start == 0)
		return;
	mpu6050_rec_date_start = 0;
//	Uart1_SendStr("-------------------------------------------------------\r\n");

	unsigned char Temp[35];
	int i;
	while(mpu6050_rec_date_complete != 1);//�ȴ����ݽ������
	mpu6050_rec_date_complete = 0;
	memcpy(Temp,RxBuffer2,33);
	if(Temp[0]==0x55)       //���֡ͷ
	{
		for (i = 0; i < 3; ++i) {
			switch(Temp[1 + 11 * i])
			{
			case 0x51: //��ʶ������Ǽ��ٶȰ�
				a[0] = ((short)(Temp[3 + 11 * i]<<8 | Temp[2 + 11 * i]))/32768.0*16;      //X����ٶ�
				a[1] = ((short)(Temp[5 + 11 * i]<<8 | Temp[4 + 11 * i]))/32768.0*16;      //Y����ٶ�
				a[2] = ((short)(Temp[7 + 11 * i]<<8 | Temp[6 + 11 * i]))/32768.0*16;      //Z����ٶ�
				T    = ((short)(Temp[9 + 11 * i]<<8 | Temp[8 + 11 * i]))/340.0+36.25;      //�¶�
				break;
			case 0x52: //��ʶ������ǽ��ٶȰ�
				w[0] = ((short)(Temp[3 + 11 * i]<<8| Temp[2 + 11 * i]))/32768.0*2000;      //X����ٶ�
				w[1] = ((short)(Temp[5 + 11 * i]<<8| Temp[4 + 11 * i]))/32768.0*2000;      //Y����ٶ�
				w[2] = ((short)(Temp[7 + 11 * i]<<8| Temp[6 + 11 * i]))/32768.0*2000;      //Z����ٶ�
				T    = ((short)(Temp[9 + 11 * i]<<8| Temp[8 + 11 * i]))/340.0+36.25;      //�¶�
				break;
			case 0x53: //��ʶ������ǽǶȰ�
				angle[0] = ((short)(Temp[3 + 11 * i]<<8| Temp[2 + 11 * i]))/32768.0*180;   //X���ת�ǣ�x �ᣩ
				angle[1] = ((short)(Temp[5 + 11 * i]<<8| Temp[4 + 11 * i]))/32768.0*180;   //Y�ḩ���ǣ�y �ᣩ
				angle[2] = ((short)(Temp[7 + 11 * i]<<8| Temp[6 + 11 * i]))/32768.0*180;   //Z��ƫ���ǣ�z �ᣩ
				T        = ((short)(Temp[9 + 11 * i]<<8| Temp[8 + 11 * i]))/340.0+36.25;   //�¶�
				//printf("X��Ƕȣ�%.2f   Y��Ƕȣ�%.2f   Z��Ƕȣ�%.2f\r\n",angle[0],angle[1],angle[2]);
				break;
			default:  break;
			}
		}

//		sprintf(mpu6050_data,"ACC: X %-3.2f  Y %3.2f  Z %3.2f\r\n"
//				"RAD: X %-3.2f  Y %3.2f  Z %3.2f\r\n"
//				"ANG: X %-3.2f  Y %3.2f  Z %3.2f\r\n",a[0],a[1],a[2],w[0],w[1],w[2],angle[0],angle[1],angle[2]);

		sprintf(mpu6050_data,"ANG: X %3.1f  Y %3.1f  Z %3.1f\r\n",angle[0],angle[1],angle[2]);
		Uart1_SendStr(mpu6050_data);

		sprintf(bc35_send_data + 14,"%04d", (int)((angle[0] + 180.0) * 10));

		sprintf(bc35_send_data + 18,"%04d", (int)((angle[1] + 180.0) * 10));

		sprintf(bc35_send_data + 22,"%04d", (int)((angle[2] + 180.0) * 10));


		//SW180������
		if (sw180_status == SW180_VIBRATE_OFF) {
			strncat(bc35_send_data + 26,"00",2);
		} else {
			strncat(bc35_send_data + 26,"11",2);
		}
		//��������
		if (sw180_status == SW180_VIBRATE_OFF) {
			strncat(bc35_send_data + 28,"00",2);
		} else {
			strncat(bc35_send_data + 28,"11",2);
		}
		//����Աģʽ
		if (admin_status == ADMIN_STATUS_OFF) {
			strncat(bc35_send_data + 30,"00",2);
		} else {
			strncat(bc35_send_data + 30,"11",2);
		}

		Uart1_SendStr(bc35_send_data);
		Uart1_SendStr("\r\n");
		BC95_Senddata(16,bc35_send_data);
		sw180_status = SW180_VIBRATE_OFF;
	}
	Uart1_SendStr("-------------------------------------------------------\r\n");

}
