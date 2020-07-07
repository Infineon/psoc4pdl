/***************************************************************************//**
* \file cy_syslib.h
* \version 1.0
*
* Provides an API declaration of the SysLib driver.
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

/**
* \addtogroup group_syslib
* \{
* The system libraries provide APIs that can be called in the user application
* to handle the timing, logical checking or register.
*
* The functions and other declarations used in this driver are in cy_syslib.h.
* You can include cy_pdl.h (ModusToolbox only) to get access to all functions
* and declarations in the PDL.
*
* The SysLib driver contains a set of different system functions. These functions
* can be called in the application routine. Major features of the system library:
* * Delay functions
* * The register Read/Write macro
* * Assert and Halt
* * Assert Classes and Levels
* * Reading the reset cause
* * An API to invalidate the flash cache and buffer
* * Data manipulation macro
* * A variable type definition from MISRA-C which specifies signedness
* * Cross compiler compatible attributes
* * Setting wait states API
* * APIs to serve Fault handler
*
* \section group_syslib_configuration Configuration Considerations
* <b> Assertion Usage </b> <br />
* Use the CY_ASSERT() macro to check expressions that must be true as long as the
* program is running correctly. It is a convenient way to insert sanity checks.
* The CY_ASSERT() macro is defined in the cy_syslib.h file which is part of
* the PDL library. The behavior of the macro is as follows: if the expression
* passed to the macro is false, output an error message that includes the file
* name and line number, and then halts the CPU. \n
* In case of fault, the CY_ASSERT() macro calls the Cy_SysLib_AssertFailed() function.
* This is a weakly linked function. The default implementation stores the file
* name and line number of the ASSERT into global variables, cy_assertFileName
* and cy_assertLine . It then calls the Cy_SysLib_Halt() function.
* \note Firmware can redefine the Cy_SysLib_AssertFailed() function for custom processing.
*
* The PDL source code uses this assert mechanism extensively. It is recommended
* that you enable asserts when debugging firmware. \n
* <b> Assertion Classes and Levels </b> <br />
* The PDL defines three assert classes, which correspond to different kinds
* of parameters. There is a corresponding assert "level" for each class.
* <table class="doxtable">
*   <tr><th>Class Macro</th><th>Level Macro</th><th>Type of check</th></tr>
*   <tr>
*     <td>CY_ASSERT_CLASS_1</td>
*     <td>CY_ASSERT_L1</td>
*     <td>A parameter that could change between different PSoC devices
*         (e.g. the number of clock paths)</td>
*   </tr>
*   <tr>
*     <td>CY_ASSERT_CLASS_2</td>
*     <td>CY_ASSERT_L2</td>
*     <td>A parameter that has fixed limits such as a counter period</td>
*   </tr>
*   <tr>
*     <td>CY_ASSERT_CLASS_3</td>
*     <td>CY_ASSERT_L3</td>
*     <td>A parameter that is an enum constant</td>
*   </tr>
* </table>
* Firmware defines which ASSERT class is enabled by defining CY_ASSERT_LEVEL.
* This is a compiler command line argument, similar to how the DEBUG / NDEBUG
* macro is passed. \n
* Enabling any class also enables any lower-numbered class.
* CY_ASSERT_CLASS_3 is the default level, and it enables asserts for all three
* classes. The following example shows the command-line option to enable all
* the assert levels:
* \code -D CY_ASSERT_LEVEL=CY_ASSERT_CLASS_3 \endcode
* \note The use of special characters, such as spaces, parenthesis, etc. must
* be protected with quotes.
*
* After CY_ASSERT_LEVEL is defined, firmware can use
* one of the three level macros to make an assertion. For example, if the
* parameter can vary between devices, firmware uses the L1 macro.
* \code CY_ASSERT_L1(clkPath < SRSS_NUM_CLKPATH); \endcode
* If the parameter has bounds, firmware uses L2.
* \code CY_ASSERT_L2(trim <= CY_CTB_TRIM_VALUE_MAX); \endcode
* If the parameter is an enum, firmware uses L3.
* \code CY_ASSERT_L3(config->LossAction <= CY_SYSCLK_CSV_ERROR_FAULT_RESET); \endcode
* Each check uses the appropriate level macro for the kind of parameter being checked.
* If a particular assert class/level is not enabled, then the assert does nothing.
*
* \section group_syslib_more_information More Information
* Refer to the technical reference manual (TRM).
*
* \section group_syslib_MISRA MISRA-C Compliance
*
* The SysLib driver has the following specific deviations:
*
* <table class="doxtable">
*   <tr>
*     <th>MISRA Rule</th>
*     <th>Rule/Directive Class (Required/Advisory)</th>
*     <th>Rule/Directive Description</th>
*     <th>Description of Deviation(s)</th>
*   </tr>
*   <tr>
*     <td>Directive 4.3</td>
*     <td>R</td>
*     <td>Assembly language shall be encapsulated and isolated.</td>
*     <td>Not critical. Deviation will be fixed in final release.</td>
*   </tr>
*   <tr>
*     <td>Directive 4.5</td>
*     <td>A</td>
*     <td>Identifiers in the same name space with overlapping visibility should
*         be typographically unambiguous.</td>
*     <td>List of macro is provided for backward compatibility. It will be
*         removed, when 3rd part software is updated.</td>
*   </tr>
*   <tr>
*     <td>8.9</td>
*     <td>A</td>
*     <td>An object should be defined at block scope if its identifier only
*         appears in a single function.</td>
*     <td>PDL is a library. The \ref cy_faultFrame is provided as part user API.</td>
*   </tr>
* \if MISRA
*   <tr>
*     <td>8.6</td>
*     <td>R</td>
*     <td>An identifier with external linkage shall have exactly one external
*         definition.</td>
*     <td>IGNORE: Tools issue. Coverity could not find definition in assembler
*         file.</td>
*   </tr>
* \endif
* </table>
*
* \section group_syslib_changelog Changelog
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
*   <tr>
*     <td>1.0</td>
*     <td>Initial version</td>
*     <td></td>
*   </tr>
* </table>
*
* \defgroup group_syslib_macros Macros
* \defgroup group_syslib_functions Functions
* \defgroup group_syslib_data_structures Data Structures
* \defgroup group_syslib_enumerated_types Enumerated Types
*
*/

