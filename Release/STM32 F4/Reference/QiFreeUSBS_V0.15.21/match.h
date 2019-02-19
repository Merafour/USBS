/******************** (C) COPYRIGHT 2015 merafour ********************
* Author             : 冷月追风@merafour.blog.163.com
* Version            : V1.0.0
* Date               : 2/26/2015
* Description        : 加密程序.
********************************************************************************
* merafour.blog.163.com
*
*******************************************************************************/
#ifndef _MATCH_H_
#define _MATCH_H_

#include <stdint.h>
#include <stdio.h>
//#include "stm32f10x.h"
#include "stm32f4xx.h" 

#define UID0    (0x35fd84edU)
#define UID1    (0x4ba34568U)
#define UID2    (0xadc24e89U)
//#define UID     (0xa366db89U)

__inline static void passwd_id(uint32_t sign[8], uint32_t uid[3])  
{
    sign[0] = (uid[0]&uid[1])|((~uid[0])&uid[2]);
    sign[1] = (uid[0]&uid[2])|(uid[1]&(~uid[2]));
    sign[2] = uid[0]^uid[1]^uid[2];
	sign[3] = uid[1]^(uid[0]|(~uid[2]));
//	sign[4] = (sign[0]&sign[1])   |UID0;    // 
//	sign[5] = (sign[0]&sign[2])   |UID1;    // 
//	sign[6] = ((~sign[0])&sign[2])|UID2;    // 
//	sign[7] = (sign[1]&(~sign[2]))|UID0;    // 
}
__inline static void passwd_sign(uint32_t sign[8], uint32_t uid[3])
{
	sign[4] = (sign[0]&sign[1])   |UID0;    // 
	sign[5] = (sign[0]&sign[2])   |UID1;    // 
	sign[6] = ((~sign[0])&sign[2])|UID2;    // 
	sign[7] = (sign[1]&(~sign[2]))|UID0;    // 
}

#endif //_MATCH_H_
