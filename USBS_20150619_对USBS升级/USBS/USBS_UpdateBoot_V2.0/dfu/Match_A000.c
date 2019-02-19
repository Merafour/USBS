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
#define VectTabOffset  0xA000
// 20K
//#define VectTabOffset  0x5000

typedef int (*FuncSection)(void);
typedef struct {
    uint32_t id[3];  // 加密信息
	uint32_t sign[4];   // 加密签名
    FuncSection code;   //函数指针
}mtext;
#define BUF_LEN   32

#define UID0    (0x357584edU)
#define UID1    (0x46734568U)
#define UID2    (0xadc86789U)
#define UID     (0xa366db89U)

__inline static void passwd_app(mtext* _mtext, uint32_t uid[3])  
{
#if 0
	//passwd_id(_mtext, uid);
	//passwd_sign(NULL, _mtext, uid);
#else
    _mtext->id[0] = (uid[0]&uid[1])|((~uid[0])&uid[2]);
    _mtext->id[1] = (uid[0]&uid[2])|(uid[1]&(~uid[2]));
    _mtext->id[2] = uid[0]^uid[1]^uid[2];
	_mtext->sign[0]  = uid[0]^(_mtext->id[2]|(~UID2));
	_mtext->sign[1]  = ((uid[0]>>16)&0XFFFF0000) | (uid[1]&0XFFFF0000); // uid1[15:8] uid0[15:8]
	_mtext->sign[2]  = ((uid[1]<<16)&0XFFFF0000) | (uid[2]&0X0000FFFF); // uid1[7:0] uid2[7:0]
	_mtext->sign[3]  = ((uid[0]<<16)&0XFFFF0000) | ((uid[2]>>16)&0X0000FFFF); // uid0[7:0] uid2[15:8]
#endif
}

__inline static void passwd_id(mtext* _mtext, uint32_t uid[3])  
{
    _mtext->id[0] = (uid[0]&uid[1])|((~uid[0])&uid[2]);
    _mtext->id[1] = (uid[0]&uid[2])|(uid[1]&(~uid[2]));
    _mtext->id[2] = uid[0]^uid[1]^uid[2];
}
__inline static void passwd_sign(mtext* _mtext, uint32_t uid[3])
{
//	NVIC->ISER[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F)); /* enable interrupt */
//    _mtext.id[0] = (uid[0]&uid[1])|((~uid[0])&uid[2]);
//    _mtext.id[1] = (uid[0]&uid[2])|(uid[1]&(~uid[2]));
//    _mtext.id[2] = uid[0]^uid[1]^uid[2];
//	_mtext.sign[0]  = uid[0]^(_mtext.id[2]|(~UID2));
//	_mtext.sign[1]  = ((uid[0]>>16)&0XFFFF0000) | (uid[1]&0XFFFF0000); // uid1[15:8] uid0[15:8]
//	_mtext.sign[2]  = ((uid[1]<<16)&0XFFFF0000) | (uid[2]&0X0000FFFF); // uid1[7:0] uid2[7:0]
//	_mtext.sign[3]  = ((uid[0]<<16)&0XFFFF0000) | ((uid[2]>>16)&0X0000FFFF); // uid0[7:0] uid2[15:8]
//    _mtext->id[0] = (uid[0]&uid[1])|((~uid[0])&uid[2]);
//    _mtext->id[1] = (uid[0]&uid[2])|(uid[1]&(~uid[2]));
//    _mtext->id[2] = uid[0]^uid[1]^uid[2];
	_mtext->sign[0]  = uid[0]^(_mtext->id[2]|(~UID2));
	_mtext->sign[1]  = ((uid[0]>>16)&0XFFFF0000) | (uid[1]&0XFFFF0000); // uid1[15:8] uid0[15:8]
	_mtext->sign[2]  = ((uid[1]<<16)&0XFFFF0000) | (uid[2]&0X0000FFFF); // uid1[7:0] uid2[7:0]
	_mtext->sign[3]  = ((uid[0]<<16)&0XFFFF0000) | ((uid[2]>>16)&0X0000FFFF); // uid0[7:0] uid2[15:8]
}

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


#define BOOT_VERSION  0 // bootloader版本
#define BOOT_DATE     1 // bootloader编译时间
//static void bootloader(int buf[], int hex)
typedef int (*bootloader_func)(char str[64], int item);

int match_A000(void)
{
	mtext* _mtext=NULL;
	mtext passwd;
	//uint32_t* _mtext=NULL;
	//uint32_t passwd[8]={0};
	uint32_t uid[3]={0};
	char buf[BUF_LEN]={0};
	char boot[64];
	int match=0;
	bootloader_func func=NULL;	
	//u8 len;
	uint32_t addr=0;	
	u16 times=0; 
	//uint32_t addr=0;
	//char str[64] = {0};
	int Flag=0; // 通过该变量检测是否强行跳过了加密校验代码

	printf("\r\nApp start!\r\n");
	// 测试加密
	{
		// read UID
		uid[0]=*(vu32*)(0x1ffff7e8);
		uid[1]=*(vu32*)(0x1ffff7ec);
		uid[2]=*(vu32*)(0x1ffff7f0);
		// passwd
		passwd_id(&passwd, uid);
		passwd_sign(&passwd, uid);
		Flag = 1;
#if 1
		// read flash
		match_Read(0x08004C00, (uint16_t*)buf, BUF_LEN/2);
		_mtext = (mtext*)buf;
#else
		_mtext = (mtext*)0x08004C00;
#endif
		// match
		if(passwd.id[0] != _mtext->id[0]) match=1;
		if(passwd.id[1] != _mtext->id[1]) match=1;
		if(passwd.id[2] != _mtext->id[2]) match=1;
		if(passwd.sign[0] != _mtext->sign[0]) match=1;
#if 0
		if(passwd.sign[1] != _mtext->sign[1]) match=1;
		if(passwd.sign[2] != _mtext->sign[2]) match=1;
#else
		if((passwd.sign[1]&0XFFFF0000) != (_mtext->sign[1]&0XFFFF0000)) match=1;
		if((passwd.sign[2]&0X0000FFFF) != (_mtext->sign[2]&0X0000FFFF)) match=1;
#endif
		if(passwd.sign[3] != _mtext->sign[3]) match=1;
		//addr = (uint32_t)bootloader;
		//_mtext->sign[1]  = ((_mtext->sign[1])&0XFFFF0000) | ((_mtext->sign[0]^addr)&0X0000FFFF);
		//_mtext->sign[2]  = ((_mtext->sign[0]^addr)&0XFFFF0000) | ((_mtext->sign[2])&0X0000FFFF);
		//addr = ((_mtext->sign[0]^_mtext->sign[1])&0XFFFF0000) | ((_mtext->sign[0]^_mtext->sign[2])&0X0000FFFF);
		//printf("addr:%08X\r\n", addr);
		addr = (_mtext->sign[2]&0XFFFF0000) | (_mtext->sign[1]&0X0000FFFF);
		addr = addr ^ passwd.sign[0];
		//printf("addr:%08X\r\n", addr);
		func = (bootloader_func)addr;
		func(boot, BOOT_VERSION);
		printf("func:%s\r\n", boot);
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


