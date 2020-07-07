/***************************************************************************//**
* \file cy_device.h
* \version 1.0
*
* This file specifies the structure for core and peripheral block HW base
* addresses, versions, and parameters.
*
********************************************************************************
* \copyright
* Copyright 2018-2020 Cypress Semiconductor Corporation
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


#ifndef CY_DEVICE_H_
#define CY_DEVICE_H_

#include <stdint.h>
#include <stddef.h>


/*******************************************************************************
*               Register Access Helper Macros
*******************************************************************************/

#define CY_SYSCLK_HF_CLK_MAX_FREQ           (CY_HF_CLK_MAX_FREQ)

#define SRSS_PWR_CONTROL                    (((SRSSLT_Type *) SRSSLT)->PWR_CONTROL)
#define SRSSLT_PWR_KEY_DELAY                (((SRSSLT_Type *) SRSSLT)->PWR_KEY_DELAY)

#define SRSSLT_WDT_DISABLE_KEY              (((SRSSLT_Type *) SRSSLT)->WDT_DISABLE_KEY)
#define SRSSLT_WDT_COUNTER                  (((SRSSLT_Type *) SRSSLT)->WDT_COUNTER)
#define SRSSLT_WDT_MATCH                    (((SRSSLT_Type *) SRSSLT)->WDT_MATCH)

#define SRSSLT_CLK_SELECT                   (((SRSSLT_Type *) SRSSLT)->CLK_SELECT)
#define SRSSLT_CLK_ILO_CONFIG               (((SRSSLT_Type *) SRSSLT)->CLK_ILO_CONFIG)
#define SRSSLT_CLK_IMO_CONFIG               (((SRSSLT_Type *) SRSSLT)->CLK_IMO_CONFIG)
#define SRSSLT_CLK_DFT_SELECT               (((SRSSLT_Type *) SRSSLT)->CLK_DFT_SELECT)
#define SRSSLT_CLK_IMO_SELECT               (((SRSSLT_Type *) SRSSLT)->CLK_IMO_SELECT)
#define SRSSLT_CLK_IMO_TRIM1                (((SRSSLT_Type *) SRSSLT)->CLK_IMO_TRIM1)
#define SRSSLT_CLK_IMO_TRIM2                (((SRSSLT_Type *) SRSSLT)->CLK_IMO_TRIM2)
#define SRSSLT_CLK_IMO_TRIM3                (((SRSSLT_Type *) SRSSLT)->CLK_IMO_TRIM3)

#define EXCO_PLL_CONFIG                     (((EXCO_Type *) EXCO)->PLL_CONFIG)
#define EXCO_PLL_STATUS                     (((EXCO_Type *) EXCO)->PLL_STATUS)
#define EXCO_EXCO_PGM_CLK                   (((EXCO_Type *) EXCO)->EXCO_PGM_CLK)

#define SRSSLT_SRSS_INTR                      (((SRSSLT_Type *) SRSSLT)->SRSS_INTR)
#define SRSSLT_SRSS_INTR_SET                  (((SRSSLT_Type *) SRSSLT)->SRSS_INTR_SET)
#define SRSSLT_SRSS_INTR_MASK                 (((SRSSLT_Type *) SRSSLT)->SRSS_INTR_MASK)

#define SRSS_RES_CAUSE                      (((SRSSLT_Type *) SRSSLT)->RES_CAUSE)

#define SRSSLT_TST_DDFT_CTRL                   (*((__IOM uint32_t *)(SRSSLT_BASE + 0x18UL)))
#define SRSSLT_TST_DDFT_CTRL_DFT_SEL0_Pos      0UL
#define SRSSLT_TST_DDFT_CTRL_DFT_SEL0_Msk      0xFUL
#define SRSSLT_TST_DDFT_CTRL_DFT_SEL1_Pos      8UL
#define SRSSLT_TST_DDFT_CTRL_DFT_SEL1_Msk      0xF00UL

#define SRSSLT_TST_TRIM_CNTR1                  (*((__IOM uint32_t *)(SRSSLT_BASE + 0x1CUL)))
#define SRSSLT_TST_TRIM_CNTR1_COUNTER_DONE_Pos 31UL
#define SRSSLT_TST_TRIM_CNTR1_COUNTER_DONE_Msk 0x80000000UL

