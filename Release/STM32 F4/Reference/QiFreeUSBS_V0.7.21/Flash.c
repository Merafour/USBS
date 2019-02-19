/******************** (C) COPYRIGHT 2015 merafour ********************
* Author             : 冷月追风@merafour.blog.163.com
* Version            : V1.0.0
* Date               : 3/31/2015
* Description        : Flash.
********************************************************************************
* merafour.blog.163.com
* 深圳市乐迪电子有限公司:www.radiolink.com.cn
*******************************************************************************/
#include "Flash.h"
#include "stm32f4xx.h"
#include <stdio.h>

// Flash Sector Addr
//#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) 	// 16 Kbytes  
//#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) 	// 16 Kbytes  
//#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) 	// 16 Kbytes  
//#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) 	// 16 Kbytes  
//#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) 	// 64 Kbytes  
//#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) 	// 128 Kbytes  
//#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) 	// 128 Kbytes  
//#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) 	// 128 Kbytes  
//#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) 	// 128 Kbytes  
//#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) 	// 128 Kbytes  
//#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) 	// 128 Kbytes  
//#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) 	// 128 Kbytes 
static uint16_t Flash_Sector_Map[]={
	// 16K
	FLASH_Sector_0,
	// 16K
	FLASH_Sector_1,
	// 16K
	FLASH_Sector_2,
	// 16K
	FLASH_Sector_3,
	// 64K
	FLASH_Sector_4,FLASH_Sector_4,FLASH_Sector_4,FLASH_Sector_4,
	// 128K
	FLASH_Sector_5,FLASH_Sector_5,FLASH_Sector_5,FLASH_Sector_5,
	FLASH_Sector_5,FLASH_Sector_5,FLASH_Sector_5,FLASH_Sector_5,
	// 128K
	FLASH_Sector_6,FLASH_Sector_6,FLASH_Sector_6,FLASH_Sector_6,
	FLASH_Sector_6,FLASH_Sector_6,FLASH_Sector_6,FLASH_Sector_6,
	// 128K
	FLASH_Sector_7,FLASH_Sector_7,FLASH_Sector_7,FLASH_Sector_7,
	FLASH_Sector_7,FLASH_Sector_7,FLASH_Sector_7,FLASH_Sector_7,
	// 128K
	FLASH_Sector_8,FLASH_Sector_8,FLASH_Sector_8,FLASH_Sector_8,
	FLASH_Sector_8,FLASH_Sector_8,FLASH_Sector_8,FLASH_Sector_8,
	// 128K
	FLASH_Sector_9,FLASH_Sector_9,FLASH_Sector_9,FLASH_Sector_9,
	FLASH_Sector_9,FLASH_Sector_9,FLASH_Sector_9,FLASH_Sector_9,
	// 128K
	FLASH_Sector_10,FLASH_Sector_10,FLASH_Sector_10,FLASH_Sector_10,
	FLASH_Sector_10,FLASH_Sector_10,FLASH_Sector_10,FLASH_Sector_10,
	// 128K
	FLASH_Sector_11,FLASH_Sector_11,FLASH_Sector_11,FLASH_Sector_11,
	FLASH_Sector_11,FLASH_Sector_11,FLASH_Sector_11,FLASH_Sector_11,
};
static const uint32_t Sector_Map_Size = sizeof(Flash_Sector_Map)/sizeof(Flash_Sector_Map[0]);
static FlashSectorNo(uint32_t addr)
{
//	uint32_t No=0;
//	//uint32_t size = sizeof(Flash_Sector_Map)/sizeof(Flash_Sector_Map[0]);
//	addr >>= 14; // 0x4000
//	//addr &= 0x3FF; // 0x004000 ==> 0x3FF
//	No = addr&0xFF;
//	if(No > Sector_Map_Size) No=Sector_Map_Size;
//	return Flash_Sector_Map[No]; 
	//addr >>= 14; // 0x4000
	//addr = addr&0xFF;
	addr = (addr>>14)&0xFF;
	if(addr > Sector_Map_Size) addr=Sector_Map_Size;
	return Flash_Sector_Map[addr]; 
}
int Flash_Write(uint32_t const WriteAddr,uint32_t const *pBuffer,uint32_t const NumToWrite)	
{ 
	FLASH_Status status = FLASH_COMPLETE;	
	uint32_t size = 0;
	volatile const uint32_t* const FlashAddr=(volatile uint32_t*)WriteAddr;
	if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4) return -1;	// Addr error
	FLASH_Unlock();
	FLASH_DataCacheCmd(DISABLE);
	if(WriteAddr<0X1FFF0000)
	{
		for(size=0; size<NumToWrite; size++)
		{
			if(FlashAddr[size]!=0XFFFFFFFF)// empty
			{   
				status=FLASH_EraseSector(FlashSectorNo(WriteAddr+size*4),VoltageRange_3);
				if(status!=FLASH_COMPLETE)break;
			}
		} 
	}
	if(status==FLASH_COMPLETE)
	{
		for(size=0; size<NumToWrite; size++)
		{
			if(FLASH_ProgramWord(WriteAddr+size*4,pBuffer[size])!=FLASH_COMPLETE)// write
			{ 
				break;	// error
			}
		} 
	}
	FLASH_DataCacheCmd(ENABLE);
	FLASH_Lock();
	return 0;
}
int Flash_Read(uint32_t ReadAddr,uint32_t *pBuffer,uint32_t NumToRead)   	
{
	uint32_t i;
	volatile const uint32_t* FlashAddr = NULL;
	FlashAddr = (volatile uint32_t*)ReadAddr;
	for(i=0;i<NumToRead;i++)
	{	
		pBuffer[i]=FlashAddr[i]; // read 4 Byte
	}
	return 0;
}
