/******************** (C) COPYRIGHT 2015 merafour ********************
* Author             : ¿‰‘¬◊∑∑Á@merafour.blog.163.com
* Version            : V1.0.0
* Date               : 2/26/2015
* Description        : RAM–Èƒ‚¥≈≈Ã.
********************************************************************************
* merafour.blog.163.com
*
*******************************************************************************/
#ifndef _SRAM_UPAN_H_
#define _SRAM_UPAN_H_

#include "sys.h"
extern void SRAM_Init(void);
extern void SRAM_Write(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite) ;
extern void SRAM_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead) ;

#endif //_SRAM_UPAN_H_
