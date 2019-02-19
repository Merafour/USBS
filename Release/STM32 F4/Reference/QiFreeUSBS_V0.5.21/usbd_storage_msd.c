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
#ifndef USE_STM3210C_EVAL 
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#endif
//  if( SD_Init() != 0)
//  {
//    return (-1); 
//  } 

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
//#ifdef USE_STM3210C_EVAL   
//  SD_CardInfo SDCardInfo;
//  
//  SD_GetCardInfo(&SDCardInfo);  
//  
//#else
//  if(SD_GetStatus() != 0 )
//  {
//    return (-1); 
//  }   
//#endif  

  
  *block_size =  512;  
  //*block_num =  SDCardInfo.CardCapacity / 512;  
  *block_num =  (512*1024) / 512;  
  
  return (0);
  
}

/**
  * @brief  check whether the medium is ready
  * @param  lun : logical unit number
  * @retval Status
  */
int8_t  STORAGE_IsReady (uint8_t lun)
{
//  
//#ifndef USE_STM3210C_EVAL  
//  
//  static int8_t last_status = 0;
//
//  if(last_status  < 0)
//  {
//    SD_Init();
//    last_status = 0;
//  }
//  
//  if(SD_GetStatus() != 0)
//  {
//    last_status = -1;
//    return (-1); 
//  }  
//#else
//  if( SD_Init() != 0)
//  {
//    return (-1);
//  }  
//#endif
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
//  
//  if( SD_ReadMultiBlocks (buf, 
//                          blk_addr * 512, 
//                          512,
//                          blk_len) != 0)
//  {
//    return -1;
//  }
//#ifndef USE_STM3210C_EVAL 
//  SD_WaitReadOperation();
//  while (SD_GetStatus() != SD_TRANSFER_OK);
//#endif    
  return 0;
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
//  
//  if( SD_WriteMultiBlocks (buf, 
//                           blk_addr * 512, 
//                           512,
//                           blk_len) != 0)
//  {
//    return -1;
//  }
//#ifndef USE_STM3210C_EVAL  
//  SD_WaitWriteOperation();
//  while (SD_GetStatus() != SD_TRANSFER_OK);  
//#endif  
  return (0);
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

