/***************************************************************************//**
* \file system_psoc4.h
* \version 1.0
*
* \brief Device system header file.
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


#ifndef _SYSTEM_PSOC4_H_
#define _SYSTEM_PSOC4_H_

/**
* \addtogroup group_system_config
* \{
* Provides device startup, system configuration, and linker script files.
* The system startup provides the followings features:
* - \ref group_system_config_device_memory_definition
* - \ref group_system_config_device_initialization
* - \ref group_system_config_heap_stack_config
* - \ref group_system_config_default_handlers
* - \ref group_system_config_device_vector_table
*
* \section group_system_config_configuration Configuration Considerations
*
* \subsection group_system_config_device_memory_definition Device Memory Definition
* The flash and RAM allocation is defined by the linker scripts.
*
* \note The linker files provided with the PDL are generic and handle all common
* use cases. Your project may not use every section defined in the linker files.
* In that case you may see warnings during the build process. To eliminate build
* warnings in your project, you can simply comment out or remove the relevant
* code in the linker file.
*
* <b>ARM GCC</b>\n
* The flash and RAM sections for the CPU are defined in the linker file:
* for example, 'cy8c4xx5.ld'.
*
* Change the flash and RAM sizes by editing the macros value in the
* linker file:
* \code
* __FLASH_START = 0x00000000;
* __FLASH_SIZE =  0x00008000;
*
* __RAM_START = 0x20000000;
* __RAM_SIZE =  0x00001000;
* \endcode
*
* <b>ARM MDK</b>\n
* The flash and RAM sections for the CPU are defined in the linker file:
* for example, 'cy8c4xx5.sct'.
*
* \note The linker files provided with the PDL are generic and handle all common
* use cases. Your project may not use every section defined in the linker file.
* In that case you may see the warnings during the build process:
* L6314W (no section matches pattern) and/or L6329W
* (pattern only matches removed unused sections). In your project, you can
* suppress the warning by passing the "--diag_suppress=L6314W,L6329W" option to
* the linker. You can also comment out or remove the relevant code in the linker
* file.
*
* Change the flash and RAM sizes by editing the macros value in the
* linker file:
* \code
* #define __FLASH_START  0x00000000
* #define __FLASH_SIZE   0x00008000
*
* #define __RAM_START    0x20000000
* #define __RAM_SIZE     0x00001000
* \endcode
*
* <b>IAR</b>\n
* The flash and RAM sections for the CPU are defined in the linker files:
* for example, 'cy8c4xx5.icf'.
*
* Change the flash and RAM sizes by editing the macros value in the
* linker file:
* \code
* define symbol __ICFEDIT_region_IROM1_start__ = 0x00000000;
* define symbol __ICFEDIT_region_IROM1_end__   = 0x00007FFF;
*
* define symbol __ICFEDIT_region_IRAM1_start__ = 0x20000000;
* define symbol __ICFEDIT_region_IRAM1_end__   = 0x20000FFF;
* \endcode
*
* \subsection group_system_config_device_initialization Device Initialization
* After a power-on-reset (POR), the boot process is handled by the boot code
* from the on-chip ROM that is always executed by the core. The boot code
* passes the control to the startup code located in flash.
*
* The startup code performs the device initialization by a call to Reset_Handler(),
* which calls SystemInit(), and then calls the CMSIS-defined __PROGRAM_START().
*
* \subsection group_system_config_heap_stack_config Heap and Stack Configuration
* The heap and stack configurations are defined by the linker scripts.
*
* By default, the stack size is set to 0x0000400 and the heap size is allocated
* dynamically to the whole available free memory up to stack memory and it
* is set to the 0x00000080 (for ARM GCC and IAR compilers) as minimal value.
*
* Change the heap and stack sizes by editing the macros value in the
* linker file:
* <b>ARM GCC</b>\n
* \code
* __STACK_SIZE = 0x00000400;
* __HEAP_SIZE  = 0x00000080;
* \endcode
* <b>ARM MDK</b>\n
* \code
* #define __STACK_SIZE       0x00000400
* #define __HEAP_SIZE        0x00000080
* \endcode
* <b>IAR</b>\n
* \code
* define symbol __ICFEDIT_size_cstack__ = 0x0400;
* define symbol __ICFEDIT_size_heap__ = 0x0080;
* \endcode
*
* \subsection group_system_config_default_handlers Default Interrupt Handlers Definition
* The default interrupt handler functions are defined as weak functions to a dummy
* handler in the startup file. The naming convention for the interrupt handler names
* is \<interrupt_name\>_IRQHandler. A default interrupt handler can be overwritten in
* user code by defining the handler function using the same name. For example:
* \code
* void scb_0_interrupt_IRQHandler(void)
*{
*    ...
*}
* \endcode
*
* \subsection group_system_config_device_vector_table Vectors Table Copy from Flash to RAM
* This process uses memory sections defined in the linker script. The vector table address
* (and the vector table itself) is defined in the startup files (e.g. startup_psoc4100sp.c).
* The code in these files defines the Reset_Handler, represents __VECTOR_TABLE as vector
* table in Flash and __RAM_VECTOR_TABLE represents this table in RAM.
* \n The vector table is copied from Flash to RAM in Reset_Handler using memcpy().
* \code
* memcpy(__RAM_VECTOR_TABLE, __VECTOR_TABLE, CY_VECTOR_TABLE_SIZE_BYTES);
* \endcode
*
* \section group_system_config_MISRA MISRA-C Compliance
*
* The startup driver has the following specific deviations:
*
* <table class="doxtable">
*   <tr>
*     <th>MISRA Rule</th>
*     <th>Rule Class (Required/Advisory)</th>
*     <th>Rule Description</th>
*     <th>Description of Deviation(s)</th>
*   </tr>
*   <tr>
*     <td>20.1</td>
*     <td>A</td>
*     <td>\#include directives should only be preceded by preprocessor directives
*         or comments.</td>
*     <td>The include directives are in generated files. These files should not
*         be changed manually.</td>
*   </tr>
* </table>
*
* \section group_system_config_changelog Changelog
*   <table class="doxtable">
*   <tr>
*       <th>Version</th>
*       <th>Changes</th>
*       <th>Reason for Change</th>
*   </tr>
*   <tr>
*       <td>1.0</td>
*       <td>Initial version</td>
*       <td></td>
*   </tr>
* </table>
*
* \defgroup group_system_config_system_functions Functions
* \defgroup group_system_config_globals Global Variables
*
* \}
*/

