/***************************************************************************//**
* \file cy_syslib.c
* \version 1.0
*
*  Description:
*   Provides system API implementation for the SysLib driver.
*
********************************************************************************
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

#include "cy_syslib.h"
#if !defined(NDEBUG)
    #include <string.h>
#endif /* NDEBUG */

/* Flash wait states */
#define CY_SYSLIB_FLASH_WS_0_FREQ_MAX       ( 16UL)
#define CY_SYSLIB_FLASH_WS_1_FREQ_MAX       ( 32UL)
/* Flash invalidation bit position in CPUSS_FLASH_CTL register*/
#define CPUSS_FLASH_INVALIDATE_BIT (1UL << 8)


#if !defined(NDEBUG)
    CY_NOINIT char_t cy_assertFileName[CY_MAX_FILE_NAME_SIZE];
    CY_NOINIT uint32_t cy_assertLine;
#endif /* NDEBUG */

#if (CY_ARM_FAULT_DEBUG == CY_ARM_FAULT_DEBUG_ENABLED)
    CY_NOINIT cy_stc_fault_frame_t cy_faultFrame;
#endif /* (CY_ARM_FAULT_DEBUG == CY_ARM_FAULT_DEBUG_ENABLED) */

#if defined(__ARMCC_VERSION)
        #if (__ARMCC_VERSION >= 6010050)
            static void Cy_SysLib_AsmInfiniteLoop(void) { __ASM (" b . "); };
        #else
            static __ASM void Cy_SysLib_AsmInfiniteLoop(void) { b . };
        #endif /* (__ARMCC_VERSION >= 6010050) */
#endif  /* (__ARMCC_VERSION) */


/*******************************************************************************
* Function Name: Cy_SysLib_Delay
****************************************************************************//**
*
* The function delays by the specified number of milliseconds.
* By default, the number of cycles to delay is calculated based on the
* \ref SystemCoreClock.
*
* \param milliseconds  The number of milliseconds to delay.
*
* \note The function calls \ref Cy_SysLib_DelayCycles() API to generate a delay.
*       If the function parameter (milliseconds) is bigger than 
*       CY_DELAY_MS_OVERFLOW constant, then an additional loop runs to prevent
*       an overflow in parameter passed to \ref Cy_SysLib_DelayCycles() API.
*
*******************************************************************************/
void Cy_SysLib_Delay(uint32_t milliseconds)
{
    uint32_t ms = milliseconds;

    while(ms > CY_DELAY_MS_OVERFLOW)
    {
        /* This loop prevents an overflow in value passed to Cy_SysLib_DelayCycles() API.
         * At 100 MHz, (ms * cy_delayFreqKhz) product overflows
         * in case if ms parameter is more than 42 seconds.
         */
        Cy_SysLib_DelayCycles(cy_delay32kMs);
        ms -= CY_DELAY_MS_OVERFLOW;
    }

    Cy_SysLib_DelayCycles(ms * cy_delayFreqKhz);
}


/*******************************************************************************
* Function Name: Cy_SysLib_DelayUs
****************************************************************************//**
*
* The function delays by the specified number of microseconds.
* By default, the number of cycles to delay is calculated based on the
* \ref SystemCoreClock.
*
* \param microseconds  The number of microseconds to delay.
*
* \note If the CPU frequency is a small non-integer number, the actual delay
*       can be up to twice as long as the nominal value. The actual delay
*       cannot be shorter than the nominal one.
*
*******************************************************************************/
void Cy_SysLib_DelayUs(uint16_t microseconds)
{
    Cy_SysLib_DelayCycles((uint32_t) microseconds * cy_delayFreqMhz);
}


