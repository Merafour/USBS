/******************** (C) COPYRIGHT 2015 merafour ********************
* Author             : 冷月追风@merafour.blog.163.com
* Version            : V1.0.0
* Date               : 2/26/2015
* Description        : 加密测试程序.
********************************************************************************
* merafour.blog.163.com
*
*******************************************************************************/
#ifndef _MATCH_H_
#define _MATCH_H_

#include <stdint.h>
#include <stdio.h>
#include "stm32f10x.h"

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

__inline static void passwd_id(mtext* _mtext, uint32_t uid[3])  
{
    _mtext->id[0] = (uid[0]&uid[1])|((~uid[0])&uid[2]);
    _mtext->id[1] = (uid[0]&uid[2])|(uid[1]&(~uid[2]));
    _mtext->id[2] = uid[0]^uid[1]^uid[2];
}
__inline static void passwd_sign(mtext* _mtext, uint32_t uid[3])
{
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

#endif //_MATCH_H_
