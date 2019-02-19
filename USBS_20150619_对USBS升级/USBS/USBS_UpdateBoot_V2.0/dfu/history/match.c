
#include "match.h"
#include <stdint.h>
#include <stdio.h>
#include "stm32f10x.h"

//static __INLINE void passwd_app(mtext* _mtext, uint32_t uid[3]);
//static uint16_t STMFLASH_ReadHalfWord(uint32_t faddr);
//static void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead);

#define BUF_LEN   32

extern int match_first(void);
//mtext _mtext;
static __INLINE void inline_test(IRQn_Type IRQn, mtext* _mtext, uint32_t uid[3])
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

int match_id(void)
{
	mtext* _mtext=NULL;
	mtext passwd;
	uint32_t uid[3]={0};
	char buf[BUF_LEN]={0};
	int match=0;
	// read UID
	uid[0]=*(vu32*)(0x1ffff7e8);
	uid[1]=*(vu32*)(0x1ffff7ec);
	uid[2]=*(vu32*)(0x1ffff7f0);
	// passwd
	passwd_app(&passwd, uid);
	passwd_id(&passwd, uid);
	passwd_sign(&passwd, uid);
	inline_test(NULL, &passwd, uid);
	//inline_test_2(NULL, &passwd, uid);
	// read flash
	match_Read(0x08004C00+0x18, (uint16_t*)buf, BUF_LEN/2);
	_mtext = (mtext*)buf;
	// match
	if(passwd.id[0] != _mtext->id[0]) match=1;
	if(passwd.id[1] != _mtext->id[1]) match=1;
	if(passwd.id[2] != _mtext->id[2]) match=1;
	if(passwd.sign[0] != _mtext->sign[0]) match=1;
	if(passwd.sign[1] != _mtext->sign[1]) match=1;
	if(passwd.sign[2] != _mtext->sign[2]) match=1;
	if(passwd.sign[3] != _mtext->sign[3]) match=1;

	if(match && match_first()) // error
	{
		printf("uid:%08X, %08X, %08X\r\n", uid[0], uid[1], uid[2]);
		printf("id:%08X, _id:%08X\r\n", passwd.id[0], _mtext->id[0]);
		printf("id:%08X, _id:%08X\r\n", passwd.id[1], _mtext->id[1]);
		printf("id:%08X, _id:%08X\r\n", passwd.id[2], _mtext->id[2]);
		printf("id:%08X, _id:%08X\r\n", passwd.sign[0], _mtext->sign[0]);
		printf("id:%08X, _id:%08X\r\n", passwd.sign[1], _mtext->sign[1]);
		printf("id:%08X, _id:%08X\r\n", passwd.sign[2], _mtext->sign[2]);
		printf("id:%08X, _id:%08X\r\n", passwd.sign[3], _mtext->sign[3]);
		return MATCH_ERR;
	}
	return MATCH_OK;
}
#if 0

#define UID0    (0x357584edU)
#define UID1    (0x46734568U)
#define UID2    (0xadc86789U)
#define UID     (0xa366db89U)
static __INLINE void passwd_app_02(mtext* _mtext, uint32_t uid[3])  
{
    _mtext->id[0] = (uid[0]&uid[1])|((~uid[0])&uid[2]);
    _mtext->id[1] = (uid[0]&uid[2])|(uid[1]&(~uid[2]));
    _mtext->id[2] = uid[0]^uid[1]^uid[2];
	_mtext->sign[0]  = uid[0]^(_mtext->id[2]|(~UID2));
	_mtext->sign[1]  = ((uid[0]>>16)&0XFFFF0000) | (uid[1]&0XFFFF0000); // uid1[15:8] uid0[15:8]
	_mtext->sign[2]  = ((uid[1]<<16)&0XFFFF0000) | (uid[2]&0X0000FFFF); // uid1[7:0] uid2[7:0]
	_mtext->sign[3]  = ((uid[0]<<16)&0XFFFF0000) | ((uid[2]>>16)&0X0000FFFF); // uid0[7:0] uid2[15:8]
}

//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.
static uint16_t STMFLASH_ReadHalfWord(uint32_t faddr)
{
	return *(vu16*)faddr; 
}
//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
static void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead)   	
{
	uint16_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//读取2个字节.
		ReadAddr+=2;//偏移2个字节.	
	}
}

#endif