/**
* \addtogroup group_system_config_system_functions
* \{
*   \details
*   The following system functions implement CMSIS Core functions.
*   Refer to the [CMSIS documentation]
*   (http://www.keil.com/pack/doc/CMSIS/Core/html/group__system__init__gr.html "System and Clock Configuration")
*   for more details.
* \}
*/

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
* Include files
*******************************************************************************/
#include <stdint.h>


/*******************************************************************************
* Global preprocessor symbols/macros ('define')
*******************************************************************************/
#if ((defined(__GNUC__)        &&  (__ARM_ARCH == 6) && (__ARM_ARCH_6M__ == 1)) || \
     (defined (__ICCARM__)     &&  (__CORE__ == __ARM6M__))  || \
     (defined(__ARMCC_VERSION) &&  (__TARGET_ARCH_THUMB == 3)))
    #define CY_SYSTEM_CPU_CM0P          1UL
#else
    #define CY_SYSTEM_CPU_CM0P          0UL
#endif


/**
* \addtogroup group_system_config_system_functions
* \{
*/
#if defined(__ARMCC_VERSION)
    extern void SystemInit(void) __attribute__((constructor));
#else
    extern void SystemInit(void);
#endif /* (__ARMCC_VERSION) */

extern void     Cy_SystemInit(void);

extern void SystemCoreClockUpdate(void);
/** \} group_system_config_system_functions */

/** \cond */
extern void     Default_Handler (void);

extern void     Cy_OnResetUser(void);

extern uint32_t cy_delayFreqHz;
extern uint32_t cy_delayFreqKhz;
extern uint8_t  cy_delayFreqMhz;
extern uint32_t cy_delay32kMs;
/** \endcond */

/**
* \addtogroup group_system_config_globals
* \{
*/

extern uint32_t SystemCoreClock;

/** \} group_system_config_globals */


#ifdef __cplusplus
}
#endif

#endif /* _SYSTEM_PSOC4_H_ */


/* [] END OF FILE */
