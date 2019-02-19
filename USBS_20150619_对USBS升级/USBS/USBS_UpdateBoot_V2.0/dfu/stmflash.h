/******************** (C) COPYRIGHT 2015 merafour ********************
* Author             : ����׷��@merafour.blog.163.com
* Version            : V1.0.0
* Date               : 4/1/2015
* Description        : Flash ��д����.
********************************************************************************
* merafour.blog.163.com
*
*******************************************************************************/
#ifndef __STMFLASH_H__
#define __STMFLASH_H__
//#include "sys.h"  
#include <stdint.h>
#include "config.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//�û������Լ�����Ҫ����
//#define STM32_FLASH_SIZE 512 	 		//��ѡSTM32��FLASH������С(��λΪK)
//#define STM32_FLASH_SIZE 128 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 1              //ʹ��FLASHд��(0��������;1��ʹ��)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ
 
 

//uint16_t STMFLASH_ReadHalfWord(uint32_t faddr);		  //��������  
//void STMFLASH_WriteLenByte(uint32_t WriteAddr,uint32_t DataToWrite,uint16_t Len);	//ָ����ַ��ʼд��ָ�����ȵ�����
//uint32_t STMFLASH_ReadLenByte(uint32_t ReadAddr,uint16_t Len);						//ָ����ַ��ʼ��ȡָ����������
extern uint16_t STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);		//��ָ����ַ��ʼд��ָ�����ȵ�����
extern void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����
extern uint16_t STMFLASH_Write_zone(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);
extern void FlashInit(uint32_t KB);

//����д��
//void Test_Write(uint32_t WriteAddr,uint16_t WriteData);								   
#endif

















