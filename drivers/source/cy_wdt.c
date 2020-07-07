/***************************************************************************//**
* \file cy_wdt.c
* \version 1.0
*
*  This file provides the source code to the API for the WDT driver.
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

#include "cy_wdt.h"
#include "cy_syslib.h"

#if defined(__cplusplus)
extern "C" {
#endif


/*******************************************************************************
* Function Name: Cy_WDT_Init
****************************************************************************//**
*
* Initializes the Watchdog timer to its default state.
*
* The given default setting of the WDT:
* The WDT match value is 4096.
* None of ignore bits are set: the whole WDT counter bits are checked against 
* the match value.
*
* \sideeffect
* This function clears the WDT interrupt.
*
*******************************************************************************/
void Cy_WDT_Init(void)
{
    /* Unlock the WDT by two writes */

    Cy_WDT_Disable();
    Cy_WDT_SetMatch(CY_SRSS_WDT_DEFAULT_MATCH_VALUE);
    Cy_WDT_SetIgnoreBits(CY_SRSS_WDT_DEFAULT_IGNORE_BITS);
    Cy_WDT_ClearInterrupt();
}


/*******************************************************************************
* Function Name: Cy_WDT_SetMatch
****************************************************************************//**
*
* Configures the WDT counter match comparison value.
*
* \param match
* The valid valid range is [0-65535]. The value to be used to match 
* against the counter.
*
*******************************************************************************/
void Cy_WDT_SetMatch(uint32_t match)
{
    CY_ASSERT_L2(CY_WDT_IS_MATCH_VALID(match));
    
    CY_REG32_CLR_SET(SRSSLT_WDT_MATCH, SRSSLT_WDT_MATCH_MATCH, match);
}


/*******************************************************************************
* Function Name: Cy_WDT_SetIgnoreBits
****************************************************************************//**
*
* Configures the number of the most significant bits of the Watchdog timer that 
* are not checked against the match.
*
* \param bitsNum
* The number of the most significant bits. The valid range is [0-15].
*
* \details The value of bitsNum controls the time-to-reset of the Watchdog timer.
* This happens after 3 successive matches.
*
* \note Certain products may enforce a minimum value for this register 
* through design time configuration.
*
* \warning This function changes the WDT interrupt period, therefore 
* the device can go into an infinite WDT reset loop. This may happen
* if a WDT reset occurs faster that a device start-up.
*
*******************************************************************************/
void Cy_WDT_SetIgnoreBits(uint32_t bitsNum)
{
    CY_ASSERT_L2(CY_WDT_IS_IGNORE_BITS_VALID(bitsNum));
    CY_REG32_CLR_SET(SRSSLT_WDT_MATCH, SRSSLT_WDT_MATCH_IGNORE_BITS, bitsNum);
}


/*******************************************************************************
* Function Name: Cy_WDT_ClearInterrupt
****************************************************************************//**
*
* Clears the WDT match flag which is set every time the WDT counter reaches a 
* WDT match value. Two unserviced interrupts lead to a system reset 
* (i.e. at the third match).
*
*******************************************************************************/
void Cy_WDT_ClearInterrupt(void)
{
    SRSSLT_SRSS_INTR = SRSSLT_SRSS_INTR_WDT_MATCH_Msk;

    /* Read the interrupt register to ensure that the initial clearing write has
    * been flushed out to the hardware.
    */
    (void) SRSSLT_SRSS_INTR;
}


#if defined(__cplusplus)
}
#endif


/* [] END OF FILE */