#define SRSSLT_TST_TRIM_CNTR2                  (*((__IOM uint32_t *)(SRSSLT_BASE + 0x20UL)))


/*******************************************************************************
*                FLASHC
*******************************************************************************/

#define FLASHC_FLASH_CTL                    (FLASHC->FLASH_CTL)


/*******************************************************************************
*                SFLASH
*******************************************************************************/

#define SFLASH_SILICON_ID                   (((SFLASH_Type *) SFLASH)->SILICON_ID)
#define SFLASH_DPSLP_KEY_DELAY              (((SFLASH_Type *) SFLASH)->DPSLP_KEY_DELAY)

#define SFLASH_IMO_TRIM_LT(freq)            ((uint32_t)(((SFLASH_Type *) SFLASH)->IMO_TRIM_LT[(freq)]))
#define SFLASH_IMO_TCTRIM_LT(freq)          ((uint32_t)(((SFLASH_Type *) SFLASH)->IMO_TCTRIM_LT[(freq)]))

/* CapSense-related trim registers */
#define SFLASH_CSD0_ADC_VREF_TRIM1          (((SFLASH_Type *) SFLASH)->CSDV2_CSD0_ADC_TRIM1)
#define SFLASH_CSD0_ADC_VREF_TRIM2          (((SFLASH_Type *) SFLASH)->CSDV2_CSD0_ADC_TRIM2)


/*******************************************************************************
*                CPUSS
*******************************************************************************/

#define CPUSS_FLASH_CTL                     (((CPUSS_Type *) CPUSS)->FLASH_CTL)

/* ARM core registers */
#define SYSTICK_CTRL                        (((SysTick_Type *)SysTick)->CTRL)
#define SYSTICK_LOAD                        (((SysTick_Type *)SysTick)->LOAD)
#define SYSTICK_VAL                         (((SysTick_Type *)SysTick)->VAL)
#define SCB_SCR                             (((SCB_Type *)SCB)->SCR)


/*******************************************************************************
*                LPCOMP
*******************************************************************************/

#define LPCOMP_CONFIG(base)                 (((LPCOMP_Type *)(base))->CONFIG)
#define LPCOMP_INTR(base)                   (((LPCOMP_Type *)(base))->INTR)
#define LPCOMP_INTR_SET(base)               (((LPCOMP_Type *)(base))->INTR_SET)
#define LPCOMP_INTR_MASK(base)              (((LPCOMP_Type *)(base))->INTR_MASK)
#define LPCOMP_INTR_MASKED(base)            (((LPCOMP_Type *)(base))->INTR_MASKED)


/*******************************************************************************
*                TCPWM
*******************************************************************************/

#define TCPWM_CTRL(base)                (((TCPWM_Type *)(base))->CTRL)
#define TCPWM_CMD(base)                 (((TCPWM_Type *)(base))->CMD)

#define TCPWM_CNT_CTRL(base, cntNum)         (((TCPWM_Type *)(base))->CNT[cntNum].CTRL)
#define TCPWM_CNT_CC(base, cntNum)           (((TCPWM_Type *)(base))->CNT[cntNum].CC)
#define TCPWM_CNT_CC_BUFF(base, cntNum)      (((TCPWM_Type *)(base))->CNT[cntNum].CC_BUFF)
#define TCPWM_CNT_COUNTER(base, cntNum)      (((TCPWM_Type *)(base))->CNT[cntNum].COUNTER)
#define TCPWM_CNT_PERIOD(base, cntNum)       (((TCPWM_Type *)(base))->CNT[cntNum].PERIOD)
#define TCPWM_CNT_PERIOD_BUFF(base, cntNum)  (((TCPWM_Type *)(base))->CNT[cntNum].PERIOD_BUFF)
#define TCPWM_CNT_STATUS(base, cntNum)       (((TCPWM_Type *)(base))->CNT[cntNum].STATUS)
#define TCPWM_CNT_INTR(base, cntNum)         (((TCPWM_Type *)(base))->CNT[cntNum].INTR)
#define TCPWM_CNT_INTR_SET(base, cntNum)     (((TCPWM_Type *)(base))->CNT[cntNum].INTR_SET)
#define TCPWM_CNT_INTR_MASK(base, cntNum)    (((TCPWM_Type *)(base))->CNT[cntNum].INTR_MASK)
#define TCPWM_CNT_INTR_MASKED(base, cntNum)  (((TCPWM_Type *)(base))->CNT[cntNum].INTR_MASKED)
#define TCPWM_CNT_TR_CTRL0(base, cntNum)     (((TCPWM_Type *)(base))->CNT[cntNum].TR_CTRL0)
#define TCPWM_CNT_TR_CTRL1(base, cntNum)     (((TCPWM_Type *)(base))->CNT[cntNum].TR_CTRL1)
#define TCPWM_CNT_TR_CTRL2(base, cntNum)     (((TCPWM_Type *)(base))->CNT[cntNum].TR_CTRL2)


