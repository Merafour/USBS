/******************** (C) COPYRIGHT 2015 merafour ********************
* Author             : 冷月追风@merafour.blog.163.com
* Version            : V1.0.0
* Date               : 2/26/2015
* Description        : RAM虚拟磁盘.
********************************************************************************
* merafour.blog.163.com
* 深圳市乐迪电子有限公司:www.radiolink.com.cn
*******************************************************************************/

#include "SRAM.h"
#include "log.h"

static uint8_t SRAM_BUF[SRAM_LEN_K*1024]={0};

uint8_t* GetRAM(void)
{
	return SRAM_BUF;
}

void SRAM_Init(void)
{
	int i = 0;
	for(i=0; i<SRAM_LEN_K*1024; i++)
	{
		SRAM_BUF[i] = 0;
	}
}

int SRAM_Write(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)   
{ 
	int i=0;
	dprintf("WriteAddr: %08X\r\n", WriteAddr);
	for(i=0;i<NumByteToWrite;i++)// Write SRAM
	{
		if((WriteAddr+i)>=sizeof(SRAM_BUF)) return -1;
		SRAM_BUF[WriteAddr+i] = pBuffer[i];  	  
	}	 	 
	return 0;
}
int SRAM_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)   
{ 
	int i=0;
	dprintf("ReadAddr: %08X\r\n", ReadAddr);
	for(i=0;i<NumByteToRead;i++)// Read SRAM
	{
		if((ReadAddr+i)>=sizeof(SRAM_BUF)) return -1;
		pBuffer[i] = SRAM_BUF[ReadAddr+i];  	  
	}	  	      
	return 0;
} 