/*******************************************************************************
* Function Name: Cy_SysLib_Halt
****************************************************************************//**
*
* This function halts the CPU but only the CPU which calls the function.
* It doesn't affect other CPUs.
*
* \param reason  The value to be used during debugging.
*
* \note The function executes the BKPT instruction for halting CPU and is
*       intended to be used for the debug purpose. A regular use case requires
*       Debugger attachment before the function call.
*       The BKPT instruction causes the CPU to enter the Debug state. Debug
*       tools can use this to investigate the system state, when the
*       instruction at a particular address is reached.
*
* \note Execution of a BKPT instruction without a debugger attached produces
*       a fault. The fault results in the HardFault exception being taken
*       or causes a Lockup state if it occurs in the NMI or HardFault handler.
*       The default HardFault handler make a software reset if the build option
*       is the release mode (NDEBUG). If the build option is the debug mode,
*       the system will stay in the infinite loop of the
*       \ref Cy_SysLib_ProcessingFault() function.
*
*******************************************************************************/
__NO_RETURN void Cy_SysLib_Halt(uint32_t reason)
{
    if(0U != reason)
    {
        /* To remove an unreferenced local variable warning */
    }

    #if defined (__ARMCC_VERSION)
        __breakpoint(0x0);
    #elif defined(__GNUC__)
        __asm("    bkpt    1");
    #elif defined (__ICCARM__)
        __asm("    bkpt    1");
    #else
        #error "An unsupported toolchain"
    #endif  /* (__ARMCC_VERSION) */

    while(true) {}
}


/*******************************************************************************
* Macro Name: Cy_SysLib_AssertFailed
****************************************************************************//**
*
* This function stores the ASSERT location of the file name (including path
* to file) and line number in a non-zero init area for debugging. Also it calls
* the \ref Cy_SysLib_Halt() function to halt the processor.
*
* \param file  The file name of the ASSERT location.
* \param line  The line number of the ASSERT location.
*
* \note A stored file name and line number could be accessed by
*       cy_assertFileName and cy_assertLine global variables.
* \note This function has the WEAK option, so the user can redefine
*       the function for a custom processing.
*
*******************************************************************************/
__WEAK void Cy_SysLib_AssertFailed(const char_t * file, uint32_t line)
{
#if !defined(NDEBUG) || defined(CY_DOXYGEN)
    (void) strncpy(cy_assertFileName, file, CY_MAX_FILE_NAME_SIZE);
    cy_assertLine = line;
    Cy_SysLib_Halt(0UL);
#else
    (void) file;
    (void) line;
#endif  /* !defined(NDEBUG) || defined(CY_DOXYGEN) */
}

/*******************************************************************************
* Function Name: Cy_SysLib_ClearFlashCacheAndBuffer
****************************************************************************//**
*
* This function invalidates the flash cache and buffer. It ensures the valid
* data is read from flash instead of using outdated data from the cache.
* The caches' LRU structure is also reset to their default state.
*
* \note The operation takes a maximum of three clock cycles on the slowest of
*       the clk_slow and clk_fast clocks.
*
*******************************************************************************/
void Cy_SysLib_ClearFlashCacheAndBuffer(void)
{
    /* Flash invalidation bit in FLASH_CTL register is RW1C
     * (bit is cleared upon writing 1), so write 1 to invalidate the content of
     * flash controller's buffer.
     */
    CPUSS_FLASH_CTL |= CPUSS_FLASH_INVALIDATE_BIT;
}


/*******************************************************************************
* Function Name: Cy_SysLib_GetResetReason
****************************************************************************//**
*
* The function returns the cause for the latest reset(s) that occurred in
* the system. The reset causes are taken by reading RES_CAUSE register.
*
* \return The cause of a system reset.
*
* | Name                          | Value
* |-------------------------------|---------------------
* | CY_SYSLIB_RESET_HWWDT         | 0x00001 (bit0)
* | CY_SYSLIB_PROT_FAULT          | 0x00008 (bit3)
* | CY_SYSLIB_RESET_SOFT          | 0x00010 (bit4)
*
*******************************************************************************/
uint32_t Cy_SysLib_GetResetReason(void)
{
    return (SRSS_RES_CAUSE & (CY_SYSLIB_RESET_HWWDT | CY_SYSLIB_PROT_FAULT | CY_SYSLIB_RESET_SOFT));
}


/*******************************************************************************
* Function Name: Cy_SysLib_ClearResetReason
****************************************************************************//**
*
* This function clears the values of RES_CAUSE and RES_CAUSE2.
*
*******************************************************************************/
void Cy_SysLib_ClearResetReason(void)
{
    /* RES_CAUSE register bits are RW1C (every bit is cleared upon writing 1),
     * so write all ones to clear all the reset reasons.
     */
    SRSS_RES_CAUSE = (CY_SYSLIB_RESET_HWWDT | CY_SYSLIB_PROT_FAULT | CY_SYSLIB_RESET_SOFT);
}


