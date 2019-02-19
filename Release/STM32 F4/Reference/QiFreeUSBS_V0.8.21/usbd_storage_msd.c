/******************** (C) COPYRIGHT 2015 merafour ********************
* Author             : 冷月追风@merafour.blog.163.com
* Version            : V1.0.0
* Date               : 6/12/2015
* Description        : storage.
********************************************************************************
* merafour.blog.163.com
* 深圳市乐迪电子有限公司:www.radiolink.com.cn
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "usbd_msc_mem.h"
#include "usb_conf.h"
#include "SRAM.h"
#include "Flash.h"
#include "log.h"

#include <string.h>

//故乡的原风景简谱前半段
static const char Sojiro[]="Sojiro 1=F 4/4 ||5612-|2-3216|3--5612|2---|321553|3-5612|2---|32163-|3-5612|2---|32166-|6--0|";
//const char Description[]="\
//* Author   : merafour.blog.163.com\r\n\
////* Version  : V1.0.0\r\n\
//* Date     : 2/26/2015\r\n";

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */


/** @defgroup STORAGE 
  * @brief media storage application module
  * @{
  */ 

/** @defgroup STORAGE_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup STORAGE_Private_Defines
  * @{
  */ 

#define STORAGE_LUN_NBR                  1 

#define FLASH_SAVE_ADDR  0X08008000 	//32K
#define RAM_FAT32_Offset_Default 0x00008000 // default 32K
static uint32_t RAM_FAT32_Offset = RAM_FAT32_Offset_Default; // default 32K

/**
  * @}
  */ 


/** @defgroup STORAGE_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup STORAGE_Private_Variables
  * @{
  */ 
/* USB Mass storage Standard Inquiry Data */
//const int8_t  STORAGE_Inquirydata[] = {//36
//  
//  /* LUN 0 */
//  0x00,		
//  0x80,		
//  0x02,		
//  0x02,
//  (USBD_STD_INQUIRY_LENGTH - 5),
//  0x00,
//  0x00,	
//  0x00,
//  'S', 'T', 'M', ' ', ' ', ' ', ' ', ' ', /* Manufacturer : 8 bytes */
//  'm', 'i', 'c', 'r', 'o', 'S', 'D', ' ', /* Product      : 16 Bytes */
//  'F', 'l', 'a', 's', 'h', ' ', ' ', ' ',
//  '1', '.', '0' ,'0',                     /* Version      : 4 Bytes */
//}; 
const int8_t  STORAGE_Inquirydata[] = {//36
  
  /* LUN 0 */
  0x00,		
  0x80,		
  0x02,		
  0x02,
  (USBD_STD_INQUIRY_LENGTH - 5),
  0x00,
  0x00,	
  0x00,
  // QiFree USBS bootloader 1.0
  'Q', 'i', 'F', 'r', 'e', 'e', ' ', ' ', /* Manufacturer : 8 bytes */
  'U', 'S', 'B', 'S', ' ', 'b', 'o', 'o', /* Product      : 16 Bytes */
  't', 'l', 'o', 'a', 'd', 'e', 'r', ' ',
  '1', '.', '0' ,'0',                     /* Version      : 4 Bytes */
};

/**
  * @}
  */ 


/** @defgroup STORAGE_Private_FunctionPrototypes
  * @{
  */ 
int8_t STORAGE_Init (uint8_t lun);

int8_t STORAGE_GetCapacity (uint8_t lun, 
                           uint32_t *block_num, 
                           uint32_t *block_size);

int8_t  STORAGE_IsReady (uint8_t lun);

int8_t  STORAGE_IsWriteProtected (uint8_t lun);

int8_t STORAGE_Read (uint8_t lun, 
                        uint8_t *buf, 
                        uint32_t blk_addr,
                        uint16_t blk_len);

int8_t STORAGE_Write (uint8_t lun, 
                        uint8_t *buf, 
                        uint32_t blk_addr,
                        uint16_t blk_len);

int8_t STORAGE_GetMaxLun (void);


USBD_STORAGE_cb_TypeDef USBD_MICRO_SDIO_fops =
{
  STORAGE_Init,
  STORAGE_GetCapacity,
  STORAGE_IsReady,
  STORAGE_IsWriteProtected,
  STORAGE_Read,
  STORAGE_Write,
  STORAGE_GetMaxLun,
  (int8_t *)STORAGE_Inquirydata,
};

USBD_STORAGE_cb_TypeDef  *USBD_STORAGE_fops = &USBD_MICRO_SDIO_fops;
//#ifndef USE_STM3210C_EVAL   
//extern SD_CardInfo SDCardInfo;
//#endif
__IO uint32_t count = 0;
/**
  * @}
  */ 


/** @defgroup STORAGE_Private_Functions
  * @{
  */ 


/**
  * @brief  Initialize the storage medium
  * @param  lun : logical unit number
  * @retval Status
  */

int8_t STORAGE_Init (uint8_t lun)
{
	SRAM_Init(); 
	return (0);
}