/*******************************************************************************
*                SAR
*******************************************************************************/

#define SAR_SAMPLE_CTRL(base)               (((SAR_Type *)(base))->SAMPLE_CTRL)
#define SAR_SAMPLE_TIME01(base)             (((SAR_Type *)(base))->SAMPLE_TIME01)
#define SAR_SAMPLE_TIME23(base)             (((SAR_Type *)(base))->SAMPLE_TIME23)

#define SAR_RANGE_THRES(base)               (((SAR_Type *)(base))->RANGE_THRES)
#define SAR_RANGE_COND(base)                (((SAR_Type *)(base))->RANGE_COND)
#define SAR_RANGE_INTR(base)                (((SAR_Type *)(base))->RANGE_INTR)
#define SAR_RANGE_INTR_SET(base)            (((SAR_Type *)(base))->RANGE_INTR_SET)

#define SAR_RANGE_INTR_MASK(base)           (((SAR_Type *)(base))->RANGE_INTR_MASK)
#define SAR_RANGE_INTR_MASKED(base)         (((SAR_Type *)(base))->RANGE_INTR_MASKED)

#define SAR_CHAN_EN(base)                   (((SAR_Type *)(base))->CHAN_EN)
#define SAR_CHAN_CONFIG(base, chan)         (((SAR_Type *)(base))->CHAN_CONFIG[(chan)])
#define SAR_CHAN_RESULT(base, chan )        (((SAR_Type *)(base))->CHAN_RESULT[(chan)])

#define SAR_INTR(base)                      (((SAR_Type *)(base))->INTR)
#define SAR_INTR_MASK(base)                 (((SAR_Type *)(base))->INTR_MASK)
#define SAR_INTR_MASKED(base)               (((SAR_Type *)(base))->INTR_MASKED)
#define SAR_INTR_SET(base)                  (((SAR_Type *)(base))->INTR_SET)
#define SAR_INTR_CAUSE(base)                (((SAR_Type *)(base))->INTR_CAUSE)

#define SAR_MUX_SWITCH_CLEAR0(base)         (((SAR_Type *)(base))->MUX_SWITCH_CLEAR0)
#define SAR_MUX_SWITCH0(base)               (((SAR_Type *)(base))->MUX_SWITCH0)

#define SAR_CTRL(base)                      (((SAR_Type *)(base))->CTRL)
#define SAR_STATUS(base)                    (((SAR_Type *)(base))->STATUS)
#define SAR_START_CTRL(base)                (((SAR_Type *)(base))->START_CTRL)

#define SAR_SATURATE_INTR(base)             (((SAR_Type *)(base))->SATURATE_INTR)
#define SAR_SATURATE_INTR_MASK(base)        (((SAR_Type *)(base))->SATURATE_INTR_MASK)
#define SAR_SATURATE_INTR_MASKED(base)      (((SAR_Type *)(base))->SATURATE_INTR_MASKED)
#define SAR_SATURATE_INTR_SET(base)         (((SAR_Type *)(base))->SATURATE_INTR_SET)


