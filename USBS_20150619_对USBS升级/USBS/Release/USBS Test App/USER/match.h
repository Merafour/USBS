/******************** (C) COPYRIGHT 2015 merafour ********************
* Author             : ����׷��@merafour.blog.163.com
* Version            : V1.0.0
* Date               : 2/26/2015
* Description        : ���ܲ��Գ���.
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
    uint32_t id[3];  // ������Ϣ
	uint32_t sign[4];   // ����ǩ��
    FuncSection code;   //����ָ��
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

#endif //_MATCH_H_
