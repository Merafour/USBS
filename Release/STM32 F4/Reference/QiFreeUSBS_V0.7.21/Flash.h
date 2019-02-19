/******************** (C) COPYRIGHT 2015 merafour ********************
* Author             : 冷月追风@merafour.blog.163.com
* Version            : V1.0.0
* Date               : 3/31/2015
* Description        : Flash.
********************************************************************************
* merafour.blog.163.com
* 深圳市乐迪电子有限公司:www.radiolink.com.cn
*******************************************************************************/
#ifndef __FLASH_H__
#define __FLASH_H__	 

#include <stdint.h>

// Flash Base Addr
#define STM32_FLASH_BASE 0x08000000 	
int Flash_Write(uint32_t const WriteAddr,uint32_t const *pBuffer,uint32_t const NumToWrite);
int Flash_Read(uint32_t ReadAddr,uint32_t *pBuffer,uint32_t NumToRead);
		 				    
#endif // __FLASH_H__