#if !defined(CY_SYSLIB_H)
#define CY_SYSLIB_H

#include <stdint.h>
#include <stdbool.h>
#include "cy_utils.h"
#include "cy_result.h"
#include "cy_device.h"
#include "cy_device_headers.h"

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#if defined( __ICCARM__ )
    /* Suppress the warning for multiple volatile variables in an expression. */
    /* This is common for driver's code and the usage is not order-dependent. */
    #pragma diag_suppress=Pa082
#endif  /* defined( __ICCARM__ ) */

/**
* \addtogroup group_syslib_macros
* \{
*/

/******************************************************************************
* Macros
*****************************************************************************/

#define CY_CPU_CORTEX_M0P   (__CORTEX_M == 0)    /**< CM0+ core CPU Code */

/** The macro to disable the Fault Handler */
#define CY_ARM_FAULT_DEBUG_DISABLED    (0U)
/** The macro to enable the Fault Handler */
#define CY_ARM_FAULT_DEBUG_ENABLED     (1U)

#if !defined(CY_ARM_FAULT_DEBUG)
    /** The macro defines if the Fault Handler is enabled. Enabled by default. */
    #define CY_ARM_FAULT_DEBUG         (CY_ARM_FAULT_DEBUG_ENABLED)
#endif /* CY_ARM_FAULT_DEBUG */

