/***************************************************************************//**
* \file system_psoc4.c
* \version 1.0
*
* The device system-source file.
*
********************************************************************************
* \copyright
* Copyright 2016-2019 Cypress Semiconductor Corporation
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

#include <stdbool.h>
#include "system_psoc4.h"
#include "cy_device.h"
#include "cy_device_headers.h"
#include "cy_syslib.h"
#include "cy_sysclk.h"
#include "cy_wdt.h"


/*******************************************************************************
* SystemCoreClockUpdate()
*******************************************************************************/

/** Default ClkSys system core frequency in Hz */
#define CY_CLK_SYSTEM_FREQ_HZ_DEFAULT       (12000000UL)


/**
* Holds the ClkSys system core clock,
* which is the system clock frequency supplied to the SysTick timer and the
* processor core clock.
* This variable implements CMSIS Core global variable.
* Refer to the [CMSIS documentation]
* (http://www.keil.com/pack/doc/CMSIS/Core/html/group__system__init__gr.html "System and Clock Configuration")
* for more details.
* This variable can be used by debuggers to query the frequency
* of the debug timer or to configure the trace clock speed.
*
* \attention Compilers must be configured to avoid removing this variable in case
* the application program is not using it. Debugging systems require the variable
* to be physically present in memory so that it can be examined to configure the debugger. */
uint32_t SystemCoreClock = CY_CLK_SYSTEM_FREQ_HZ_DEFAULT;


/*******************************************************************************
* SystemCoreClockUpdate (void)
*******************************************************************************/

/* Do not use these definitions directly in your application */
#define CY_DELAY_MS_OVERFLOW_THRESHOLD  (0x8000u)
#define CY_DELAY_1K_THRESHOLD           (1000u)
#define CY_DELAY_1M_THRESHOLD           (1000000u)

uint32_t cy_delayFreqKhz  = CY_SYSLIB_DIV_ROUNDUP(CY_CLK_SYSTEM_FREQ_HZ_DEFAULT, CY_DELAY_1K_THRESHOLD);

uint8_t cy_delayFreqMhz  = (uint8_t)CY_SYSLIB_DIV_ROUNDUP(CY_CLK_SYSTEM_FREQ_HZ_DEFAULT, CY_DELAY_1M_THRESHOLD);

uint32_t cy_delay32kMs    = CY_DELAY_MS_OVERFLOW_THRESHOLD *
                            CY_SYSLIB_DIV_ROUNDUP(CY_CLK_SYSTEM_FREQ_HZ_DEFAULT, CY_DELAY_1K_THRESHOLD);


/*******************************************************************************
* Function Name: SystemInit
****************************************************************************//**
*
* Initializes the system:
* - Unlocks and disables WDT.
* - Disables interrupts.
* - Calls the Cy_SystemInit() function.
*
* \note
* This function does not initialize clocks.
* To set up clocks in pre-main, define strong function \ref Cy_SystemInit with 
* your custom clock initialization.
*
*******************************************************************************/
void SystemInit(void)
{
    /* Disable WDT */
    Cy_WDT_Disable();

    /* Disable interrupts */
    __disable_irq();

    /* Call Cy_SystemInit */
    Cy_SystemInit();
}


/*******************************************************************************
* Function Name: Cy_SystemInit
****************************************************************************//**
*
* Empty weak function. The application code provides a strong function 
* for the actual implementation.
*
*******************************************************************************/
__WEAK void Cy_SystemInit(void)
{
    /* A strong function can be provided instead of this function */
}


/*******************************************************************************
* Function Name: SystemCoreClockUpdate
****************************************************************************//**
*
* Gets core clock frequency and updates \ref SystemCoreClock.
*
* Updates global variables used by the \ref Cy_SysLib_Delay(), \ref
* Cy_SysLib_DelayUs(), and \ref Cy_SysLib_DelayCycles().
*
*******************************************************************************/
void SystemCoreClockUpdate (void)
{
    uint32_t locCoreClock = Cy_SysClk_ClkSysGetFrequency();

    if (0UL != locCoreClock)
    {
        SystemCoreClock = locCoreClock;

        /* Sets clock frequency for Delay API */
        cy_delayFreqMhz = (uint8_t)CY_SYSLIB_DIV_ROUNDUP(SystemCoreClock, CY_DELAY_1M_THRESHOLD);
        cy_delayFreqKhz = CY_SYSLIB_DIV_ROUNDUP(SystemCoreClock, CY_DELAY_1K_THRESHOLD);
        cy_delay32kMs   = CY_DELAY_MS_OVERFLOW_THRESHOLD * cy_delayFreqKhz;
    }
}


/* [] END OF FILE */
