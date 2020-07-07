/***************************************************************************//**
* \file cyip_sflash_128.h
*
* \brief
* SFLASH IP definitions
*
* \note
* Generator version: 1.6.0.136
*
********************************************************************************
* \copyright
* Copyright 2016-2020 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#ifndef _CYIP_SFLASH_128_H_
#define _CYIP_SFLASH_128_H_

#include "cyip_headers.h"

/*******************************************************************************
*                                    SFLASH
*******************************************************************************/

#define SFLASH_SECTION_SIZE                     0x00000400UL

/**
  * \brief Supervisory Flash Area (Cypress Trim & Wounding Info) (SFLASH)
  */
typedef struct {
   __IM uint32_t RESERVED[53];
  __IOM uint8_t  CSDV2_CSD1_ADC_TRIM1;          /*!< 0x000000D4 CSDV2 CSD1 ADC TRIM 1 */
  __IOM uint8_t  CSDV2_CSD1_ADC_TRIM2;          /*!< 0x000000D5 CSDV2 CSD1 ADC TRIM2 */
   __IM uint16_t RESERVED1[55];
  __IOM uint32_t SILICON_ID;                    /*!< 0x00000144 Silicon ID */
   __IM uint32_t RESERVED2[2];
  __IOM uint16_t HIB_KEY_DELAY;                 /*!< 0x00000150 Hibernate wakeup value for PWR_KEY_DELAY */
  __IOM uint16_t DPSLP_KEY_DELAY;               /*!< 0x00000152 DeepSleep wakeup value for PWR_KEY_DELAY */
   __IM uint32_t RESERVED3;
  __IOM uint32_t SWD_LISTEN;                    /*!< 0x00000158 Listen Window Length */
  __IOM uint32_t FLASH_START;                   /*!< 0x0000015C Flash Image Start Address */
  __IOM uint8_t  CSDV2_CSD0_ADC_TRIM1;          /*!< 0x00000160 CSDV2 CSD0 ADC TRIM 1 */
  __IOM uint8_t  CSDV2_CSD0_ADC_TRIM2;          /*!< 0x00000161 CSDV2 CSD0 ADC TRIM2 */
   __IM uint16_t RESERVED4;
  __IOM uint16_t SAR_TEMP_MULTIPLIER;           /*!< 0x00000164 SAR Temperature Sensor Multiplication Factor */
  __IOM uint16_t SAR_TEMP_OFFSET;               /*!< 0x00000166 SAR Temperature Sensor Offset */
   __IM uint16_t RESERVED5[43];
  __IOM uint8_t  IMO_TRIM_USBMODE_24;           /*!< 0x000001BE USB IMO TRIM 24MHz */
  __IOM uint8_t  IMO_TRIM_USBMODE_48;           /*!< 0x000001BF USB IMO TRIM 48MHz */
   __IM uint32_t RESERVED6[3];
  __IOM uint8_t  IMO_TCTRIM_LT[25];             /*!< 0x000001CC IMO TempCo Trim Register (SRSS-Lite) */
  __IOM uint8_t  IMO_TRIM_LT[25];               /*!< 0x000001E5 IMO Frequency Trim Register (SRSS-Lite) */
} SFLASH_Type;                                  /*!< Size = 510 (0x1FE) */