#define CY_VECTOR_TABLE_SIZE (48)                              /**< Size of the vector table */
#define CY_VECTOR_TABLE_SIZE_BYTES (CY_VECTOR_TABLE_SIZE * 4)  /**< Size of the vector table in bytes */
/**
* \defgroup group_syslib_macros_status_codes Status codes
* \{
* Function status type codes
*/
#define CY_PDL_STATUS_CODE_Pos  (CY_RSLT_CODE_POSITION)     /**< The module status code position in the status code */
#define CY_PDL_STATUS_TYPE_Pos  (CY_RSLT_TYPE_POSITION)     /**< The status type position in the status code */
#define CY_PDL_MODULE_ID_Pos    (CY_RSLT_MODULE_POSITION)   /**< The software module ID position in the status code */
#define CY_PDL_STATUS_INFO      ((uint32_t)CY_RSLT_TYPE_INFO << CY_PDL_STATUS_TYPE_Pos)     /**< The information status type */
#define CY_PDL_STATUS_WARNING   ((uint32_t)CY_RSLT_TYPE_WARNING << CY_PDL_STATUS_TYPE_Pos)  /**< The warning status type */
#define CY_PDL_STATUS_ERROR     ((uint32_t)CY_RSLT_TYPE_ERROR << CY_PDL_STATUS_TYPE_Pos)    /**< The error status type */
#define CY_PDL_MODULE_ID_Msk    (CY_RSLT_MODULE_MASK)       /**< The software module ID mask */
/** Get the software PDL module ID */
#define CY_PDL_DRV_ID(id)       ((uint32_t)((uint32_t)((id) & CY_PDL_MODULE_ID_Msk) << CY_PDL_MODULE_ID_Pos))
#define CY_SYSLIB_ID            CY_PDL_DRV_ID(0x11U)     /**< SYSLIB PDL ID */
/** \} group_syslib_macros_status_codes */

/** \} group_syslib_macros */

/**
* \addtogroup group_syslib_enumerated_types
* \{
*/

/** The SysLib status code structure. */
typedef enum
{
    CY_SYSLIB_SUCCESS       = 0x00UL,    /**< The success status code */
    CY_SYSLIB_BAD_PARAM     = CY_SYSLIB_ID | CY_PDL_STATUS_ERROR | 0x01UL,    /**< The bad parameter status code */
    CY_SYSLIB_TIMEOUT       = CY_SYSLIB_ID | CY_PDL_STATUS_ERROR | 0x02UL,    /**< The time out status code */
    CY_SYSLIB_INVALID_STATE = CY_SYSLIB_ID | CY_PDL_STATUS_ERROR | 0x03UL,    /**< The invalid state status code */
    CY_SYSLIB_UNKNOWN       = CY_SYSLIB_ID | CY_PDL_STATUS_ERROR | 0xFFUL     /**< Unknown status code */
} cy_en_syslib_status_t;

/** \} group_syslib_enumerated_types */
/**
* \addtogroup group_syslib_data_structures
* \{
*/

#if (CY_ARM_FAULT_DEBUG == CY_ARM_FAULT_DEBUG_ENABLED)
    /** The fault configuration structure. */
    typedef struct
    {
        uint32_t r0;       /**< R0 register content */
        uint32_t r1;       /**< R1 register content */
        uint32_t r2;       /**< R2 register content */
        uint32_t r3;       /**< R3 register content */
        uint32_t r12;      /**< R12 register content */
        uint32_t lr;       /**< LR register content */
        uint32_t pc;       /**< PC register content */
        uint32_t psr;      /**< PSR register content */
    } cy_stc_fault_frame_t;
#endif /* (CY_ARM_FAULT_DEBUG == CY_ARM_FAULT_DEBUG_ENABLED) */

/** \} group_syslib_data_structures */

/**
* \addtogroup group_syslib_macros
* \{
*/

/** The driver major version */
#define CY_SYSLIB_DRV_VERSION_MAJOR    1

/** The driver minor version */
#define CY_SYSLIB_DRV_VERSION_MINOR    0

typedef void (* cy_israddress)(void);   /**< Type of ISR callbacks */
#if defined (__ICCARM__)
    typedef union { cy_israddress __fun; void * __ptr; } cy_intvec_elem;
#endif  /* defined (__ICCARM__) */

