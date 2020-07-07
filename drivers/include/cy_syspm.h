/***************************************************************************//**
* \file cy_syspm.h
* \version 1.0
*
* Provides the function definitions for the power management API.
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
*
*******************************************************************************/

/**
* \addtogroup group_syspm
* \{
*
* Use the System Power Management (SysPm) driver to change power modes and
* reduce system power consumption in power sensitive designs.
*
* The functions and other declarations used in this driver are in cy_syspm.h.
* You can include cy_pdl.h (ModusToolbox only) to get access to all functions
* and declarations in the PDL.
*
* \if MISRA
* \section group_smartio_MISRA MISRA-C Compliance]
* <table class="doxtable">
*   <tr>
*     <th>MISRA Rule</th>
*     <th>Rule Class (Required/Advisory)</th>
*     <th>Rule Description</th>
*     <th>Description of Deviation(s)</th>
*   </tr>
*   <tr>
*     <td>Directive 4.4</td>
*     <td>TBD</td>
*     <td>TBD</td>
*     <td>Driver is out of Alpha Release. Ignoring MISRA violations</td>
*   </tr>
*   <tr>
*     <td>8.6</td>
*     <td>TBD</td>
*     <td>TBD</td>
*     <td>Driver is out of Alpha Release. Ignoring MISRA violations</td>
*   </tr>
*   <tr>
*     <td>8.13</td>
*     <td>TBD</td>
*     <td>TBD</td>
*     <td>Driver is out of Alpha Release. Ignoring MISRA violations</td>
*   </tr>
*   <tr>
*     <td>14.4</td>
*     <td>TBD</td>
*     <td>TBD</td>
*     <td>Driver is out of Alpha Release. Ignoring MISRA violations</td>
*   </tr>
*   <tr>
*     <td>20.9</td>
*     <td>TBD</td>
*     <td>TBD</td>
*     <td>Driver is out of Alpha Release. Ignoring MISRA violations</td>
*   </tr>
* </table>
* \endif
*
* \defgroup group_syspm_macros Macros
* \defgroup group_syspm_functions Functions
* \{
*   \defgroup group_syspm_functions_power          Power Modes
*   \defgroup group_syspm_functions_callback   Low Power Callbacks
* \}
* \defgroup group_syspm_data_structures Data Structures
* \defgroup group_syspm_data_enumerates Enumerated Types
*/

#if !defined (CY_SYSPM_H)
#define CY_SYSPM_H

#include <stdbool.h>
#include <stddef.h>

#include "cy_device.h"
#include "cy_device_headers.h"
#include "cy_syslib.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
*       Register Constants
*******************************************************************************/

/**
* \addtogroup group_syspm_macros
* \{
*/

/** Driver major version */
#define CY_SYSPM_DRV_VERSION_MAJOR       1

/** Driver minor version */
#define CY_SYSPM_DRV_VERSION_MINOR       0

/** SysPm driver identifier */
#define CY_SYSPM_ID                      (CY_PDL_DRV_ID(0x10U))


/*******************************************************************************
*       Internal Defines
*******************************************************************************/

/** \cond INTERNAL */

/* Macro to validate parameters in Cy_SysPm_ExecuteCallback() function */
#define CY_SYSPM_IS_CALLBACK_TYPE_VALID(type)           (((type) == CY_SYSPM_SLEEP) || \
                                                         ((type) == CY_SYSPM_DEEPSLEEP))

/* Macro to validate parameters in Cy_SysPm_ExecuteCallback() function */
#define CY_SYSPM_IS_CALLBACK_MODE_VALID(mode)           (((mode) == CY_SYSPM_CHECK_READY) || \
                                                         ((mode) == CY_SYSPM_CHECK_FAIL) || \
                                                         ((mode) == CY_SYSPM_BEFORE_TRANSITION) || \
                                                         ((mode) == CY_SYSPM_AFTER_TRANSITION))

/* Macro to validate parameters in Cy_SysPm_CpuEnterSleep() and for Cy_SysPm_CpuEnterDeepSleep() function */
#define CY_SYSPM_IS_WAIT_FOR_VALID(waitFor)             (((waitFor) == CY_SYSPM_WAIT_FOR_INTERRUPT) || \
                                                         ((waitFor) == CY_SYSPM_WAIT_FOR_EVENT))

/** \endcond */

/** \} group_syspm_macros */

/*******************************************************************************
*       Configuration Structures
*******************************************************************************/

/**
* \addtogroup group_syspm_data_enumerates
* \{
*/