/*******************************************************************************
*                SMARTIO
*******************************************************************************/
#define SMARTIO_PRT_Type                        PRGIO_PRT_Type
#define SMARTIO_PRT_CTL(base)                   (((SMARTIO_PRT_Type *)(base))->CTL)
#define SMARTIO_PRT_SYNC_CTL(base)              (((SMARTIO_PRT_Type *)(base))->SYNC_CTL)
#define SMARTIO_PRT_LUT_SEL(base, idx)          (((SMARTIO_PRT_Type *)(base))->LUT_SEL[idx])
#define SMARTIO_PRT_LUT_CTL(base, idx)          (((SMARTIO_PRT_Type *)(base))->LUT_CTL[idx])
#define SMARTIO_PRT_DU_SEL(base)                (((SMARTIO_PRT_Type *)(base))->DU_SEL)
#define SMARTIO_PRT_DU_CTL(base)                (((SMARTIO_PRT_Type *)(base))->DU_CTL)
#define SMARTIO_PRT_DATA(base)                  (((SMARTIO_PRT_Type *)(base))->DATA)
#define SMARTIO_PRT_CTL_BYPASS_Pos              PRGIO_PRT_CTL_BYPASS_Pos
#define SMARTIO_PRT_CTL_BYPASS_Msk              PRGIO_PRT_CTL_BYPASS_Msk
#define SMARTIO_PRT_CTL_CLOCK_SRC_Pos           PRGIO_PRT_CTL_CLOCK_SRC_Pos
#define SMARTIO_PRT_CTL_CLOCK_SRC_Msk           PRGIO_PRT_CTL_CLOCK_SRC_Msk
#define SMARTIO_PRT_CTL_HLD_OVR_Pos             PRGIO_PRT_CTL_HLD_OVR_Pos
#define SMARTIO_PRT_CTL_HLD_OVR_Msk             PRGIO_PRT_CTL_HLD_OVR_Msk
#define SMARTIO_PRT_CTL_PIPELINE_EN_Pos         PRGIO_PRT_CTL_PIPELINE_EN_Pos
#define SMARTIO_PRT_CTL_PIPELINE_EN_Msk         PRGIO_PRT_CTL_PIPELINE_EN_Msk
#define SMARTIO_PRT_CTL_ENABLED_Pos             PRGIO_PRT_CTL_ENABLED_Pos
#define SMARTIO_PRT_CTL_ENABLED_Msk             PRGIO_PRT_CTL_ENABLED_Msk
#define SMARTIO_PRT_SYNC_CTL_IO_SYNC_EN_Pos     PRGIO_PRT_SYNC_CTL_IO_SYNC_EN_Pos
#define SMARTIO_PRT_SYNC_CTL_IO_SYNC_EN_Msk     PRGIO_PRT_SYNC_CTL_IO_SYNC_EN_Msk
#define SMARTIO_PRT_SYNC_CTL_CHIP_SYNC_EN_Pos   PRGIO_PRT_SYNC_CTL_CHIP_SYNC_EN_Pos
#define SMARTIO_PRT_SYNC_CTL_CHIP_SYNC_EN_Msk   PRGIO_PRT_SYNC_CTL_CHIP_SYNC_EN_Msk
#define SMARTIO_PRT_LUT_SEL_LUT_TR0_SEL_Pos     PRGIO_PRT_LUT_SEL_LUT_TR0_SEL_Pos
#define SMARTIO_PRT_LUT_SEL_LUT_TR0_SEL_Msk     PRGIO_PRT_LUT_SEL_LUT_TR0_SEL_Msk
#define SMARTIO_PRT_LUT_SEL_LUT_TR1_SEL_Pos     PRGIO_PRT_LUT_SEL_LUT_TR1_SEL_Pos
#define SMARTIO_PRT_LUT_SEL_LUT_TR1_SEL_Msk     PRGIO_PRT_LUT_SEL_LUT_TR1_SEL_Msk
#define SMARTIO_PRT_LUT_SEL_LUT_TR2_SEL_Pos     PRGIO_PRT_LUT_SEL_LUT_TR2_SEL_Pos
#define SMARTIO_PRT_LUT_SEL_LUT_TR2_SEL_Msk     PRGIO_PRT_LUT_SEL_LUT_TR2_SEL_Msk
#define SMARTIO_PRT_LUT_CTL_LUT_Pos             PRGIO_PRT_LUT_CTL_LUT_Pos
#define SMARTIO_PRT_LUT_CTL_LUT_Msk             PRGIO_PRT_LUT_CTL_LUT_Msk
#define SMARTIO_PRT_LUT_CTL_LUT_OPC_Pos         PRGIO_PRT_LUT_CTL_LUT_OPC_Pos
#define SMARTIO_PRT_LUT_CTL_LUT_OPC_Msk         PRGIO_PRT_LUT_CTL_LUT_OPC_Msk
#define SMARTIO_PRT_DU_SEL_DU_TR0_SEL_Pos       PRGIO_PRT_DU_SEL_DU_TR0_SEL_Pos
#define SMARTIO_PRT_DU_SEL_DU_TR0_SEL_Msk       PRGIO_PRT_DU_SEL_DU_TR0_SEL_Msk
#define SMARTIO_PRT_DU_SEL_DU_TR1_SEL_Pos       PRGIO_PRT_DU_SEL_DU_TR1_SEL_Pos
#define SMARTIO_PRT_DU_SEL_DU_TR1_SEL_Msk       PRGIO_PRT_DU_SEL_DU_TR1_SEL_Msk
#define SMARTIO_PRT_DU_SEL_DU_TR2_SEL_Pos       PRGIO_PRT_DU_SEL_DU_TR2_SEL_Pos
#define SMARTIO_PRT_DU_SEL_DU_TR2_SEL_Msk       PRGIO_PRT_DU_SEL_DU_TR2_SEL_Msk
#define SMARTIO_PRT_DU_SEL_DU_DATA0_SEL_Pos     PRGIO_PRT_DU_SEL_DU_DATA0_SEL_Pos
#define SMARTIO_PRT_DU_SEL_DU_DATA0_SEL_Msk     PRGIO_PRT_DU_SEL_DU_DATA0_SEL_Msk
#define SMARTIO_PRT_DU_SEL_DU_DATA1_SEL_Pos     PRGIO_PRT_DU_SEL_DU_DATA1_SEL_Pos
#define SMARTIO_PRT_DU_SEL_DU_DATA1_SEL_Msk     PRGIO_PRT_DU_SEL_DU_DATA1_SEL_Msk
#define SMARTIO_PRT_DU_CTL_DU_SIZE_Pos          PRGIO_PRT_DU_CTL_DU_SIZE_Pos
#define SMARTIO_PRT_DU_CTL_DU_SIZE_Msk          PRGIO_PRT_DU_CTL_DU_SIZE_Msk
#define SMARTIO_PRT_DU_CTL_DU_OPC_Pos           PRGIO_PRT_DU_CTL_DU_OPC_Pos
#define SMARTIO_PRT_DU_CTL_DU_OPC_Msk           PRGIO_PRT_DU_CTL_DU_OPC_Msk
#define SMARTIO_PRT_DATA_DATA_Pos               PRGIO_PRT_DATA_DATA_Pos
#define SMARTIO_PRT_DATA_DATA_Msk               PRGIO_PRT_DATA_DATA_Msk