/* MISRA rule 6.3 recommends using specific-length typedef for the basic
 * numerical types of signed and unsigned variants of char, float, and double.
 */
typedef char     char_t;    /**< Specific-length typedef for the basic numerical types of char */
typedef float    float32_t; /**< Specific-length typedef for the basic numerical types of float */
typedef double   float64_t; /**< Specific-length typedef for the basic numerical types of double */

#if !defined(NDEBUG)
    /** The max size of the file name which stores the ASSERT location */
    #define CY_MAX_FILE_NAME_SIZE  (24U)
    extern CY_NOINIT char_t cy_assertFileName[CY_MAX_FILE_NAME_SIZE];  /**< The assert buffer */
    extern CY_NOINIT uint32_t cy_assertLine;                           /**< The assert line value */
#endif /* NDEBUG */

#if (CY_ARM_FAULT_DEBUG == CY_ARM_FAULT_DEBUG_ENABLED)
    #define CY_R0_Pos             (0U)     /**< The position of the R0  content in a fault structure */
    #define CY_R1_Pos             (1U)     /**< The position of the R1  content in a fault structure */
    #define CY_R2_Pos             (2U)     /**< The position of the R2  content in a fault structure */
    #define CY_R3_Pos             (3U)     /**< The position of the R3  content in a fault structure */
    #define CY_R12_Pos            (4U)     /**< The position of the R12 content in a fault structure */
    #define CY_LR_Pos             (5U)     /**< The position of the LR  content in a fault structure */
    #define CY_PC_Pos             (6U)     /**< The position of the PC  content in a fault structure */
    #define CY_PSR_Pos            (7U)     /**< The position of the PSR content in a fault structure */

    extern CY_NOINIT cy_stc_fault_frame_t cy_faultFrame;    /**< Fault frame structure */
#endif /* (CY_ARM_FAULT_DEBUG == CY_ARM_FAULT_DEBUG_ENABLED) */


/**
* \defgroup group_syslib_macros_assert Assert Classes and Levels
* \{
* Defines for the Assert Classes and Levels
*/

/**
* Class 1 - The highest class, safety-critical functions which rely on parameters that could be
* changed between different PSoC devices
*/
#define CY_ASSERT_CLASS_1           (1U)

/** Class 2 - Functions that have fixed limits such as counter periods (16-bits/32-bits etc.) */
#define CY_ASSERT_CLASS_2           (2U)

/** Class 3 - Functions that accept enums as constant parameters */
#define CY_ASSERT_CLASS_3           (3U)

#ifndef CY_ASSERT_LEVEL
    /** The user-definable assert level from compiler command-line argument (similarly to DEBUG / NDEBUG) */
    #define CY_ASSERT_LEVEL         CY_ASSERT_CLASS_3
#endif /* CY_ASSERT_LEVEL */

#if (CY_ASSERT_LEVEL == CY_ASSERT_CLASS_1)
    #define CY_ASSERT_L1(x)         CY_ASSERT(x)     /**< Assert Level 1 */
    #define CY_ASSERT_L2(x)         do{} while(0)    /**< Assert Level 2 */
    #define CY_ASSERT_L3(x)         do{} while(0)    /**< Assert Level 3 */
#elif (CY_ASSERT_LEVEL == CY_ASSERT_CLASS_2)
    #define CY_ASSERT_L1(x)         CY_ASSERT(x)     /**< Assert Level 1 */
    #define CY_ASSERT_L2(x)         CY_ASSERT(x)     /**< Assert Level 2 */
    #define CY_ASSERT_L3(x)         do{} while(0)    /**< Assert Level 3 */
#else /* Default is Level 3 */
    #define CY_ASSERT_L1(x)         CY_ASSERT(x)     /**< Assert Level 1 */
    #define CY_ASSERT_L2(x)         CY_ASSERT(x)     /**< Assert Level 2 */
    #define CY_ASSERT_L3(x)         CY_ASSERT(x)     /**< Assert Level 3 */
#endif /* CY_ASSERT_LEVEL == CY_ASSERT_CLASS_1 */