/** The SysPm function return value status definitions. */
typedef enum
{
    CY_SYSPM_SUCCESS         = 0x0U,                                         /**< Successful. */
    CY_SYSPM_BAD_PARAM       = CY_SYSPM_ID | CY_PDL_STATUS_ERROR | 0x01U,    /**< One or more invalid parameters. */
    CY_SYSPM_TIMEOUT         = CY_SYSPM_ID | CY_PDL_STATUS_ERROR | 0x02U,    /**< A time-out occurred. */
    CY_SYSPM_INVALID_STATE   = CY_SYSPM_ID | CY_PDL_STATUS_ERROR | 0x03U,    /**< The operation is not setup or is in an
                                                                                  improper state. */
    CY_SYSPM_CANCELED        = CY_SYSPM_ID | CY_PDL_STATUS_ERROR | 0x04U,    /**< Operation canceled. */
    CY_SYSPM_SYSCALL_PENDING = CY_SYSPM_ID | CY_PDL_STATUS_ERROR | 0x05U,    /**< Canceled due syscall operation pending. */
    CY_SYSPM_FAIL            = CY_SYSPM_ID | CY_PDL_STATUS_ERROR | 0xFFU     /**< Unknown failure. */
} cy_en_syspm_status_t;

/**
* This enumeration is used to initialize the functions wait action. The wait actions can be -
* an interrupt or an event. Refer to the CMSIS for WFE and WFI instruction explanations.
*/
typedef enum
{
    CY_SYSPM_WAIT_FOR_INTERRUPT,    /**< Wait for an interrupt. */
    CY_SYSPM_WAIT_FOR_EVENT         /**< Wait for an event. */
} cy_en_syspm_waitfor_t;

/**
* This enumeration is used to select the low power mode for which the
* appropriate registered callback handler will be executed. For example,
* the registered callback of the type CY_SYSPM_SLEEP will be executed while
* switching into the Sleep power mode.
*/
typedef enum
{
    CY_SYSPM_SLEEP      = 0U,   /**< The Sleep enum callback type */
    CY_SYSPM_DEEPSLEEP  = 1U,   /**< The Deep Sleep enum callback type */
    CY_SYSPM_HIBERNATE  = 2U,   /**< The Hibernate enum callback type */
    CY_SYSPM_STOP       = 3U    /**< The Stop enum callback type */

} cy_en_syspm_callback_type_t;

/** This enumeration specifies the associated callback mode. This enum defines the callback mode. */
typedef enum
{
    CY_SYSPM_CHECK_READY        = 0x01U,    /**< Callbacks with this mode are executed before entering into the
                                                 low power mode. The purpose of his callback function is to check
                                                 if the device is ready to enter the low power mode. */
    CY_SYSPM_CHECK_FAIL         = 0x02U,    /**< Callbacks with this mode are executed after the CY_SYSPM_CHECK_READY
                                                 callbacks execution returns CY_SYSPM_FAIL.
                                                 The callback with the CY_SYSPM_CHECK_FAIL mode should roll back the
                                                 actions performed in the previously executed callback with
                                                 CY_SYSPM_CHECK_READY */
    CY_SYSPM_BEFORE_TRANSITION  = 0x04U,    /**< Callbacks with this mode are executed after the CY_SYSPM_CHECK_READY
                                                 callbacks execution returns CY_SYSPM_SUCCESS.
                                                 Performs the actions to be done before entering into the
                                                 low power mode. */
    CY_SYSPM_AFTER_TRANSITION   = 0x08U     /**< Performs the actions to be done after exiting the low power mode
                                                 if entered. */
} cy_en_syspm_callback_mode_t;

/** \} group_syspm_data_enumerates */

/**
* \addtogroup group_syspm_macros
* \{
*/
/**
* \defgroup group_syspm_skip_callback_modes Defines to skip the callbacks modes
* \{
* Defines for the SysPm callbacks modes that can be skipped during execution.
* For more information about callbacks modes, refer
* to \ref cy_en_syspm_callback_mode_t.
*/
#define CY_SYSPM_SKIP_CHECK_READY                   (0x01U)   /**< Define to skip check ready mode in the syspm callback */
#define CY_SYSPM_SKIP_CHECK_FAIL                    (0x02U)   /**< Define to skip check fail mode in the syspm callback */
#define CY_SYSPM_SKIP_BEFORE_TRANSITION             (0x04U)   /**< Define to skip before transition mode in the syspm callback */
#define CY_SYSPM_SKIP_AFTER_TRANSITION              (0x08U)   /**< Define to skip after transition mode in the syspm callback */
/** \} group_syspm_skip_callback_modes */
/** \} group_syspm_macros */

/**
* \addtogroup group_syspm_data_structures
* \{
*/

