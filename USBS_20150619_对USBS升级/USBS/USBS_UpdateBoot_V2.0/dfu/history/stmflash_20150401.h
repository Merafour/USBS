#ifndef __STMFLASH_H__
#define __STMFLASH_H__
//#include "sys.h"  
#include <stdint.h>
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK miniSTM32开发板
//STM32 FLASH 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/13
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
#include "config.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//用户根据自己的需要设置
//#define STM32_FLASH_SIZE 512 	 		//所选STM32的FLASH容量大小(单位为K)
//#define STM32_FLASH_SIZE 128 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 1              //使能FLASH写入(0，不是能;1，使能)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
 
 

//uint16_t STMFLASH_ReadHalfWord(uint32_t faddr);		  //读出半字  
//void STMFLASH_WriteLenByte(uint32_t WriteAddr,uint32_t DataToWrite,uint16_t Len);	//指定地址开始写入指定长度的数据
//uint32_t STMFLASH_ReadLenByte(uint32_t ReadAddr,uint16_t Len);						//指定地址开始读取指定长度数据
extern uint16_t STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);		//从指定地址开始写入指定长度的数据
extern void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead);   		//从指定地址开始读出指定长度的数据
extern uint16_t STMFLASH_Write_zone(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);

//测试写入
//void Test_Write(uint32_t WriteAddr,uint16_t WriteData);								   
#endif

















