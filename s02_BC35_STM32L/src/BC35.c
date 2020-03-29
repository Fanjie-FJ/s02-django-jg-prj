///AT+NSOCL=0
#include "bc35.h"
#include "main.h"
#include "string.h"
#include "usart.h"
char *strx,*extstrx;
extern char  RxBuffer[100],RxCounter;
BC95 BC95_Status;

unsigned char bc35_send_data[50];

void Clear_Buffer(void)//��ջ���
{
	u8 i;
	Uart1_SendStr(RxBuffer);
	RxCounter=0;
	memset(RxBuffer,'\0',sizeof(RxBuffer));
}
void BC95_Init(void)
{
	memset(RxBuffer,'\0',sizeof(RxBuffer));
	Clear_Buffer();
	printf("AT\r\n");
	Delay(300);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
	Clear_Buffer();
	while(strx==NULL)
	{
		Clear_Buffer();
		printf("AT\r\n");
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
	}
	printf("AT+CMEE=1\r\n"); //�������ֵ
	Delay(300);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
	Clear_Buffer();
	printf("AT+NBAND?\r\n");//��ȡƵ�κ�
	Delay(300);
	strx=strstr((const char*)RxBuffer,(const char*)"+NBAND:5");//����5  ����  �ƶ��Ƿ���8
	Clear_Buffer();
	printf("AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
	Delay(300);
	strx=strstr((const char*)RxBuffer,(const char*)"460");//��460
	Clear_Buffer();
	while(strx==NULL)
	{
		Clear_Buffer();
		printf("AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"460");//����OK,˵�����Ǵ��ڵ�
	}
	printf("AT+CGATT=1\r\n");//�������磬PDP
	Delay(300);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//��OK
	Clear_Buffer();
	while(strx==NULL)
	{
		Clear_Buffer();
		printf("AT+CGATT=1\r\n");//
		Delay(100);
		strx=strstr((const char*)RxBuffer,(const char*)"OK");//
	}
	Uart1_SendStr("��������...�ɹ�\r\n");
	printf("AT+CGATT?\r\n");//��ѯ����״̬
	Delay(300);
	strx=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//��1 ��������ɹ� ��ȡ��IP��ַ��
	Clear_Buffer();
	while(strx==NULL)
	{
		Clear_Buffer();
		printf("AT+CGATT?\r\n");//��ȡ����״̬
		Delay(3000);
		Uart1_SendStr("wait for connect net...\n");

		strx=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//����1,����ע���ɹ�
	}
	Uart1_SendStr("��ѯ��������״̬...�ɹ�\r\n");
	printf("AT+CSQ\r\n");//�鿴��ȡCSQֵ
	Delay(300);
	strx=strstr((const char*)RxBuffer,(const char*)"+CSQ");//����CSQ
	Uart1_SendStr("\r\n");
	Uart1_SendStr(strx);
	Uart1_SendStr("\r\n");
	if(strx)
	{
		BC95_Status.CSQ=(strx[5]-0x30)*10+(strx[6]-0x30);//��ȡCSQ
		if(BC95_Status.CSQ==99)//˵��ɨ��ʧ��
		{
			while(1)
			{
				Uart1_SendStr("�ź�����ʧ�ܣ���鿴ԭ��!\r\n");
				Delay(300);
				BC95_Init();
			}
		}
	}
	while(strx==NULL)
	{
		Clear_Buffer();
		printf("AT+CSQ\r\n");//�鿴��ȡCSQֵ
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+CSQ");//
		if(strx)
		{
			BC95_Status.CSQ=(strx[5]-0x30)*10+(strx[6]-0x30);//��ȡCSQ
			if(BC95_Status.CSQ==99)//˵��ɨ��ʧ��
			{
				while(1)
				{
					Uart1_SendStr("�ź�����ʧ�ܣ���鿴ԭ��!\r\n");
					Delay(300);
				}
			}
		}
	}
	Clear_Buffer();
	printf("AT+CEREG?\r\n");
	Delay(300);
	strx=strstr((const char*)RxBuffer,(const char*)"+CEREG:0,1");//����ע��״̬
	extstrx=strstr((const char*)RxBuffer,(const char*)"+CEREG:1,1");//����ע��״̬
	Clear_Buffer();
	while(strx==NULL&&extstrx==NULL)
	{
		Clear_Buffer();
		printf("AT+CEREG?\r\n");//�ж���Ӫ��
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+CEREG:0,1");//����ע��״̬
		extstrx=strstr((const char*)RxBuffer,(const char*)"+CEREG:1,1");//����ע��״̬
	}
	printf("AT+CEREG=1\r\n");
	Delay(300);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
	Clear_Buffer();
	while(strx==NULL&&extstrx==NULL)
	{
		Clear_Buffer();
		printf("AT+CEREG=1\r\n");//�ж���Ӫ��
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
	}
	Uart1_SendStr("��ʼ��BC95�ɹ�\r\n");
	/*	 printf("AT+COPS?\r\n");//�ж���Ӫ��
Delay(300);
strx=strstr((const char*)RxBuffer,(const char*)"46011");//���ص�����Ӫ��
Clear_Buffer();	
while(strx==NULL)
{
Clear_Buffer();	
printf("AT+COPS?\r\n");//�ж���Ӫ��
Delay(300);
strx=strstr((const char*)RxBuffer,(const char*)"46011");//���ص�����Ӫ��
}
	 *///ż������������ ����ע���������ģ�����COPS 2,2,""�����Դ˴������ε�
}

void BC95_PDPACT(void)//�������Ϊ���ӷ�������׼��
{
	printf("AT+CGDCONT=1,\042IP\042,\042HUAWEI.COM\042\r\n");//����APN
	Delay(300);
	printf("AT+CGATT=1\r\n");//�����
	Delay(300);
	printf("AT+CGATT?\r\n");//�����
	Delay(300);
	strx=strstr((const char*)RxBuffer,(const char*)" +CGATT:1");//ע����������Ϣ
	Clear_Buffer();
	while(strx==NULL)
	{
		Clear_Buffer();
		printf("AT+CGATT?\r\n");//�����
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//���ص�����Ӫ��
	}
	printf("AT+CSCON?\r\n");//�ж�����״̬������1���ǳɹ�
	Delay(300);
	strx=strstr((const char*)RxBuffer,(const char*)"+CSCON:0,1");//ע����������Ϣ
	extstrx=strstr((const char*)RxBuffer,(const char*)"+CSCON:0,0");//ע����������Ϣ
	Clear_Buffer();
	while(strx==NULL&&extstrx==NULL)
	{
		Clear_Buffer();
		printf("AT+CSCON?\r\n");//
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+CSCON:0,1");//
		extstrx=strstr((const char*)RxBuffer,(const char*)"+CSCON:0,0");//
	}
	Uart1_SendStr("�����...�ɹ�\r\n");

}

void BC95_ConUDP(void)
{
	uint8_t i;
	printf("AT+NSOCL=1\r\n");//�ر�socekt����
	Delay(300);
	Clear_Buffer();
	printf("AT+NSOCR=DGRAM,17,3568,1\r\n");//����һ��Socket
	Delay(300);
	BC95_Status.Socketnum=RxBuffer[2];//��ȡ��ǰ��socket����
	Uart1_SendStr("����Socket����...�ɹ�\r\n");
	Uart1_SendStr("Socket Port is:");
	Uart1_SendStr(BC95_Status.Socketnum);
	Uart1_SendStr("\r\n");

	//tencent cloud 175.24.105.191 AF1869BF3930
	//��������ݸ�ʽ���ͣ�
	//IP:	175.24.105.191 PROT: 12345    BC95����Ϸ��͸�ʽ��PROT��λҪ�ߵ�һ��λ��
	//HEX:	AF	18 69  BF 		 3039
	Uart1_SendStr("���Է�������...\r\n");
	printf("AT+NSOST=%c,120.24.184.124,8010,%c,%s\r\n",BC95_Status.Socketnum,'8',"AF1869BF39301234");//����0socketIP�Ͷ˿ں������Ӧ���ݳ����Լ�����,
	Delay(300);

	strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
	while(strx==NULL)
	{
		strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
	}
	Clear_Buffer();
	for(i=0;i<100;i++)
		RxBuffer[i]=0;
	Delay(500);
	Uart1_SendStr("���Է�������...�ɹ�\r\n");
}

void BC95_Senddata(uint8_t len,uint8_t *data)
{

	printf("AT+NSOST=%c,120.24.184.124,8010,%d,%s\r\n",BC95_Status.Socketnum,len,data);//����0socketIP�Ͷ˿ں������Ӧ���ݳ����Լ�����,
//	printf("AT+NSOST=0,120.24.184.124,8010,%c,%s\r\n",'8',"AF1869BF39301234");//����0 socketIP�Ͷ˿ں������Ӧ���ݳ����Լ�����,727394ACB8221234
	Delay(300);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
	while(strx==NULL)
	{
		strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
	}
	Clear_Buffer();

}
void BC95_RECData(void)
{
	char i;
	static char nexti;
	strx=strstr((const char*)RxBuffer,(const char*)"+NSONMI:");//����+NSONMI:���������յ�UDP���������ص�����
	if(strx)
	{
		Clear_Buffer();
		BC95_Status.Socketnum=strx[8];//���
		BC95_Status.reclen=strx[10];//����
		printf("AT+NSORF=%c,%c\r\n",BC95_Status.Socketnum,BC95_Status.reclen);//�����Լ����
		Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)",");//��ȡ����һ������
		strx=strstr((const char*)(strx+1),(const char*)",");//��ȡ���ڶ�������
		strx=strstr((const char*)(strx+1),(const char*)",");//��ȡ������������
		for(i=0;;i++)
		{
			if(strx[i+1]==',')
				break;
			BC95_Status.recdatalen[i]=strx[i+1];//��ȡ���ݳ���
		}
		strx=strstr((const char*)(strx+1),(const char*)",");//��ȡ�����ĸ�����
		for(i=0;;i++)
		{
			if(strx[i+1]==',')
				break;
			BC95_Status.recdata[i]=strx[i+1];//��ȡ��������
		}
	}
}
