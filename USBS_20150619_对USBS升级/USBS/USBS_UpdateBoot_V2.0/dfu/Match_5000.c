/******************** (C) COPYRIGHT 2015 merafour ********************
* Author             : ����׷��@merafour.blog.163.com
* Version            : V1.0.0
* Date               : 2/26/2015
* Description        : ���ܲ��Գ���.
********************************************************************************
* merafour.blog.163.com
*******************************************************************************/
#include "delay.h"
#include "stm32f10x.h"
#include "usart.h"
#include "match.h"

#define BUF_LEN   32
// 40K
//#define VectTabOffset  0xA000
// 20K
#define VectTabOffset  0x7000


//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
//����ֵ:��Ӧ����.
__inline static uint16_t match_ReadHalfWord(uint32_t faddr)
{
	return *(vu16*)faddr; 
}
//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
__inline static void match_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead)   	
{
	uint16_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=match_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
	}
}

int Match_5000(void)
{
	//mtext* _mtext=NULL;
	//mtext passwd;
	uint32_t* _mtext=NULL;
	uint32_t passwd[8]={0};
	uint32_t uid[3]={0};
	char buf[BUF_LEN]={0};
	int match=0;	
	u8 t;
	//u8 len;	
	u16 times=0; 
	//uint32_t addr=0;
	//char str[64] = {0};
	int Flag=0; // ͨ���ñ�������Ƿ�ǿ�������˼���У�����
	//printf("\r\nApp start!\r\n");
	// ���Լ���
	{
		// read UID
		uid[0]=*(vu32*)(0x1ffff7e8);
		uid[1]=*(vu32*)(0x1ffff7ec);
		uid[2]=*(vu32*)(0x1ffff7f0);
		// passwd
		//UserLicense(passwd, uid);
		Flag = 1;
#if 1
		// read flash
		match_Read(0x0800000C, (uint16_t*)buf, BUF_LEN/2);
		_mtext = (uint32_t*)buf;
#else
		_mtext = (uint32_t*)0x0800000C;
#endif
		// match
		for(t=0; t<8; t++)
		{
			if(passwd[t] != _mtext[t]) match=1;
		}
		printf("bootloader:%s", (char*)0x080000F0);
	}
	if(0 != match)
	{
		printf("match error!\r\n");
		goto UN_PERMISSION; // ������
	}
	printf("match OK!\r\n");
	if(0==Flag) // ����������У��
	{
		printf("match2 error!\r\n");
		goto UN_PERMISSION; // ������
	}
	return 0;

UN_PERMISSION:
	while(1)
	{
		times++;  
		if(times>50)
		{
			printf("\r\n Please Reset!\r\n");
			times = 0;
		}
		delay_ms(10);   
		//printf("\r\n test %d\r\n", times);
	}
}