/* SFLASH.CSDV2_CSD1_ADC_TRIM1 */
#define SFLASH_CSDV2_CSD1_ADC_TRIM1_ADCTRIM_1P2V_Pos 0UL
#define SFLASH_CSDV2_CSD1_ADC_TRIM1_ADCTRIM_1P2V_Msk 0x1FUL
#define SFLASH_CSDV2_CSD1_ADC_TRIM1_ADCTRIM_2P4V_2_0_Pos 5UL
#define SFLASH_CSDV2_CSD1_ADC_TRIM1_ADCTRIM_2P4V_2_0_Msk 0xE0UL
/* SFLASH.CSDV2_CSD1_ADC_TRIM2 */
#define SFLASH_CSDV2_CSD1_ADC_TRIM2_ADCTRIM_3P84V_2_0_Pos 0UL
#define SFLASH_CSDV2_CSD1_ADC_TRIM2_ADCTRIM_3P84V_2_0_Msk 0x1FUL
#define SFLASH_CSDV2_CSD1_ADC_TRIM2_ADCTRIM_2P4V_5_4_Pos 5UL
#define SFLASH_CSDV2_CSD1_ADC_TRIM2_ADCTRIM_2P4V_5_4_Msk 0x60UL
/* SFLASH.SILICON_ID */
#define SFLASH_SILICON_ID_ID_Pos                0UL
#define SFLASH_SILICON_ID_ID_Msk                0xFFFFUL
/* SFLASH.HIB_KEY_DELAY */
#define SFLASH_HIB_KEY_DELAY_WAKEUP_HOLDOFF_Pos 0UL
#define SFLASH_HIB_KEY_DELAY_WAKEUP_HOLDOFF_Msk 0x3FFUL
/* SFLASH.DPSLP_KEY_DELAY */
#define SFLASH_DPSLP_KEY_DELAY_WAKEUP_HOLDOFF_Pos 0UL
#define SFLASH_DPSLP_KEY_DELAY_WAKEUP_HOLDOFF_Msk 0x3FFUL
/* SFLASH.SWD_LISTEN */
#define SFLASH_SWD_LISTEN_CYCLES_Pos            0UL
#define SFLASH_SWD_LISTEN_CYCLES_Msk            0xFFFFFFFFUL
/* SFLASH.FLASH_START */
#define SFLASH_FLASH_START_ADDRESS_Pos          0UL
#define SFLASH_FLASH_START_ADDRESS_Msk          0xFFFFFFFFUL
/* SFLASH.CSDV2_CSD0_ADC_TRIM1 */
#define SFLASH_CSDV2_CSD0_ADC_TRIM1_CSD_ADC_CAL_LSB_Pos 0UL
#define SFLASH_CSDV2_CSD0_ADC_TRIM1_CSD_ADC_CAL_LSB_Msk 0xFFUL
/* SFLASH.CSDV2_CSD0_ADC_TRIM2 */
#define SFLASH_CSDV2_CSD0_ADC_TRIM2_CSD_ADC_CAL_MSB_Pos 0UL
#define SFLASH_CSDV2_CSD0_ADC_TRIM2_CSD_ADC_CAL_MSB_Msk 0xFFUL
/* SFLASH.SAR_TEMP_MULTIPLIER */
#define SFLASH_SAR_TEMP_MULTIPLIER_TEMP_MULTIPLIER_Pos 0UL
#define SFLASH_SAR_TEMP_MULTIPLIER_TEMP_MULTIPLIER_Msk 0xFFFFUL
/* SFLASH.SAR_TEMP_OFFSET */
#define SFLASH_SAR_TEMP_OFFSET_TEMP_OFFSET_Pos  0UL
#define SFLASH_SAR_TEMP_OFFSET_TEMP_OFFSET_Msk  0xFFFFUL
/* SFLASH.IMO_TRIM_USBMODE_24 */
#define SFLASH_IMO_TRIM_USBMODE_24_TRIM_24_Pos  0UL
#define SFLASH_IMO_TRIM_USBMODE_24_TRIM_24_Msk  0xFFUL
/* SFLASH.IMO_TRIM_USBMODE_48 */
#define SFLASH_IMO_TRIM_USBMODE_48_TRIM_24_Pos  0UL
#define SFLASH_IMO_TRIM_USBMODE_48_TRIM_24_Msk  0xFFUL
/* SFLASH.IMO_TCTRIM_LT */
#define SFLASH_IMO_TCTRIM_LT_STEPSIZE_Pos       0UL
#define SFLASH_IMO_TCTRIM_LT_STEPSIZE_Msk       0x1FUL
#define SFLASH_IMO_TCTRIM_LT_TCTRIM_Pos         5UL
#define SFLASH_IMO_TCTRIM_LT_TCTRIM_Msk         0x60UL
/* SFLASH.IMO_TRIM_LT */
#define SFLASH_IMO_TRIM_LT_OFFSET_Pos           0UL
#define SFLASH_IMO_TRIM_LT_OFFSET_Msk           0xFFUL


#endif /* _CYIP_SFLASH_128_H_ */


/* [] END OF FILE */