/** \} group_syslib_macros_assert */


/******************************************************************************
* Constants
*****************************************************************************/
/** Defines a 32-kHz clock delay */
#define CY_DELAY_MS_OVERFLOW            (0x8000U)

/**
* \defgroup group_syslib_macros_reset_cause Reset cause
* \{
* Define RESET_CAUSE mask values
*/
/** A basic WatchDog Timer (WDT) reset has occurred since the last power cycle. */
#define CY_SYSLIB_RESET_HWWDT           (0x0001UL)
/** A protection violation occurred that requires a RESET. */
#define CY_SYSLIB_PROT_FAULT            (0x0008UL)
/** The CPU requested a system reset through it's SYSRESETREQ. This can be done via a debugger probe or in firmware. */
#define CY_SYSLIB_RESET_SOFT            (0x0010UL)

/** \} group_syslib_macros_reset_cause */

/** \} group_syslib_macros */

/******************************************************************************
* Function prototypes
******************************************************************************/

/**
* \addtogroup group_syslib_functions
* \{
*/

void Cy_SysLib_Delay(uint32_t milliseconds);
void Cy_SysLib_DelayUs(uint16_t microseconds);
/** Delays for the specified number of cycles.
 *  The function is implemented in the assembler for each supported compiler.
 *  \param cycles  The number of cycles to delay.
 */
void Cy_SysLib_DelayCycles(uint32_t cycles);
__NO_RETURN void Cy_SysLib_Halt(uint32_t reason);
void Cy_SysLib_AssertFailed(const char_t * file, uint32_t line);
void Cy_SysLib_ClearFlashCacheAndBuffer(void);
uint32_t Cy_SysLib_GetResetReason(void);
void Cy_SysLib_ClearResetReason(void);
#if (CY_ARM_FAULT_DEBUG == CY_ARM_FAULT_DEBUG_ENABLED) || defined(CY_DOXYGEN)
    void Cy_SysLib_FaultHandler(uint32_t const *faultStackAddr);
    void Cy_SysLib_ProcessingFault(void);
#endif /* (CY_ARM_FAULT_DEBUG == CY_ARM_FAULT_DEBUG_ENABLED) */
void Cy_SysLib_SetWaitStates(uint32_t clkHfMHz);


/*******************************************************************************
* Function Name: Cy_SysLib_EnterCriticalSection
****************************************************************************//**
*
*  Cy_SysLib_EnterCriticalSection disables interrupts and returns a value
*  indicating whether the interrupts were previously enabled.
*
*  \return Returns the current interrupt status. Returns 0 if the interrupts
*          were previously enabled or 1 if the interrupts were previously
*          disabled.
*
*  \note Implementation of Cy_SysLib_EnterCriticalSection manipulates the IRQ
*        enable bit with interrupts still enabled.
*
*******************************************************************************/
uint32_t Cy_SysLib_EnterCriticalSection(void);


/*******************************************************************************
* Function Name: Cy_SysLib_ExitCriticalSection
****************************************************************************//**
*
*  Re-enables the interrupts if they were enabled before
*  Cy_SysLib_EnterCriticalSection() was called. The argument should be the value
*  returned from \ref Cy_SysLib_EnterCriticalSection().
*
*  \param savedIntrStatus  Puts the saved interrupts status returned by
*                          the \ref Cy_SysLib_EnterCriticalSection().
*
*******************************************************************************/
void Cy_SysLib_ExitCriticalSection(uint32_t savedIntrStatus);

/** \} group_syslib_functions */