/*******************************************************************************
*                DMAC
*******************************************************************************/

#define DMAC_CTL(base)                      (((DMAC_Type*)(base))->CTL)


/*******************************************************************************
*                PERI
*******************************************************************************/

#define PERI_TR_CTL                         (PERI->TR_CTL)
#define PERI_TR_GR_TR_CTL(group, trCtl)     (PERI->TR_GROUP[group].TR_OUT_CTL[trCtl])

#define PERI_DIV_CMD                        (((PERI_Type *) PERI)->DIV_CMD)
#define PERI_PCLK_CTL                       (((PERI_Type *) PERI)->PCLK_CTL)

#define PERI_DIV_8_CTL                      (((PERI_Type *) PERI)->DIV_8_CTL)
#define PERI_DIV_16_CTL                     (((PERI_Type *) PERI)->DIV_16_CTL)
#define PERI_DIV_16_5_CTL                   (((PERI_Type *) PERI)->DIV_16_5_CTL)
#define PERI_DIV_24_5_CTL                   (((PERI_Type *) PERI)->DIV_24_5_CTL)


/*******************************************************************************
*                IOSS
*******************************************************************************/

#define GPIO_INTR_CAUSE                    (GPIO->INTR_CAUSE)

#define GPIO_PRT_DR(base)                  (((GPIO_PRT_Type*)(base))->DR)
#define GPIO_PRT_OUT_CLR(base)             (((GPIO_PRT_Type*)(base))->DR_CLR)
#define GPIO_PRT_OUT_SET(base)             (((GPIO_PRT_Type*)(base))->DR_SET)
#define GPIO_PRT_OUT_INV(base)             (((GPIO_PRT_Type*)(base))->DR_INV)
#define GPIO_PRT_IN(base)                  (((GPIO_PRT_Type*)(base))->PS)
#define GPIO_PRT_PC(base)                  (((GPIO_PRT_Type*)(base))->PC)
#define GPIO_PRT_PC2(base)                 (((GPIO_PRT_Type*)(base))->PC2)
#define GPIO_PRT_INTR_CFG(base)            (((GPIO_PRT_Type*)(base))->INTR_CFG)
#define GPIO_PRT_INTR(base)                (((GPIO_PRT_Type*)(base))->INTR)
#define GPIO_PRT_SIO(base)                 (((GPIO_PRT_Type*)(base))->SIO)

