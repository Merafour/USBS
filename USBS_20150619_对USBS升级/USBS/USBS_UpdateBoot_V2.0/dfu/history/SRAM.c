/******************** (C) COPYRIGHT 2015 merafour ********************
* Author             : 冷月追风@merafour.blog.163.com
* Version            : V1.0.0
* Date               : 2/26/2015
* Description        : RAM虚拟磁盘.
********************************************************************************
* merafour.blog.163.com
*
*******************************************************************************/

/**
 * 由于原版代码使用的 Device为 STM32F103VB,其 RAM仅 20K，而这里使用的 RAM已经超出 20K,
 * 故根据开发板所使用的芯片 STM32F103RCT6将 Device改为 STM32F103RC.
 */
#include "SRAM.h"
//#include "malloc.h"
#include "log.h"

//写SPI FLASH  
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)  		   
#define SRAM_LEN_K  38
//static uint8_t SRAM_BUF[30*1024]={0};
static uint8_t SRAM_BUF[SRAM_LEN_K*1024]={0};
//static uint16_t SRAM_BUF[30*512]={0};
//static u8* SRAM_BUF=0;
//__attribute__ ((section (".FAT32"))) u8 SRAM_BUF[30*1024] __attribute__((used)) = {0};
void SRAM_Init(void)
{
	int i = 0;
	//SRAM_BUF=mymalloc(40960);	//申请内存
	//lprintf("SRAM_Init...\r\n");
	for(i=0; i<SRAM_LEN_K*1024; i++)
	{
		SRAM_BUF[i] = 0;
	}
}

void SRAM_Write(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)   
{ 
	int i=0;
	dprintf("WriteAddr: %08X\r\n", WriteAddr);
	for(i=0;i<NumByteToWrite;i++)//写入SRAM
	{
		if((WriteAddr+i)>=sizeof(SRAM_BUF)) return;
		SRAM_BUF[WriteAddr+i] = pBuffer[i];  	  
	}	 	 
}

//读取SPI FLASH  
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(24bit)
//NumByteToRead:要读取的字节数(最大65535)
void SRAM_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)   
{ 
	int i=0;
	dprintf("ReadAddr: %08X\r\n", ReadAddr);
	for(i=0;i<NumByteToRead;i++)//写入SRAM
	{
		if((ReadAddr+i)>=sizeof(SRAM_BUF)) return;
		pBuffer[i] = SRAM_BUF[ReadAddr+i];  	  
	}	  	      
} 





