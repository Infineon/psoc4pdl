/*******************************************************************************
* \file cy_trigmux.h
* \version 1.0
*
*  This file provides constants and parameter values for the Trigger multiplexer driver.
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

/**
* \addtogroup group_trigmux
* \{
* The trigger multiplexer provides access to the multiplexer that selects a set
* of trigger output signals from different peripheral blocks to route them to the
* specific trigger input of another peripheral block.
*
* The functions and other declarations used in this driver are in cy_trigmux.h.
* You can include cy_pdl.h (ModusToolbox only) to get access to all functions
* and declarations in the PDL.
*
* The TrigMux driver is based on the trigger multiplexer's hardware block.
* The Trigger multiplexer block consists of multiple trigger multiplexers.
* These trigger multiplexers are grouped in trigger groups. All the trigger
* multiplexers in the trigger group share similar input options.
*
* The trigger multiplexer groups are either reduction multiplexers or distribution
* multiplexers. The figure below illustrates a generic trigger multiplexer block
* implementation with a reduction multiplexer layer of N trigger groups and a
* distribution multiplexer layer of M trigger groups.
* The reduction multiplexer groups have input options that are the trigger outputs
* coming from the different peripheral blocks and the reduction multiplexer groups
* route them to intermediate signals. The distribution multiplexer groups have input
* options from these intermediate signals and route them back to multiple peripheral
* blocks as their trigger inputs.
*
* The trigger architecture of the PSoC device is explained in the technical reference
* manual (TRM). Refer to the TRM to better understand the trigger multiplexer routing
* architecture available.
*
* \section group_trigmux_section_Configuration_Considerations Configuration Considerations
*
* To route a trigger signal from one peripheral in the PSoC
* to another, the user must configure a reduction multiplexer and a distribution
* multiplexer. The \ref Cy_TrigMux_Connect is used to configure a trigger multiplexer connection.
* The user will need two calls of this API, one for the reduction multiplexer and another
* for the distribution multiplexer, to achieve the trigger connection from a source
* peripheral to a destination peripheral.
*
* The Cy_TrigMux_Connect() function has two main parameters, inTrig and outTrig that
* refer to the input and output trigger lines connected using the multiplexer.
* These parameters are represented in the following format:<br>
* In addition, the \ref Cy_TrigMux_Connect function also has an invert and trigger type parameter.
* Refer to the API reference for a detailed description of this parameter.
* All the constants associated with the different trigger signals in the system
* (input and output) are defined as constants in the device configuration header file.
*
* The constants for TrigMux in the device configuration header file are divided into four
* types based on the signal being input/output and being part of a reduction/distribution
* trigger multiplexer.
*
* The four types of the input/output parameters are:
* 1) The parameters for the reduction multiplexer's inputs (input signals of TrigMux);
* 2) The parameters for the reduction multiplexer's outputs (intermediate signals);
* 3) The parameters for the distribution multiplexer's inputs (intermediate signals);
* 4) The parameters for the distribution multiplexer's outputs (output signals of TrigMux).
*
* Refer to the TRM for a more detailed description of this architecture and different options.
*
* The steps to connect one peripheral block to the other:
*
* Step 1. Find the trigger group number in the Trigger Group Inputs section of the device
* configuration header file that corresponds to the output of the first peripheral block.
* For example, TRIG11_IN_TCPWM0_TR_OVERFLOW0 input of the reduction multiplexers belongs
* to Trigger Group 11.
*
* Step 2. Find the trigger group number in the Trigger Group Outputs section of the device
* configuration header file that corresponds to the input of the second peripheral block.
* For example, TRIG0_OUT_CPUSS_DW0_TR_IN0 output of the distribution multiplexer belongs to
* Trigger Group 0.
*
* Step 3. Find the same trigger group number in the Trigger Group Inputs section of the
* device configuration header file that corresponds to the trigger group number found in
* Step 1. Select the reduction multiplexer output that can be connected to the trigger group
* found in Step 2. For example, TRIG0_IN_TR_GROUP11_OUTPUT0 means that Reduction Multiplexer
* For example, TRIG0_IN_TR_GROUP11_OUTPUT0 means that Reduction Multiplexer
* Output 15 of Trigger Group 13 can be connected to Trigger Group 0.
*
* Step 4. Find the same trigger group number in the Trigger Group Outputs section of the
* device configuration header file that corresponds to the trigger group number found in Step 2.
* Select the distribution multiplexer input that can be connected to the trigger group found
* in Step 1. For example, TRIG11_OUT_TR_GROUP0_INPUT9 means that the Distribution Multiplexer
* Input 42 of Trigger Group 0 can be connected to the output of the reduction multiplexer
* in Trigger Group 13 found in Step 3.
*
* Step 5. Call Cy_TrigMux_Connect() API twice: the first call - with the constants for the
* inTrig and outTrig parameters found in Steps 1 and Step 4, the second call - with the
* constants for the inTrig and outTrig parameters found in Steps 2 and Step 3.
* For example,
* Cy_TrigMux_Connect(TRIG11_IN_TCPWM0_TR_OVERFLOW0, TRIG11_OUT_TR_GROUP0_INPUT9,
* false, TRIGGER_TYPE_LEVEL);
* Cy_TrigMux_Connect(TRIG0_IN_TR_GROUP11_OUTPUT0, TRIG0_OUT_CPUSS_DW0_TR_IN0,
* false, TRIGGER_TYPE_EDGE);
*
* \section group_trigmux_more_information More Information
* For more information on the TrigMux peripheral, refer to the technical reference manual (TRM).
*
* \if MISRA
* \section group_trigmux_MISRA MISRA-C Compliance
* The TrigMux driver has the following specific deviations:
*
* <table class="doxtable">
*   <tr>
*     <th>MISRA Rule</th>
*     <th>Rule Class (Required/Advisory)</th>
*     <th>Rule Description</th>
*     <th>Description of Deviation(s)</th>
*   </tr>
*   <tr>
*     <td>14.4</td>
*     <td>R</td>
*     <td>The controlling expression of an if statement and the controlling
*         expression of an iteration-statement shall have essentially Boolean
*         type.</td>
*     <td>IGNORE: Tools issue. Manual inspection does not see any violations.</td>
*   </tr>
* </table>
* \endif
*
* \section group_trigmux_Changelog Changelog
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
*   <tr>
*     <td>1.0</td>
*     <td>Initial version</td>
*     <td></td>
*   </tr>
* </table>
*
* \defgroup group_trigmux_macros Macros
* \defgroup group_trigmux_functions Functions
* \defgroup group_trigmux_enums Enumerated Types
*/

