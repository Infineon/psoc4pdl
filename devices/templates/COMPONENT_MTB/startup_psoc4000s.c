/******************************************************************************
 * @file     startup_Device.c
 * @brief    CMSIS-Core(M) Device Startup File for <Device>
 * @version  V2.0.0
 * @date     20. May 2019
 ******************************************************************************/
/*
 * Copyright (c) 2009-2019 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdbool.h>
#include <string.h>
#include "cy_utils.h"
#include "system_psoc4.h"
#include "cy_device.h"
#include "cy_device_headers.h"
#include "cy_syslib.h"

#ifndef __RAM_VECTOR_TABLE_ATTRIBUTE
  #if defined(__ARMCC_VERSION)
    #define __RAM_VECTOR_TABLE_ATTRIBUTE __attribute((used, section(".bss.RESET_RAM")))
  #elif defined(__GNUC__)
    #define __RAM_VECTOR_TABLE_ATTRIBUTE CY_SECTION(".ram_vectors")
  #elif defined(__ICCARM__)
    #define __RAM_VECTOR_TABLE_ATTRIBUTE __attribute__ ((used, section(".intvec_ram")))
  #else
        #error "An unsupported toolchain"
    #endif  /* (__ARMCC_VERSION) */
#endif
cy_israddress __RAM_VECTOR_TABLE[CY_VECTOR_TABLE_SIZE] __RAM_VECTOR_TABLE_ATTRIBUTE; /**< Relocated vector table in SRAM */

/*----------------------------------------------------------------------------
  Exception / Interrupt Handler Function Prototype
 *----------------------------------------------------------------------------*/
typedef void( *pFunc )( void );

/*----------------------------------------------------------------------------
  External References
 *----------------------------------------------------------------------------*/
extern uint32_t __INITIAL_SP;

extern __NO_RETURN void __PROGRAM_START(void);

/*----------------------------------------------------------------------------
  Internal References
 *----------------------------------------------------------------------------*/
void __NO_RETURN Default_Handler(void);
void __NO_RETURN Reset_Handler  (void);

/*----------------------------------------------------------------------------
  Exception / Interrupt Handler
 *----------------------------------------------------------------------------*/
/* Exceptions */

#if defined ( __GNUC__ )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#if (__GNUC__ >= 9)
#pragma GCC diagnostic ignored "-Wmissing-attributes"
#endif
#endif

void NMI_Handler                        (void) __attribute__ ((weak, alias("Default_Handler")));
void HardFault_Handler                  (void) __attribute__ ((weak));
void SVC_Handler                        (void) __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler                     (void) __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler                    (void) __attribute__ ((weak, alias("Default_Handler")));

void ioss_interrupts_gpio_0_IRQHandler  (void) __attribute__ ((weak, alias("Default_Handler"))); /* GPIO P0 */
void ioss_interrupts_gpio_1_IRQHandler  (void) __attribute__ ((weak, alias("Default_Handler"))); /* GPIO P1 */
void ioss_interrupts_gpio_2_IRQHandler  (void) __attribute__ ((weak, alias("Default_Handler"))); /* GPIO P2 */
void ioss_interrupts_gpio_3_IRQHandler  (void) __attribute__ ((weak, alias("Default_Handler"))); /* GPIO P3 */
void ioss_interrupt_gpio_IRQHandler     (void) __attribute__ ((weak, alias("Default_Handler"))); /* GPIO All Ports */
void lpcomp_interrupt_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler"))); /* LPCOMP trigger interrupt */
void srss_interrupt_wdt_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler"))); /* WDT */
void scb_0_interrupt_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler"))); /* SCB #0 */
void scb_1_interrupt_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler"))); /* SCB #1 */
void cpuss_interrupt_spcif_IRQHandler   (void) __attribute__ ((weak, alias("Default_Handler"))); /* SPCIF interrupt */
void csd_interrupt_IRQHandler           (void) __attribute__ ((weak, alias("Default_Handler"))); /* CSD #0 (Primarily Capsense) */
void tcpwm_interrupts_0_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler"))); /* TCPWM #0, Counter #0 */
void tcpwm_interrupts_1_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler"))); /* TCPWM #0, Counter #1 */
void tcpwm_interrupts_2_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler"))); /* TCPWM #0, Counter #2 */
void tcpwm_interrupts_3_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler"))); /* TCPWM #0, Counter #3 */
void tcpwm_interrupts_4_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler"))); /* TCPWM #0, Counter #4 */