#define HSIOM_PRT_PORT_SEL(base)           (((HSIOM_PRT_Type *)(base))->PORT_SEL)

#define HSIOM_AMUX_SPLIT_CTL(switchCtrl)   (HSIOM->AMUX_SPLIT_CTL[switchCtrl])


/*******************************************************************************
*                LCD
*******************************************************************************/

#define LCD_OCTET_NUM                       (8U) /* LCD_NUMPORTS - number of octets supporting up to 4 COMs */
#define LCD_OCTET_NUM_8                     (8U) /* LCD_NUMPORTS8 - number of octets supporting up to 8 COMs */
#define LCD_OCTET_NUM_16                    (0U) /* LCD_NUMPORTS16 - number of octets supporting up to 16 COMs */
#define LCD_COM_NUM                         (8U) /* LCD_CHIP_TOP_COM_NR - maximum number of commons */

#define LCD_ID(base)                        (((LCD_Type*)(base))->ID)
#define LCD_CONTROL(base)                   (((LCD_Type*)(base))->CONTROL)
#define LCD_DIVIDER(base)                   (((LCD_Type*)(base))->DIVIDER)
#define LCD_DATA0(base)                     (((LCD_Type*)(base))->DATA0)
#define LCD_DATA1(base)                     (((LCD_Type*)(base))->DATA1)
#define LCD_DATA2(base)                     (((LCD_Type*)(base))->DATA2)
#define LCD_DATA3(base)                     (((LCD_Type*)(base))->DATA3)


/*******************************************************************************
*                CTB
*******************************************************************************/