#if (CY_ARM_FAULT_DEBUG == CY_ARM_FAULT_DEBUG_ENABLED) || defined(CY_DOXYGEN)
/*******************************************************************************
* Function Name: Cy_SysLib_FaultHandler
****************************************************************************//**
*
* This function stores the ARM Cortex registers into a non-zero init area for
* debugging. This function calls Cy_SysLib_ProcessingFault() after storing all
* information.
*
* \param faultStackAddr The address of the stack pointer, indicates the lowest
*                       address in the fault stack frame to be stored.
* \note This function stores the fault stack frame only for the first occurred
*       fault.
* \note The PDL doesn't provide an API to analyze the stored register
*       values. The user has to add additional functions for the analysis,
*       if necessary.
* \note The CY_ARM_FAULT_DEBUG macro defines if the Fault Handler is enabled.
*       By default it is set to CY_ARM_FAULT_DEBUG_ENABLED and enables the
*       Fault Handler.
*       If there is a necessity to save memory or have some specific custom
*       handler, etc. then CY_ARM_FAULT_DEBUG should be redefined as
*       CY_ARM_FAULT_DEBUG_DISABLED. To do this, the following definition should
*       be added to the compiler Command Line (through the project Build
*       Settings): "-D CY_ARM_FAULT_DEBUG=0".
*
*******************************************************************************/
void Cy_SysLib_FaultHandler(uint32_t const *faultStackAddr)
{
    /* Stores general registers */
    cy_faultFrame.r0  = faultStackAddr[CY_R0_Pos];
    cy_faultFrame.r1  = faultStackAddr[CY_R1_Pos];
    cy_faultFrame.r2  = faultStackAddr[CY_R2_Pos];
    cy_faultFrame.r3  = faultStackAddr[CY_R3_Pos];
    cy_faultFrame.r12 = faultStackAddr[CY_R12_Pos];
    cy_faultFrame.lr  = faultStackAddr[CY_LR_Pos];
    cy_faultFrame.pc  = faultStackAddr[CY_PC_Pos];
    cy_faultFrame.psr = faultStackAddr[CY_PSR_Pos];

    Cy_SysLib_ProcessingFault();
}


/*******************************************************************************
* Function Name: Cy_SysLib_ProcessingFault
****************************************************************************//**
*
* This function determines how to process the current fault state. By default
* in case of exception the system will stay in the infinite loop of this
* function.
*
* \note This function has the WEAK option, so the user can redefine the function
*       behavior for a custom processing.
*       For example, the function redefinition could be constructed from fault
*       stack processing and NVIC_SystemReset() function call.
*
*******************************************************************************/
__WEAK void Cy_SysLib_ProcessingFault(void)
{
    #if defined(__ARMCC_VERSION)
        /* Assembly implementation of an infinite loop
         * is used for the armcc compiler to preserve the call stack.
         * Otherwise, the compiler destroys the call stack,
         * because treats this API as a no return function.
         */
        Cy_SysLib_AsmInfiniteLoop();
    #else
        while(true) {}
    #endif  /* (__ARMCC_VERSION) */
}
#endif /* (CY_ARM_FAULT_DEBUG == CY_ARM_FAULT_DEBUG_ENABLED) || defined(CY_DOXYGEN) */


/*******************************************************************************
* Function Name: Cy_SysLib_SetWaitStates
****************************************************************************//**
*
* Sets the number of clock cycles the cache will wait for, before it samples
* data coming back from ROM, SRAM, and Flash.
*
* Call this function before increasing the ClkSys clock frequency.
* Call this function optionally after lowering the ClkSys clock frequency
* in order to improve the CPU performance.
*
* \param clkHfMHz  The ClkSys clock frequency in MHz. Specifying a frequency
*                  above the supported maximum will set the wait states as for
*                  the maximum frequency.
*
*******************************************************************************/
void Cy_SysLib_SetWaitStates(uint32_t clkHfMHz)
{
    uint32_t waitStates;
    waitStates =  (clkHfMHz <= CY_SYSLIB_FLASH_WS_0_FREQ_MAX) ? 0UL :
                  ((clkHfMHz <= CY_SYSLIB_FLASH_WS_1_FREQ_MAX) ? 1UL : 2UL);

    CY_REG32_CLR_SET(CPUSS_FLASH_CTL, CPUSS_FLASH_CTL_FLASH_WS, waitStates);
}

/* [] END OF FILE */
