/******************** (C) COPYRIGHT 2015 merafour ********************
* Author             : 冷月追风@merafour.blog.163.com
* Version            : V1.0.0
* Date               : 2/26/2015
* Description        : RAM虚拟磁盘.
********************************************************************************
* merafour.blog.163.com
* 深圳市乐迪电子有限公司:www.radiolink.com.cn
*******************************************************************************/
#ifndef _SRAM_UPAN_H_
#define _SRAM_UPAN_H_

#include <stdint.h>
//#define SRAM_LEN_K  32
//#define SRAM_LEN_K  33
#define SRAM_LEN_K  41

extern void SRAM_Init(void);
extern int SRAM_Write(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite) ;
extern int SRAM_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead) ;
extern uint8_t* GetRAM(void);

#endif //_SRAM_UPAN_H_