/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/

extern const pFunc __VECTOR_TABLE[CY_VECTOR_TABLE_SIZE];
       const pFunc __VECTOR_TABLE[CY_VECTOR_TABLE_SIZE] __VECTOR_TABLE_ATTRIBUTE = {
    (pFunc)(&__INITIAL_SP),                   /*     Initial Stack Pointer */
    Reset_Handler,                            /*     Reset Handler */
    NMI_Handler,                              /* -14 NMI Handler */
    HardFault_Handler,                        /* -13 Hard Fault Handler */
    0,                                        /*     Reserved */
    0,                                        /*     Reserved */
    0,                                        /*     Reserved */
    0,                                        /*     Reserved */
    0,                                        /*     Reserved */
    0,                                        /*     Reserved */
    0,                                        /*     Reserved */
    SVC_Handler,                              /*  -5 SVCall Handler */
    0,                                        /*     Reserved */
    0,                                        /*     Reserved */
    PendSV_Handler,                           /*  -2 PendSV Handler */
    SysTick_Handler,                          /*  -1 SysTick Handler */

    /* Interrupts */
    ioss_interrupts_gpio_0_IRQHandler,        /*   0 GPIO P0 */
    ioss_interrupts_gpio_1_IRQHandler,        /*   1 GPIO P1 */
    ioss_interrupts_gpio_2_IRQHandler,        /*   2 GPIO P2 */
    ioss_interrupts_gpio_3_IRQHandler,        /*   3 GPIO P3 */
    ioss_interrupt_gpio_IRQHandler,           /*   4 GPIO All Ports */
    lpcomp_interrupt_IRQHandler,              /*   5 LPCOMP trigger interrupt */
    srss_interrupt_wdt_IRQHandler,            /*   6 WDT */
    scb_0_interrupt_IRQHandler,               /*   7 SCB #0 */
    scb_1_interrupt_IRQHandler,               /*   8 SCB #1 */
    cpuss_interrupt_spcif_IRQHandler,         /*   9 SPCIF interrupt */
    csd_interrupt_IRQHandler,                 /*  10 CSD #0 (Primarily Capsense) */
    tcpwm_interrupts_0_IRQHandler,            /*  11 TCPWM #0, Counter #0 */
    tcpwm_interrupts_1_IRQHandler,            /*  12 TCPWM #0, Counter #1 */
    tcpwm_interrupts_2_IRQHandler,            /*  13 TCPWM #0, Counter #2 */
    tcpwm_interrupts_3_IRQHandler,            /*  14 TCPWM #0, Counter #3 */
    tcpwm_interrupts_4_IRQHandler             /*  15 TCPWM #0, Counter #4 */
};

#if defined ( __GNUC__ )
#pragma GCC diagnostic pop
#endif

/*----------------------------------------------------------------------------
  Reset Handler called on controller reset
 *----------------------------------------------------------------------------*/
void Reset_Handler(void)
{
    Cy_OnResetUser();

    /* CMSIS System Initialization */
    SystemInit();

    /* Copy vector table from ROAM to RAM*/
    memcpy(__RAM_VECTOR_TABLE, __VECTOR_TABLE, CY_VECTOR_TABLE_SIZE_BYTES);

    /* Set vector table offset */
    SCB->VTOR = (uint32_t)&__RAM_VECTOR_TABLE;
    __DSB();

    /* Enter PreMain (C library entry point) */
    __PROGRAM_START();
}


/*----------------------------------------------------------------------------
  Default Handler for Exceptions / Interrupts
 *----------------------------------------------------------------------------*/
void Default_Handler(void)
{
    while(1);
}

/*----------------------------------------------------------------------------
  Default Handler for Hard Fault
 *----------------------------------------------------------------------------*/
__WEAK void HardFault_Handler(void)
{
    Cy_SysLib_ProcessingFault();
}

__WEAK void Cy_OnResetUser(void)
{
    /* Empty weak function. The actual implementation to be in the provided by 
    *  the application code strong function.
    */
}

/* [] END OF FILE */