#if !defined(CY_TRIGMUX_H)
#define CY_TRIGMUX_H


#include "cy_syslib.h"


#if defined(__cplusplus)
extern "C" {
#endif

/******************************************************************************
 * Macros
 *****************************************************************************/

/**
* \addtogroup group_trigmux_macros
* \{
*/

/** The driver major version */
#define CY_TRIGMUX_DRV_VERSION_MAJOR       1

/** The driver minor version */
#define CY_TRIGMUX_DRV_VERSION_MINOR       0

/** TRIGMUX PDL ID */
#define CY_TRIGMUX_ID                       CY_PDL_DRV_ID(0x33UL) /**< The trigger multiplexer driver identifier */

/** Values for the cycles parameter in the \ref Cy_TrigMux_SwTrigger() function */
#define CY_TRIGGER_INFINITE                 (255UL) /**< The trigger will be active until the user clears it or a hardware deactivates it. */
#define CY_TRIGGER_DEACTIVATE               (0UL)   /**< Use this parameter value to deactivate the trigger. */
#define CY_TRIGGER_TWO_CYCLES               (2UL)   /**< The only valid cycles number value for PERI_ver2. */

/** \} group_trigmux_macros */

/** \cond BWC macros */
#define CY_TR_MUX_TR_INV_ENABLE            (0x01u)
#define CY_TR_MUX_TR_INV_DISABLE           (0x00u)
#define CY_TR_ACTIVATE_DISABLE             (0x00u)
#define CY_TR_ACTIVATE_ENABLE              (0x01u)
#define CY_TR_GROUP_MASK                   (0x0F00u)
#define CY_TR_MASK                         (0x007Fu)
#define CY_TR_GROUP_SHIFT                  (0x08u)
#define CY_TR_OUT_CTL_MASK                 (0x40000000uL)
#define CY_TR_OUT_CTL_SHIFT                (30u)
#define CY_TR_PARAM_MASK                   (CY_TR_OUT_CTL_MASK | CY_TR_GROUP_MASK | CY_TR_MASK)
#define CY_TR_CYCLES_MIN                   (0u)
#define CY_TR_CYCLES_MAX                   (255u)
/** \endcond */


/**
* \addtogroup group_trigmux_enums
* \{
*/

/******************************************************************************
 * Enumerations
 *****************************************************************************/

/** The TRIGMUX error codes. */
typedef enum
{
    CY_TRIGMUX_SUCCESS = 0x0UL,                                             /**< Successful */
    CY_TRIGMUX_BAD_PARAM = CY_TRIGMUX_ID | CY_PDL_STATUS_ERROR | 0x1UL,     /**< One or more invalid parameters */
    CY_TRIGMUX_INVALID_STATE = CY_TRIGMUX_ID | CY_PDL_STATUS_ERROR | 0x2UL  /**< Operation not set up or is in an improper state */
} cy_en_trigmux_status_t;

/** \} group_trigmux_enums */

/**
* \addtogroup group_trigmux_functions
* \{
*/

cy_en_trigmux_status_t Cy_TrigMux_Connect(uint32_t inTrig, uint32_t outTrig, bool invert, en_trig_type_t trigType);
cy_en_trigmux_status_t Cy_TrigMux_SwTrigger(uint32_t trigLine, uint32_t cycles);

/** \} group_trigmux_functions */

#if defined(__cplusplus)
}
#endif

#endif /* CY_TRIGMUX_H */

/** \} group_trigmux */

/* [] END OF FILE */
