/******************** (C) COPYRIGHT 2015 merafour ********************
* Author             : 冷月追风@merafour.blog.163.com
* Version            : V1.0.0
* Date               : 2/26/2015
* Description        : 加密测试程序.
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


//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.
__inline static uint16_t match_ReadHalfWord(uint32_t faddr)
{
	return *(vu16*)faddr; 
}
//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
__inline static void match_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead)   	
{
	uint16_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=match_ReadHalfWord(ReadAddr);//读取2个字节.
		ReadAddr+=2;//偏移2个字节.	
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
	int Flag=0; // 通过该变量检测是否强行跳过了加密校验代码
	//printf("\r\nApp start!\r\n");
	// 测试加密
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
		goto UN_PERMISSION; // 出错处理
	}
	printf("match OK!\r\n");
	if(0==Flag) // 被跳过加密校验
	{
		printf("match2 error!\r\n");
		goto UN_PERMISSION; // 出错处理
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