#define CTBM_CTB_CTRL(base)                 (((CTBM_Type *) (base))->CTB_CTRL)
#define CTBM_COMP_STAT(base)                (((CTBM_Type *) (base))->COMP_STAT)
#define CTBM_OA0_SW_CLEAR(base)             (((CTBM_Type *) (base))->OA0_SW_CLEAR)
#define CTBM_OA1_SW_CLEAR(base)             (((CTBM_Type *) (base))->OA1_SW_CLEAR)
#define CTBM_OA0_SW(base)                   (((CTBM_Type *) (base))->OA0_SW)
#define CTBM_OA1_SW(base)                   (((CTBM_Type *) (base))->OA1_SW)
#define CTBM_OA_RES0_CTRL(base)             (((CTBM_Type *) (base))->OA_RES0_CTRL)
#define CTBM_OA_RES1_CTRL(base)             (((CTBM_Type *) (base))->OA_RES1_CTRL)
#define CTBM_OA0_COMP_TRIM(base)            (((CTBM_Type *) (base))->OA0_COMP_TRIM)
#define CTBM_OA1_COMP_TRIM(base)            (((CTBM_Type *) (base))->OA1_COMP_TRIM)
#define CTBM_OA0_OFFSET_TRIM(base)          (((CTBM_Type *) (base))->OA0_OFFSET_TRIM)
#define CTBM_OA1_OFFSET_TRIM(base)          (((CTBM_Type *) (base))->OA1_OFFSET_TRIM)
#define CTBM_OA0_SLOPE_OFFSET_TRIM(base)    (((CTBM_Type *) (base))->OA0_SLOPE_OFFSET_TRIM)
#define CTBM_OA1_SLOPE_OFFSET_TRIM(base)    (((CTBM_Type *) (base))->OA1_SLOPE_OFFSET_TRIM)
#define CTBM_INTR(base)                     (((CTBM_Type *) (base))->INTR)
#define CTBM_INTR_SET(base)                 (((CTBM_Type *) (base))->INTR_SET)
#define CTBM_INTR_MASK(base)                (((CTBM_Type *) (base))->INTR_MASK)
#define CTBM_INTR_MASKED(base)              (((CTBM_Type *) (base))->INTR_MASKED)


/*******************************************************************************
*                SCB
*******************************************************************************/

#define SCB_CTRL(base)                      (((CySCB_Type*) (base))->CTRL)
#define SCB_SPI_CTRL(base)                  (((CySCB_Type*) (base))->SPI_CTRL)
#define SCB_SPI_STATUS(base)                (((CySCB_Type*) (base))->SPI_STATUS)
#define SCB_UART_CTRL(base)                 (((CySCB_Type*) (base))->UART_CTRL)
#define SCB_UART_TX_CTRL(base)              (((CySCB_Type*) (base))->UART_TX_CTRL)
#define SCB_UART_RX_CTRL(base)              (((CySCB_Type*) (base))->UART_RX_CTRL)
#define SCB_UART_FLOW_CTRL(base)            (((CySCB_Type*) (base))->UART_FLOW_CTRL)
#define SCB_I2C_CTRL(base)                  (((CySCB_Type*) (base))->I2C_CTRL)
#define SCB_I2C_STATUS(base)                (((CySCB_Type*) (base))->I2C_STATUS)
#define SCB_I2C_M_CMD(base)                 (((CySCB_Type*) (base))->I2C_M_CMD)
#define SCB_I2C_S_CMD(base)                 (((CySCB_Type*) (base))->I2C_S_CMD)
#define SCB_I2C_CFG(base)                   (((CySCB_Type*) (base))->I2C_CFG)
#define SCB_TX_CTRL(base)                   (((CySCB_Type*) (base))->TX_CTRL)
#define SCB_TX_FIFO_CTRL(base)              (((CySCB_Type*) (base))->TX_FIFO_CTRL)
#define SCB_TX_FIFO_STATUS(base)            (((CySCB_Type*) (base))->TX_FIFO_STATUS)
#define SCB_TX_FIFO_WR(base)                (((CySCB_Type*) (base))->TX_FIFO_WR)
#define SCB_RX_CTRL(base)                   (((CySCB_Type*) (base))->RX_CTRL)
#define SCB_RX_FIFO_CTRL(base)              (((CySCB_Type*) (base))->RX_FIFO_CTRL)
#define SCB_RX_FIFO_STATUS(base)            (((CySCB_Type*) (base))->RX_FIFO_STATUS)
#define SCB_RX_MATCH(base)                  (((CySCB_Type*) (base))->RX_MATCH)
#define SCB_RX_FIFO_RD(base)                (((CySCB_Type*) (base))->RX_FIFO_RD)
#define SCB_INTR_CAUSE(base)                (((CySCB_Type*) (base))->INTR_CAUSE)
#define SCB_INTR_I2C_EC(base)               (((CySCB_Type*) (base))->INTR_I2C_EC)
#define SCB_INTR_I2C_EC_MASK(base)          (((CySCB_Type*) (base))->INTR_I2C_EC_MASK)
#define SCB_INTR_I2C_EC_MASKED(base)        (((CySCB_Type*) (base))->INTR_I2C_EC_MASKED)
#define SCB_INTR_SPI_EC(base)               (((CySCB_Type*) (base))->INTR_SPI_EC)
#define SCB_INTR_SPI_EC_MASK(base)          (((CySCB_Type*) (base))->INTR_SPI_EC_MASK)
#define SCB_INTR_SPI_EC_MASKED(base)        (((CySCB_Type*) (base))->INTR_SPI_EC_MASKED)
#define SCB_INTR_M(base)                    (((CySCB_Type*) (base))->INTR_M)
#define SCB_INTR_M_SET(base)                (((CySCB_Type*) (base))->INTR_M_SET)
#define SCB_INTR_M_MASK(base)               (((CySCB_Type*) (base))->INTR_M_MASK)
#define SCB_INTR_M_MASKED(base)             (((CySCB_Type*) (base))->INTR_M_MASKED)
#define SCB_INTR_S(base)                    (((CySCB_Type*) (base))->INTR_S)
#define SCB_INTR_S_SET(base)                (((CySCB_Type*) (base))->INTR_S_SET)
#define SCB_INTR_S_MASK(base)               (((CySCB_Type*) (base))->INTR_S_MASK)
#define SCB_INTR_S_MASKED(base)             (((CySCB_Type*) (base))->INTR_S_MASKED)
#define SCB_INTR_TX(base)                   (((CySCB_Type*) (base))->INTR_TX)
#define SCB_INTR_TX_SET(base)               (((CySCB_Type*) (base))->INTR_TX_SET)
#define SCB_INTR_TX_MASK(base)              (((CySCB_Type*) (base))->INTR_TX_MASK)
#define SCB_INTR_TX_MASKED(base)            (((CySCB_Type*) (base))->INTR_TX_MASKED)
#define SCB_INTR_RX(base)                   (((CySCB_Type*) (base))->INTR_RX)
#define SCB_INTR_RX_SET(base)               (((CySCB_Type*) (base))->INTR_RX_SET)
#define SCB_INTR_RX_MASK(base)              (((CySCB_Type*) (base))->INTR_RX_MASK)
#define SCB_INTR_RX_MASKED(base)            (((CySCB_Type*) (base))->INTR_RX_MASKED)