/** The structure contains syspm callback parameters */
typedef struct
{
    void *base;         /**< Base address of a HW instance, matches name of the driver in
                             the API for the base address. Can be undefined if not required. Base address is not
                             required for the SysPm driver as the device has only one set of power modes */
    void *context;      /**< Context for the handler function. This item can be
                             skipped if not required. Can be undefined if not required. */

} cy_stc_syspm_callback_params_t;
/** \cond */
/** The type for syspm callbacks */
typedef cy_en_syspm_status_t (*Cy_SysPmCallback) (cy_stc_syspm_callback_params_t *callbackParams, cy_en_syspm_callback_mode_t mode);
/** \endcond */
/** Structure with syspm callback configuration elements */
typedef struct cy_stc_syspm_callback
{
    Cy_SysPmCallback callback;                         /**< The callback handler function. */
    cy_en_syspm_callback_type_t type;                  /**< The callback type, see \ref cy_en_syspm_callback_type_t. */
    uint32_t skipMode;                                 /**< The mask of modes to be skipped during callback
                                                            execution, see \ref group_syspm_skip_callback_modes. The
                                                            corresponding callback mode won't execute if the
                                                            appropriate define is set. These values can be ORed.
                                                            If all modes are required to be executed this element
                                                            should be equal to zero. Skipping unneeded callback modes speeds up
                                                            power mode transitions by skipping unneeded operations. */

    cy_stc_syspm_callback_params_t *callbackParams;    /**< The address of a cy_stc_syspm_callback_params_t,
                                                            the callback is executed with these parameters. */

    struct cy_stc_syspm_callback *prevItm;             /**< The previous list item. This element should not be
                                                            defined, or defined as NULL. It is for internal
                                                            usage to link this structure to the next registered
                                                            structure. It will be updated during callback
                                                            registration. Do not modify this element at run-time. */

    struct cy_stc_syspm_callback *nextItm;             /**< The next list item. This element should not be
                                                            defined, or defined as NULL. It is for internal usage to
                                                            link this structure to the previous registered structure.
                                                            It will be updated during callback registration. Do not
                                                            modify this element at run-time. */
    uint8_t order;                                     /**< Holds the callback execution order value. Range: 0-255.
                                                            While entering low power mode, callbacks with lower order values
                                                            are executed first. While exiting low power mode,
                                                            the callbacks are executed in the opposite order.
                                                            Callbacks with the same order value are executed in the
                                                            order they are registered in the application. */
} cy_stc_syspm_callback_t;

/** \} group_syspm_data_structures */

/**
* \addtogroup group_syspm_functions
* \{
*/

/**
* \addtogroup group_syspm_functions_power
* \{
*/
cy_en_syspm_status_t Cy_SysPm_SystemEnterDeepSleep(cy_en_syspm_waitfor_t waitFor);
cy_en_syspm_status_t Cy_SysPm_SystemEnterHibernate(void);
/** \cond INTERNAL */
cy_en_syspm_status_t Cy_SysPm_CpuEnterSleep(cy_en_syspm_waitfor_t waitFor);

void Cy_SysPm_CpuSleepOnExit(bool enable);

__STATIC_INLINE void Cy_SysPm_CpuSendWakeupEvent(void);
/** \endcond INTERNAL */
/** \} group_syspm_functions_power */


/**
* \addtogroup group_syspm_functions_callback
* \{
*/
bool Cy_SysPm_RegisterCallback(cy_stc_syspm_callback_t *handler);
/** \cond */
bool Cy_SysPm_UnregisterCallback(cy_stc_syspm_callback_t const *handler);
cy_en_syspm_status_t Cy_SysPm_ExecuteCallback(cy_en_syspm_callback_type_t type, cy_en_syspm_callback_mode_t mode);
cy_stc_syspm_callback_t* Cy_SysPm_GetFailedCallback(cy_en_syspm_callback_type_t type);
/** \endcond */
/** \} group_syspm_functions_callback */

/** \cond INTERNAL */

/**
* \addtogroup group_syspm_functions_power
* \{
*/
/*******************************************************************************
* Function Name: Cy_SysPm_CpuSendWakeupEvent
****************************************************************************//**
*
* Sends the SEV (Send Event) ARM instruction to the system.
*
* \funcusage
* \snippet syspm/snippet/main.c snippet_Cy_SysPm_CpuSendWakeupEvent
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysPm_CpuSendWakeupEvent(void)
{
    __SEV();
}
/** \endcond INTERNAL */
/** \} group_syspm_functions_power */
/** \} group_syspm_functions*/


#ifdef __cplusplus
}
#endif

#endif /* CY_SYSPM_H */
/** \} group_syspm */


/* [] END OF FILE */
