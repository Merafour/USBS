;******************** (C) COPYRIGHT 2012 STMicroelectronics ********************
;* File Name          : startup_stm32f10x_md.s
;* Author             : MCD Application Team
;* Version            : V3.6.1
;* Date               : 09-March-2012
;* Description        : STM32F10x Medium Density Devices vector table for MDK-ARM 
;*                      toolchain.  
;*                      This module performs:
;*                      - Set the initial SP
;*                      - Set the initial PC == Reset_Handler
;*                      - Set the vector table entries with the exceptions ISR address
;*                      - Configure the clock system
;*                      - Branches to __main in the C library (which eventually
;*                        calls main()).
;*                      After Reset the CortexM3 processor is in Thread mode,
;*                      priority is Privileged, and the Stack is set to Main.
;* <<< Use Configuration Wizard in Context Menu >>>   
;*******************************************************************************
; 
; Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
; You may not use this file except in compliance with the License.
; You may obtain a copy of the License at:
; 
;        http://www.st.com/software_license_agreement_liberty_v2
; 
; Unless required by applicable law or agreed to in writing, software 
; distributed under the License is distributed on an "AS IS" BASIS, 
; WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; See the License for the specific language governing permissions and
; limitations under the License.
; 
;*******************************************************************************

; Amount of memory (in bytes) allocated for Stack
; Tailor this value to your application needs
; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>
                PRESERVE8
                THUMB

; my define data
;                AREA    mydata, DATA, READONLY
;				DCD     __initial_sp               ; Top of Stack
; my define goto Reset
;                AREA    mycode, CODE, READONLY
;				AREA    |mycode|, CODE, READONLY
;     IMPORT  SystemInit
;                 LDR     R0, =Reset_Handler
;                 BLX     R0
				AREA    mydata, DATA, READONLY
;				INCBIN "..\..\..\..\..\..\..\..\Projects\Mass_Storage\dfu\USBS.bin"
;				INCBIN USBS_0327.bin
;				INCBIN USBS_0327_test.bin
;				INCBIN USBS_AT9.bin
;				INCBIN STM3210B-EVAL_DFU_AT9.bin
;				INCBIN QiFreeUSBS_AT9.bin
;				INCBIN QiFreeUSBS_AT10.bin
				INCBIN QiFreeUSBS.bin
				END
;                AREA    mycode, DATA, READONLY
;				DCD     __initial_sp               ; Top of Stack
;				DCD     Reset_Handler              ; Reset Handler
;************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE*****