/**
  * @brief  return medium capacity and block size
  * @param  lun : logical unit number
  * @param  block_num :  number of physical block
  * @param  block_size : size of a physical block
  * @retval Status
  */
int8_t STORAGE_GetCapacity (uint8_t lun, uint32_t *block_num, uint32_t *block_size)
{
	uint32_t KB=256;
	*block_size =  512;   
	KB = (*(__IO uint32_t*)(0x1FFF7A22))&0xFFFF;
	*block_num =  (KB*1024) / 512;  
	return (0);
}

/**
  * @brief  check whether the medium is ready
  * @param  lun : logical unit number
  * @retval Status
  */
int8_t  STORAGE_IsReady (uint8_t lun)
{
	return (0);
}

/**
  * @brief  check whether the medium is write-protected
  * @param  lun : logical unit number
  * @retval Status
  */
int8_t  STORAGE_IsWriteProtected (uint8_t lun)
{
	return  0;
}

/**
  * @brief  Read data from the medium
  * @param  lun : logical unit number
  * @param  buf : Pointer to the buffer to save data
  * @param  blk_addr :  address of 1st block to be read
  * @param  blk_len : nmber of blocks to be read
  * @retval Status
  */
int8_t STORAGE_Read (uint8_t lun, 
                 uint8_t *buf, 
                 uint32_t blk_addr,                       
                 uint16_t blk_len)
{
	int8_t ret = 0; 
	uint32_t Memory_Offset = blk_addr*512;
	//printf("@%s [%d]addr: %03dK | %04d\r\n", __func__, lun, (blk_addr*512)/1024, blk_len*512);
	if(RAM_FAT32_Offset>Memory_Offset) // FAT32 ==> SRAM

	{
		ret=SRAM_Read(buf, blk_addr*512, blk_len*512) ;
	}
	else
	{
		Memory_Offset -= (uint32_t)RAM_FAT32_Offset;
		ret=Flash_Read(FLASH_SAVE_ADDR+Memory_Offset,(u32*)buf, (blk_len*512)/4);
	}
	return ret;
}

static void check_Sojiro(const uint32_t Memory_Offset, const char *Writebuff, const uint16_t Transfer_Length)
{
	uint16_t Sojiro_len = strlen(Sojiro);
	//char buf[1024]={0}; // 1K
	//uint32_t offsst = 0;
	//dprintf("Sojiro: %8d | %8d\r\n", Sojiro_len, Transfer_Length);
	if(Sojiro_len<=Transfer_Length) // check length
	{
		//dprintf("check: %s\r\n", Writebuff);
		// match
		//if(0==strncmp(Sojiro, (char*)Writebuff, Sojiro_len)) // 匹配成功
		if(0==strncmp(Sojiro, Writebuff, Sojiro_len)) // 匹配成功
		{
			//dprintf("match OK!\r\n");
			//dprintf("Memory_Offset: %08X\r\n", Memory_Offset);
			// RAM_FAT32_Offset move to bin
			/**
			 *  RAM                      RAM
			 *  |-------|0      |--------|0
			 *  |-FAT --|FAT32  |-FAT----|FAT32
			 *  |  ...  |move   |--------|Offsst
			 *  |-empty-|       |  Flash |
			 *  |-------|Offsst |  ...   |
			 *  |	    |       |--------|
			 */
			RAM_FAT32_Offset = Memory_Offset+1024; // 1K
			printf("RAM_FAT32_Offset: %08X\r\n", RAM_FAT32_Offset);
		}
	}
}
/**
  * @brief  Write data to the medium
  * @param  lun : logical unit number
  * @param  buf : Pointer to the buffer to write from
  * @param  blk_addr :  address of 1st block to be written
  * @param  blk_len : nmber of blocks to be read
  * @retval Status
  */
int8_t STORAGE_Write (uint8_t lun, 
                  uint8_t *buf, 
                  uint32_t blk_addr,
                  uint16_t blk_len)
{
	int8_t ret = 0; 
	uint32_t Memory_Offset = blk_addr*512;
	printf("@%s [%d]addr: %03dK | %04d\r\n", __func__, lun, (blk_addr*512)/1024, blk_len*512);
	check_Sojiro(Memory_Offset, (char*)buf, blk_len*512);
	if(RAM_FAT32_Offset>Memory_Offset) // FAT32 ==> SRAM
	{
		ret=SRAM_Write(buf, blk_addr*512, blk_len*512);
	}
	else
	{
		Memory_Offset -= (uint32_t)RAM_FAT32_Offset;
		ret=Flash_Write(FLASH_SAVE_ADDR+Memory_Offset,(u32*)buf, (blk_len*512)/4);
	}
	return ret;
}

/**
  * @brief  Return number of supported logical unit
  * @param  None
  * @retval number of logical unit
  */

int8_t STORAGE_GetMaxLun (void)
{
  return (STORAGE_LUN_NBR - 1);
}
/**
  * @}
  */ 


/**
  * @}
  */ 


/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