/** \cond */
typedef uint32_t cy_status;
/** The ARM 32-bit status value for backward compatibility with the UDB components. Do not use it in your code. */
typedef uint32_t cystatus;
typedef uint8_t  uint8;    /**< Alias to uint8_t  for backward compatibility */
typedef uint16_t uint16;   /**< Alias to uint16_t for backward compatibility */
typedef uint32_t uint32;   /**< Alias to uint32_t for backward compatibility */
typedef int8_t   int8;     /**< Alias to int8_t   for backward compatibility */
typedef int16_t  int16;    /**< Alias to int16_t  for backward compatibility */
typedef int32_t  int32;    /**< Alias to int32_t  for backward compatibility */
typedef float    float32;  /**< Alias to float    for backward compatibility */
typedef double   float64;  /**< Alias to double   for backward compatibility */
typedef int64_t  int64;    /**< Alias to int64_t  for backward compatibility */
typedef uint64_t uint64;   /**< Alias to uint64_t for backward compatibility */
/* Signed or unsigned depending on the compiler selection */
typedef char     char8;    /**< Alias to char for backward compatibility */
typedef volatile uint8_t  reg8;   /**< Alias to uint8_t  for backward compatibility */
typedef volatile uint16_t reg16;  /**< Alias to uint16_t for backward compatibility */
typedef volatile uint32_t reg32;  /**< Alias to uint32_t for backward compatibility */

/** The ARM 32-bit Return error / status code for backward compatibility.
*  Do not use them in your code.
*/
#define CY_RET_SUCCESS           (0x00U)    /**< Successful */
#define CY_RET_BAD_PARAM         (0x01U)    /**< One or more invalid parameters */
#define CY_RET_INVALID_OBJECT    (0x02U)    /**< An invalid object specified */
#define CY_RET_MEMORY            (0x03U)    /**< A memory-related failure */
#define CY_RET_LOCKED            (0x04U)    /**< A resource lock failure */
#define CY_RET_EMPTY             (0x05U)    /**< No more objects available */
#define CY_RET_BAD_DATA          (0x06U)    /**< Bad data received (CRC or other error check) */
#define CY_RET_STARTED           (0x07U)    /**< Operation started, but not necessarily completed yet */
#define CY_RET_FINISHED          (0x08U)    /**< Operation is completed */
#define CY_RET_CANCELED          (0x09U)    /**< Operation is canceled */
#define CY_RET_TIMEOUT           (0x10U)    /**< Operation timed out */
#define CY_RET_INVALID_STATE     (0x11U)    /**< Operation is not setup or is in an improper state */
#define CY_RET_UNKNOWN           ((cy_status) 0xFFFFFFFFU)    /**< Unknown failure */

/** ARM 32-bit Return error / status codes for backward compatibility with the UDB components.
*  Do not use them in your code.
*/
#define CYRET_SUCCESS            (0x00U)    /**< Successful */
#define CYRET_BAD_PARAM          (0x01U)    /**< One or more invalid parameters */
#define CYRET_INVALID_OBJECT     (0x02U)    /**< An invalid object specified */
#define CYRET_MEMORY             (0x03U)    /**< A memory-related failure */
#define CYRET_LOCKED             (0x04U)    /**< A resource lock failure */
#define CYRET_EMPTY              (0x05U)    /**< No more objects available */
#define CYRET_BAD_DATA           (0x06U)    /**< Bad data received (CRC or other error check) */
#define CYRET_STARTED            (0x07U)    /**< Operation started, but not necessarily completed yet */
#define CYRET_FINISHED           (0x08U)    /**< Operation is completed */
#define CYRET_CANCELED           (0x09U)    /**< Operation is canceled */
#define CYRET_TIMEOUT            (0x10U)    /**< Operation timed out */
#define CYRET_INVALID_STATE      (0x11U)    /**< Operation is not setup or is in an improper state */
#define CYRET_UNKNOWN            ((cystatus) 0xFFFFFFFFU)    /**< Unknown failure */

/** A type of ISR callbacks for backward compatibility with the UDB components. Do not use it in your code. */
typedef void (* cyisraddress)(void);
#if defined (__ICCARM__)
    /** A type of ISR callbacks for backward compatibility with the UDB components. Do not use it in your code. */
    typedef union { cyisraddress __fun; void * __ptr; } intvec_elem;
#endif  /* defined (__ICCARM__) */

/** \endcond */

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */

#endif /* CY_SYSLIB_H */

/** \} group_syslib */

/* [] END OF FILE */