/*******************************************************************************
*                WCO
*******************************************************************************/

#define WCO_CONFIG                          (((WCO_Type *) WCO)->CONFIG)
#define WCO_STATUS                          (((WCO_Type *) WCO)->STATUS)
#define WCO_DPLL                            (((WCO_Type *) WCO)->DPLL)
#define WCO_TRIM                            (((WCO_Type *) WCO)->TRIM)


/*******************************************************************************
*                EXCO
*******************************************************************************/

#define EXCO_CLK_SELECT                          (((EXCO_Type *) EXCO)->CLK_SELECT)
#define EXCO_ECO_CONFIG                          (((EXCO_Type *) EXCO)->ECO_CONFIG)
#define EXCO_ECO_STATUS                          (((EXCO_Type *) EXCO)->ECO_STATUS)
#define EXCO_PLL_CONFIG                          (((EXCO_Type *) EXCO)->PLL_CONFIG)
#define EXCO_PLL_STATUS                          (((EXCO_Type *) EXCO)->PLL_STATUS)
#define EXCO_PLL_TEST                            (((EXCO_Type *) EXCO)->PLL_TEST)
#define EXCO_EXCO_PGM_CLK                        (((EXCO_Type *) EXCO)->EXCO_PGM_CLK)
#define EXCO_ECO_TRIM0                           (((EXCO_Type *) EXCO)->ECO_TRIM0)
#define EXCO_ECO_TRIM1                           (((EXCO_Type *) EXCO)->ECO_TRIM1)
#define EXCO_ECO_TRIM2                           (((EXCO_Type *) EXCO)->ECO_TRIM2)
#define EXCO_PLL_TRIM                            (((EXCO_Type *) EXCO)->PLL_TRIM)


#endif /* CY_DEVICE_H_ */

/* [] END OF FILE */
