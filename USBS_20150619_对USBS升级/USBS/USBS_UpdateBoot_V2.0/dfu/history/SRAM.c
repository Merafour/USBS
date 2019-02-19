/******************** (C) COPYRIGHT 2015 merafour ********************
* Author             : ����׷��@merafour.blog.163.com
* Version            : V1.0.0
* Date               : 2/26/2015
* Description        : RAM�������.
********************************************************************************
* merafour.blog.163.com
*
*******************************************************************************/

/**
 * ����ԭ�����ʹ�õ� DeviceΪ STM32F103VB,�� RAM�� 20K��������ʹ�õ� RAM�Ѿ����� 20K,
 * �ʸ��ݿ�������ʹ�õ�оƬ STM32F103RCT6�� Device��Ϊ STM32F103RC.
 */
#include "SRAM.h"
//#include "malloc.h"
#include "log.h"

//дSPI FLASH  
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)  		   
#define SRAM_LEN_K  38
//static uint8_t SRAM_BUF[30*1024]={0};
static uint8_t SRAM_BUF[SRAM_LEN_K*1024]={0};
//static uint16_t SRAM_BUF[30*512]={0};
//static u8* SRAM_BUF=0;
//__attribute__ ((section (".FAT32"))) u8 SRAM_BUF[30*1024] __attribute__((used)) = {0};
void SRAM_Init(void)
{
	int i = 0;
	//SRAM_BUF=mymalloc(40960);	//�����ڴ�
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
	for(i=0;i<NumByteToWrite;i++)//д��SRAM
	{
		if((WriteAddr+i)>=sizeof(SRAM_BUF)) return;
		SRAM_BUF[WriteAddr+i] = pBuffer[i];  	  
	}	 	 
}

//��ȡSPI FLASH  
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
void SRAM_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)   
{ 
	int i=0;
	dprintf("ReadAddr: %08X\r\n", ReadAddr);
	for(i=0;i<NumByteToRead;i++)//д��SRAM
	{
		if((ReadAddr+i)>=sizeof(SRAM_BUF)) return;
		pBuffer[i] = SRAM_BUF[ReadAddr+i];  	  
	}	  	      
} 





