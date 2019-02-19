/**
  ******************************************************************************
  * @file    flash_if.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   specific media access Layer for internal flash
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "flash_if.h"
//#include "dfu_mal.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

#if defined(STM32L1XX_MD) || defined(STM32L1XX_HD)|| defined(STM32L1XX_MD_PLUS)
  uint32_t tHalfPage1[128/4];
  uint32_t tHalfPage2[128/4];
#endif /* STM32L1XX_XD */  

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : FLASH_If_Init
* Description    : Initializes the Media on the STM32
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t FLASH_If_Init(void)
{
  return MAL_OK;
}

/*******************************************************************************
* Function Name  : FLASH_If_Erase
* Description    : Erase sector
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t FLASH_If_Erase(uint32_t SectorAddress)
{
#if defined(STM32L1XX_MD) || defined(STM32L1XX_HD)|| defined(STM32L1XX_MD_PLUS)
  FLASH_ClearFlag(FLASH_FLAG_PGAERR | FLASH_FLAG_OPTVERR);
  FLASH_ErasePage(SectorAddress);
#else
  FLASH_ErasePage(SectorAddress);
#endif /* STM32L1XX_XD */
  
  return MAL_OK;
}

/*******************************************************************************
* Function Name  : FLASH_If_Write
* Description    : Write sectors
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t FLASH_If_Write(uint32_t SectorAddress, uint8_t Buffer[], uint32_t DataLength)
{
  uint32_t idx = 0;
#if defined(STM32L1XX_MD) || defined(STM32L1XX_HD)|| defined(STM32L1XX_MD_PLUS)
	// no compile
#endif /* STM32L1XX_XD */      
  
  if  (DataLength & 0x3) /* Not an aligned data */
  {
    for (idx = DataLength; idx < ((DataLength & 0xFFFC) + 4); idx++)
    {
      Buffer[idx] = 0xFF;
    }
  } 
  
#if defined(STM32L1XX_MD) || defined(STM32L1XX_HD)|| defined(STM32L1XX_MD_PLUS) 
   // no compile
#else
  
  /* Data received are Word multiple */    
  for (idx = 0; idx <  DataLength; idx = idx + 4)
  {
    FLASH_ProgramWord(SectorAddress, *(uint32_t *)(Buffer + idx));  
    SectorAddress += 4;
  } 
#endif /* STM32L1XX_XD */
 
  return MAL_OK;
}

/*******************************************************************************
* Function Name  : FLASH_If_Read
* Description    : Read sectors
* Input          : None
* Output         : None
* Return         : buffer address pointer
*******************************************************************************/
uint8_t *FLASH_If_Read (uint32_t SectorAddress, uint8_t Buffer[], uint32_t DataLength)
{
  return  (uint8_t*)(SectorAddress);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
