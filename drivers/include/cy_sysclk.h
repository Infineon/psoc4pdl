/***************************************************************************//**
* \file cy_sysclk.h
* \version 1.0
*
* Provides an API declaration of the sysclk driver.
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
* \addtogroup group_sysclk
* \{
* The System Clock (SysClk) driver contains the API for configuring system and
* peripheral clocks.
*
* The functions and other declarations used in this driver are in cy_sysclk.h.
* You can include cy_pdl.h (ModusToolbox only) to get access to all functions
* and declarations in the PDL.
*
* Firmware uses the API to configure , enable, or disable a clock.
*
* The clock system includes a variety of resources that can vary per device, including:
* - Internal clock sources such as internal oscillators
* - External clock sources such as crystal oscillators or a signal on an I/O pin
* - Generated clocks such as a PLL, System clock and peripheral clocks
*
* Consult the Technical Reference Manual for your device for details of the
* clock system.
*
* \section group_sysclk_configuration Configuration Considerations
* The availability of clock functions depend on the availability of the chip
* resources that support those functions. Consult the device TRM before
* attempting to use these functions.
*
* Low power modes may limit the maximum clock frequency.
* Refer to the SysPm driver and the TRM for details.
*
* \section group_sysclk_more_information More Information
* Refer to the technical reference manual (TRM) and the device datasheet.
*
* \section group_sysclk_MISRA MISRA-C Compliance
*
* The SysClk driver has the following specific deviations:
*
* <table class="doxtable">
*   <tr>
*     <th>MISRA Rule</th>
*     <th>Rule Class (Required/Advisory)</th>
*     <th>Rule Description</th>
*     <th>Description of Deviation(s)</th>
*   </tr>
*   <tr>
*     <td>10.4</td>
*     <td>R</td>
*     <td>Essential type of the left hand operand dividerType (enum) is not the
*         same as that of the right operand CY_SYSCLK_DIV_16_BIT(boolean).</td>
*     <td>The \ref CY_SYSCLK_DIV_16_BIT which is considered as a boolean, is
*         actually exactly an enum of type \ref cy_en_sysclk_divider_types_t.</td>
*   </tr>
*   <tr>
*     <td>10.5</td>
*     <td>A</td>
*     <td>The value of an expression should not be cast to an inappropriate
*         essential type.</td>
*     <td>This code is safe because value is checked before conversion to the
*         enumeration.</td>
*   </tr>
*   <tr>
*     <td>10.8</td>
*     <td>R</td>
*     <td>The value of a composite expression shall not be cast to a different
*         essential type category or a wider essential type.</td>
*     <td>This code is safe because value is checked before conversion to the
*         enumeration.</td>
*   </tr>
*   <tr>
*     <td>13.5</td>
*     <td>R</td>
*     <td>The right hand operand of a logical && or || operator shall not
*         contain persistent side effects.</td>
*     <td>Manually check that side effect has no impact. It will be addressed in
*         final release.</td>
*   </tr>
* \if MISRA
*   <tr>
*     <td>14.4</td>
*     <td>R</td>
*     <td>The controlling expression of an if statement and the controlling
*         expression of an iteration-statement shall have essentially Boolean
*         type.</td>
*     <td>IGNORE: Tools issue. Manual inspection does not see any violations.</td>
*   </tr>
* \endif
* </table>
*
* \section group_sysclk_changelog Changelog
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
*   <tr>
*     <td>1.0</td>
*     <td>Initial version</td>
*     <td></td>
*   </tr>
* </table>
*
* \defgroup group_sysclk_macros           Macros
* \{
* \}
* \defgroup group_sysclk_enums           General Enumerated Types
* \{
*   \defgroup group_sysclk_returns         Function return values
* \}
* \defgroup group_sysclk_imo             Internal Main Oscillator (IMO)
* \{
*   The Internal Main Oscillator (IMO) is a primary clock source for
*   the CPU core and most of the peripherals. It is enabled by default
*   and is not recommended to disable without a critical need.
*
*   \defgroup group_sysclk_imo_funcs       Functions
*   \defgroup group_sysclk_imo_enums       Enumerated Types
* \}
* \defgroup group_sysclk_ext             External Clock Source (EXTCLK)
* \{
*   The External Clock Source (EXTCLK) is a clock source routed into PSoC
*   through a GPIO pin. The EXTCLK is a source clock that can be used to
*   source the processors and peripherals \ref group_sysclk_clk_hf.
*
*   The EXTCLK relies on the presence of an external clock signal applied
*   to the GPIO pin. The pin must be configured to operate in Digital
*   High-Z drive mode with input buffer on and HSIOM connection
*   set to HSIOM_SEL_ACT_0 (P0_6_SRSS_EXT_CLK).
*
*   \defgroup group_sysclk_ext_funcs       Functions
* \}
* \defgroup group_sysclk_eco             External Crystal Oscillator (ECO)
* \{
*   The External Crystal Oscillator (ECO) is a clock source that consists
*   of an oscillator circuit that drives an external crystal through its
*   dedicated ECO pins. The ECO is a source clock that can be used to
*   source one or more clock paths (Refer to \ref group_sysclk_clk_hf).
*   These clock paths can then source the processors and peripherals in
*   the device.
*
*   The ECO relies on the presence of an external crystal. The pins
*   connected to this crystal must be configured to operate in analog
*   drive mode with HSIOM connection set to GPIO control (HSIOM_SEL_GPIO).
*
*   \defgroup group_sysclk_eco_funcs       Functions
*   \defgroup group_sysclk_eco_enums       Enumerated Types
* \}
* \defgroup group_sysclk_pll             Phase Locked Loop (PLL)
* \{
*   The PLL is a clock generation circuit that can be used to produce a
*   higher frequency clock from a reference clock. The output clock exhibits
*   characteristics of the reference clock such as the accuracy of the source
*   and its phase. The PLL is similar in purpose to a (Frequency locked loop) FLL
*   but they are not equivalent.
*
*   - They may have different frequency ranges.
*   - The PLL starts up more slowly and consumes more current than the FLL.
*   - The PLL requires a higher frequency source clock than PLL.
*   ![](sysclk_pll.png)
*
*   The SysClk driver supports two models for configuring the PLL. The first
*   model is to call the Cy_SysClk_PllConfigure() function, which calculates the
*   necessary parameters for the PLL at run-time. This may be necessary for dynamic
*   run-time changes to the PLL. However this method is slow as it needs to perform
*   the calculation before configuring the PLL. The other model is to call
*   Cy_SysClk_PllManualConfigure() function with pre-calculated parameter values.
*   This method is faster but requires prior knowledge of the necessary parameters.
*   Consult the device TRM for the PLL calculation equations.
*
*   \defgroup group_sysclk_pll_funcs       Functions
*   \defgroup group_sysclk_pll_structs     Data Structures
*   \defgroup group_sysclk_pll_enums       Enumerated Types
* \}
* \defgroup group_sysclk_ilo             Internal Low-Speed Oscillator (ILO)
* \{
*   The ILO operates with no external components and outputs a clock signal at roughly
*   40 kHz. The ILO is relatively low power and low accuracy. It is available
*   in all power modes and can be used as a source for the WDT \ref group_wdt.
*
*   Additionally, the ILO clock can be trimmed using
*   a higher precision clock source. Use the \ref group_sysclk_ilo_funcs API
*   to measure the current ILO frequency and trim it.
*
*   \defgroup group_sysclk_ilo_funcs       Functions
* \}
* \defgroup group_sysclk_pm              Low Power Callback
* \{
*   Entering and exiting low power modes require compatible clock configurations
*   to be set before entering low power and restored upon wake-up and exit. The
*   SysClk driver provides a Cy_SysClk_DeepSleepCallback() function to support
*   Deep Sleep mode entry.
*
*   This function can be called either by itself before initiating low-power mode
*   entry or it can be used in conjunction with the SysPm driver as a registered
*   callback. To do so, register this function as a callback before calling
*   Cy_SysPm_DeepSleep(). Specify \ref CY_SYSPM_DEEPSLEEP as the callback type,
*   and call Cy_SysPm_RegisterCallback().
*
*   \note If the FLL or PLL source is the ECO, this function must be called.
*
*   \defgroup group_sysclk_pm_funcs        Functions
* \}
* \defgroup group_sysclk_wco             Watch Crystal Oscillator (WCO)
* \{
*   \note Not supported in Alpha release.
*
*   The WCO is a highly accurate 32.768 kHz clock source capable of operating
*   in all power modes (excluding the Off mode). It is the primary clock source for
*   the backup domain clock, which is used by the real-time clock (RTC). The
*   WCO can also be used as a source for the low-frequency clock to support other
*   low power mode peripherals.
*
*   ![](sysclk_backup.png)
*
*   The WCO requires the configuration of the dedicated WCO pins (SRSS_WCO_IN_PIN,
*   SRSS_WCO_OUT_PIN). These must be configured as Analog Hi-Z drive modes and the
*   HSIOM selection set to GPIO. The WCO can also be used in bypass mode, where
*   an external 32.768 kHz square wave is brought in directly through the
*   SRSS_WCO_OUT_PIN pin.
*
*   \defgroup group_sysclk_wco_funcs       Functions
* \}
* \defgroup group_sysclk_clk_hf          High-Frequency Clocks
* \{
*   The high frequency clock ClkHf is a source for the system clock for CPU and
*   the peropheral clock didviders.
*   High frequency clock can be sourced by IMO, PLL, ECO, EXTCLK, etc.
*   Consult the Technical Reference Manual for your device for details.
*
*   \defgroup group_sysclk_clk_hf_funcs    Functions
*   \defgroup group_sysclk_clk_hf_enums    Enumerated Types
* \}
* \defgroup group_sysclk_clk_peripheral  Peripherals Clock Dividers
* \{
*   There are multiple peripheral clock dividers that, in effect, create
*   multiple separate peripheral clocks:
*
*   - Integer 8-bit dividers
*   - Integer 16-bit dividers
*   - Fractional 16.5-bit dividers (16 integer bits, 5 fractional bits)
*   - Fractional 24.5-bit divider (24 integer bits, 5 fractional bits)
*
*   The availability and number of each type of dividers depends on teh certain device family.
*   Consult the Technical Reference Manual for your device for details.
*
*   The integer dividers work simply: a divider value of 1
*   means the output frequency matches the input frequency (that is, there is
*   no change). Otherwise the frequency is divided by the value of the divider.
*   For example, if the input frequency is 50 MHz, and the divider is value 10,
*   the output frequency is 5 MHz.
*
*   The five fractional bits supports further precision in 1/32nd increments. For
*   example, a divider with an integer value of 3 and a fractional value of
*   4 (4/32) results in a divider of 3.125. Fractional dividers are useful when
*   a high-precision clock is required, for example, for a UART/SPI serial
*   interface.
*
*   ![](sysclk_peri_divs.png)
*
*   Each peripheral can connect to any one of the programmable dividers. A
*   particular peripheral clock divider can drive multiple peripherals.
*
*   The SysClk driver also supports phase aligning two peripheral clock dividers using
*   Cy_SysClk_PeriphEnablePhaseAlignDivider(). Alignment works for both integer
*   and fractional dividers. The divider to which a second divider is aligned
*   must already be enabled.
*
*   \defgroup group_sysclk_clk_peripheral_funcs Functions
*   \defgroup group_sysclk_clk_peripheral_enums Enumerated Types
* \}
* \defgroup group_sysclk_clk_sys        System Clock
* \{
*   The system clock is the source clock for CPU core (Cortex-M0+ in PSoC 4).
*   This clock is a divided version of the \ref group_sysclk_clk_hf. A divider
*   value of 1, 2, 4 or 8 can be used to further divide the High Frequency
*   clock to a desired clock speed for the processor.
*
*   \defgroup group_sysclk_clk_sys_funcs  Functions
* \}
* \defgroup group_sysclk_clk_pump        Pump Clock
* \{
*   The pump clock is a clock source used to provide analog precision in low voltage
*   applications. Depending on the usage scenario, it may be required to drive the
*   internal voltage pump for the Continuous Time Block mini (CTBm) in the analog
*   subsystem.
*
*   \defgroup group_sysclk_clk_pump_funcs  Functions
*   \defgroup group_sysclk_clk_pump_enums  Enumerated Types
* \}
*/

#if !defined(CY_SYSCLK_H)
#define CY_SYSCLK_H

#include <stdbool.h>
#include "cy_device.h"
#include "cy_device_headers.h"
#include "cy_syslib.h"
#include "cy_syspm.h"
#include "cy_wdt.h"


#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/**
* \addtogroup group_sysclk_macros
* \{
*/
/** Driver major version */
#define  CY_SYSCLK_DRV_VERSION_MAJOR   1
/** Driver minor version */
#define  CY_SYSCLK_DRV_VERSION_MINOR   0
/** Sysclk driver identifier */
#define CY_SYSCLK_ID   CY_PDL_DRV_ID(0x12U)

/** ILO clock frequency */
#define CY_SYSCLK_ILO_FREQ  (40000UL)   /* Hz */
/** WCO clock frequency */
#define CY_SYSCLK_WCO_FREQ  (32768UL)   /* Hz */
/** \} group_sysclk_macros */

/**
* \addtogroup group_sysclk_returns
* \{
*/
/** Defines general-purpose function return values */
typedef enum
{
    CY_SYSCLK_SUCCESS       = 0x00UL, /**< Command completed with no errors */
    CY_SYSCLK_BAD_PARAM     = (CY_SYSCLK_ID | CY_PDL_STATUS_ERROR | 0x01UL), /**< Invalid function input parameter */
    CY_SYSCLK_TIMEOUT       = (CY_SYSCLK_ID | CY_PDL_STATUS_ERROR | 0x02UL), /**< Timeout occurred */
    CY_SYSCLK_INVALID_STATE = (CY_SYSCLK_ID | CY_PDL_STATUS_ERROR | 0x03UL), /**< Clock is in an invalid state */
    CY_SYSCLK_STARTED       = (CY_SYSCLK_ID | CY_PDL_STATUS_WARNING | 0x04UL)  /**< Ilo measurement is running */
} cy_en_sysclk_status_t;
/** \} group_sysclk_returns */


/* ========================================================================== */
/* ===========================    EXT SECTION    ============================ */
/* ========================================================================== */

/**
* \addtogroup group_sysclk_ext_funcs
* \{
*/
    void Cy_SysClk_ExtClkSetFrequency(uint32_t freq);
uint32_t Cy_SysClk_ExtClkGetFrequency(void);
/** \} group_sysclk_ext_funcs */


/* ========================================================================== */
/* ===========================    IMO SECTION    ============================ */
/* ========================================================================== */

/**
* \addtogroup group_sysclk_imo_enums
* \{
*/

/** IMO frequencies */
typedef enum
{
    CY_SYSCLK_IMO_24MHZ = 24000000UL, /**< 24 MHz */
    CY_SYSCLK_IMO_28MHZ = 28000000UL, /**< 28 MHz */
    CY_SYSCLK_IMO_32MHZ = 32000000UL, /**< 32 MHz */
    CY_SYSCLK_IMO_36MHZ = 36000000UL, /**< 36 MHz */
    CY_SYSCLK_IMO_40MHZ = 40000000UL, /**< 40 MHz */
    CY_SYSCLK_IMO_44MHZ = 44000000UL, /**< 44 MHz */
    CY_SYSCLK_IMO_48MHZ = 48000000UL  /**< 48 MHz */
} cy_en_sysclk_imo_freq_t;

/** IMO lock sources */
typedef enum
{
    CY_SYSCLK_IMO_LOCK_NONE     =     0U, /**< Disable IMO Locking */
    CY_SYSCLK_IMO_LOCK_WCO      =     1U, /**< Lock IMO by WCO clock */
} cy_en_sysclk_imo_lock_t;
/** \} group_sysclk_imo_enums */

/**
* \addtogroup group_sysclk_imo_funcs
* \{
*/

  cy_en_sysclk_status_t Cy_SysClk_ImoSetFrequency(cy_en_sysclk_imo_freq_t freq);
               uint32_t Cy_SysClk_ImoGetFrequency(void);
  cy_en_sysclk_status_t Cy_SysClk_ImoLock(cy_en_sysclk_imo_lock_t lock);
cy_en_sysclk_imo_lock_t Cy_SysClk_ImoGetLockStatus(void);
   __STATIC_INLINE void Cy_SysClk_ImoEnable(void);
   __STATIC_INLINE void Cy_SysClk_ImoDisable(void);
   __STATIC_INLINE bool Cy_SysClk_ImoIsEnabled(void);

/** \} group_sysclk_imo_funcs */


/*******************************************************************************
* Function Name: Cy_SysClk_ImoEnable
****************************************************************************//**
*
* Enables IMO.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_ImoEnable(void)
{
   SRSSLT_CLK_IMO_CONFIG = SRSSLT_CLK_IMO_CONFIG_ENABLE_Msk;
}

/*******************************************************************************
* Function Name: Cy_SysClk_ImoDisable
****************************************************************************//**
*
* Returns IMO enable/disable state.
*
*******************************************************************************/
__STATIC_INLINE bool Cy_SysClk_ImoIsEnabled(void)
{
   return(_FLD2BOOL(SRSSLT_CLK_IMO_CONFIG_ENABLE, SRSSLT_CLK_IMO_CONFIG));
}

/*******************************************************************************
* Function Name: Cy_SysClk_ImoDisable
****************************************************************************//**
*
* Disables IMO.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_ImoDisable(void)
{
    (void) Cy_SysClk_ImoLock(CY_SYSCLK_IMO_LOCK_NONE);

    SRSSLT_CLK_IMO_CONFIG = 0UL;
}
/** \} group_sysclk_imo_funcs */



#if defined (CY_IP_M0S8EXCO) || defined(CY_DOXYGEN)
/* ========================================================================== */
/* ===========================    ECO SECTION    ============================ */
/* ========================================================================== */

/**
* \addtogroup group_sysclk_eco_enums
* \{
*/

/** Defines IMO function return values */
typedef enum
{
    CY_SYSCLK_ECO_STABLE         = 0U, /**< ECO is stable */
    CY_SYSCLK_ECO_WATCHDOG_ERROR = 1U, /**< ECO oscillator is stuck */
} cy_en_sysclk_eco_stat_t;
/** \} group_sysclk_eco_enums */

/** \cond */
#define SRSS_CLK_ECO_STATUS_Msk (SRSS_CLK_ECO_STATUS_ECO_OK_Msk | SRSS_CLK_ECO_STATUS_ECO_READY_Msk)
/** \endcond */



/**
* \addtogroup group_sysclk_eco_funcs
* \{
*/
cy_en_sysclk_status_t Cy_SysClk_EcoConfigure(uint32_t freq, uint32_t cLoad, uint32_t esr, uint32_t driveLevel);
cy_en_sysclk_status_t Cy_SysClk_EcoEnable(uint32_t timeoutUs);
             uint32_t Cy_SysClk_EcoGetFrequency(void);
 __STATIC_INLINE bool Cy_SysClk_EcoIsEnabled(void);
 __STATIC_INLINE void Cy_SysClk_EcoDisable(void);
 __STATIC_INLINE cy_en_sysclk_eco_stat_t Cy_SysClk_EcoGetStatus(void);


/*******************************************************************************
* Function Name: Cy_SysClk_EcoIsEnabled
****************************************************************************//**
*
* Returns the ECO enable/disable state.
*
* \return
* false = disabled \n
* true = enabled
*
* \funcusage
* \snippet sysclk/snippet/main.c snippet_Cy_SysClk_EcoDisable
*
*******************************************************************************/
__STATIC_INLINE bool Cy_SysClk_EcoIsEnabled(void)
{
    return(_FLD2BOOL(EXCO_ECO_CONFIG_ENABLE, EXCO_ECO_CONFIG));
}


/*******************************************************************************
* Function Name: Cy_SysClk_EcoDisable
****************************************************************************//**
*
* Disables the external crystal oscillator (ECO). This function should not be
* called if the ECO is sourcing other resources.
*
* \funcusage
* \snippet sysclk/snippet/main.c snippet_Cy_SysClk_EcoDisable
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_EcoDisable(void)
{
    EXCO_ECO_CONFIG &= ~(EXCO_ECO_CONFIG_ENABLE_Msk | EXCO_ECO_CONFIG_CLK_EN_Msk);
}


/*******************************************************************************
* Function Name: Cy_SysClk_EcoGetStatus
****************************************************************************//**
*
* Reports the current status of the external crystal oscillator (ECO).
*
* \return \ref cy_en_sysclk_eco_stat_t
*
* \funcusage
* \snippet sysclk/snippet/main.c snippet_Cy_SysClk_EcoConfigure
*
*******************************************************************************/
__STATIC_INLINE cy_en_sysclk_eco_stat_t Cy_SysClk_EcoGetStatus(void)
{
    return ((cy_en_sysclk_eco_stat_t) _FLD2VAL(EXCO_ECO_STATUS_WATCHDOG_ERROR, EXCO_ECO_STATUS));
}
/** \} group_sysclk_eco_funcs */


#if defined(EXCO_PLL_PRESENT) || defined(CY_DOXYGEN)
/* ========================================================================== */
/* ===========================    PLL SECTION    ============================ */
/* ========================================================================== */
#if defined (EXCO_PLL_REF_IN_EN) || defined(CY_DOXYGEN)
/**
* \addtogroup group_sysclk_pll_enums
* \{
*/

/**
* The PLL input clock source selection.
* Used with functions \ref Cy_SysClk_PllSetSource and \ref Cy_SysClk_PllGetSource.
*/
typedef enum
{
    CY_SYSCLK_PLL_SRC_ECO = 0U, /**< PLL runs from ECO */
    CY_SYSCLK_PLL_SRC_IMO = 1U, /**< PLL runs from IMO */
} cy_en_sysclk_pll_src_t;

/** \} group_sysclk_pll_enums */



/**
* \addtogroup group_sysclk_pll_funcs
* \{
*/

                cy_en_sysclk_status_t  Cy_SysClk_PllSetSource(uint32_t pllNum, cy_en_sysclk_pll_src_t source);
__STATIC_INLINE cy_en_sysclk_pll_src_t Cy_SysClk_PllGetSource(uint32_t pllNum);


/*******************************************************************************
* Function Name: Cy_SysClk_PllGetSource
****************************************************************************//**
*
* Returns the clock source of the specified PLL.
*
* \param pllNum the number of PLL instance, starting from 0.
* If there is only one PLL in device - the 0 is the only valid number.
* For other pllNum values the default CY_SYSCLK_PLL_SRC_ECO value is returned.
*
* \funcusage
* \snippet sysclk/snippet/main.c snippet_Cy_SysClk_PllConfigure
*
*******************************************************************************/
__STATIC_INLINE cy_en_sysclk_pll_src_t Cy_SysClk_PllGetSource(uint32_t pllNum)
{
    cy_en_sysclk_pll_src_t retVal = CY_SYSCLK_PLL_SRC_ECO;
    CY_ASSERT_L1(0UL == pllNum);
    if (0UL == pllNum)
    {
        retVal = (cy_en_sysclk_pll_src_t)_FLD2VAL(EXCO_CLK_SELECT_REF_SEL, EXCO_CLK_SELECT);
    } /*  Else return the default value */
    return (retVal);
}


/** \} group_sysclk_pll_funcs */
#endif /* EXCO_PLL_REF_IN_EN */


/**
* \addtogroup group_sysclk_pll_enums
* \{
*/

/** PLL bypass mode. See register CLK_PLL_CONFIG, bits BYPASS_SEL. */
typedef enum
{
    CY_SYSCLK_PLL_OUTPUT_AUTO   = 0U, /**< Output PLL input source when not locked, and PLL output when locked */
    CY_SYSCLK_PLL_OUTPUT_AUTO1  = 1U, /**< Same as AUTO */
    CY_SYSCLK_PLL_OUTPUT_INPUT  = 2U, /**< Output PLL input source regardless of lock status */
    CY_SYSCLK_PLL_OUTPUT_OUTPUT = 3U  /**< Output PLL output regardless of lock status. This can be dangerous, because PLL output may be unstable */
} cy_en_sysclk_pll_bypass_t;

/** \} group_sysclk_pll_enums */

/**
* \addtogroup group_sysclk_pll_structs
* \{
*/
/** Structure containing information for configuration of a PLL. */
typedef struct
{
    uint32_t                       inputFreq;  /**< Frequency of the PLL source clock, in Hz.
                                                 *  This value can be left 0, then the \ref Cy_SysClk_PllConfigure
                                                 *  will get the source frequency automatically.
                                                 *  The desired source clock should be already enabled and selected
                                                 *  using \ref Cy_SysClk_PllSetSource.
                                                 */
    uint32_t                       outputFreq; /**< Frequency of the PLL output, in Hz. */
} cy_stc_sysclk_pll_config_t;

/** Structure containing information for manual configuration of a PLL. */
typedef struct
{
    uint8_t                        feedbackDiv;  /**< EXCO_PLL_CONFIG.FEEDBACK_DIV (P) bits */
    uint8_t                        referenceDiv; /**< EXCO_PLL_CONFIG.REFERENCE_DIV (Q) bits */
    uint8_t                        outputDiv;    /**< EXCO_PLL_CONFIG.OUTPUT_DIV bits */
    uint8_t                        icp;          /**< EXCO_PLL_CONFIG.ICP_SEL bits, usually CY_SYSCLK_IS_PLL_ICP_LT67MHZ value is recommended */
} cy_stc_sysclk_pll_manual_config_t;
/** \} group_sysclk_pll_structs */

/** \cond */
#define CY_SYSCLK_IS_PLL_ICP_LT67MHZ (2U) /* PLL Fout <= 67MHz*/
#define CY_SYSCLK_IS_PLL_ICP_GT67MHZ (3U) /* PLL Fout >  67MHz*/
/** \endcond */

/**
* \addtogroup group_sysclk_pll_funcs
* \{
*/

cy_en_sysclk_status_t Cy_SysClk_PllConfigure(uint32_t pllNum, const cy_stc_sysclk_pll_config_t * config);
cy_en_sysclk_status_t Cy_SysClk_PllManualConfigure(uint32_t pllNum, const cy_stc_sysclk_pll_manual_config_t * config);
cy_en_sysclk_status_t Cy_SysClk_PllEnable(uint32_t pllNum, uint32_t timeoutUs);
                 void Cy_SysClk_PllGetConfiguration(uint32_t pllNum, cy_stc_sysclk_pll_manual_config_t * config);
             uint32_t Cy_SysClk_PllGetFrequency(uint32_t pllNum);
 __STATIC_INLINE bool Cy_SysClk_PllIsEnabled(uint32_t pllNum);
 __STATIC_INLINE bool Cy_SysClk_PllIsLocked(uint32_t pllNum);
 __STATIC_INLINE void Cy_SysClk_PllBypass(uint32_t pllNum, cy_en_sysclk_pll_bypass_t mode);
 __STATIC_INLINE cy_en_sysclk_pll_bypass_t Cy_SysClk_PllGetBypassState(uint32_t pllNum);
 __STATIC_INLINE bool Cy_SysClk_PllLostLock(uint32_t pllNum);
 __STATIC_INLINE void Cy_SysClk_PllDisable(uint32_t pllNum);


/*******************************************************************************
* Function Name: Cy_SysClk_PllIsEnabled
****************************************************************************//**
*
* Reports whether or not the selected PLL is enabled.
*
* \param pllNum the number of PLL instance, starting from 0.
* If there is only one PLL in device - the 0 is the only valid number.
* For other pllNum values the false (disabled) is returned.
*
* \return
* false = disabled \n
* true = enabled
*
* \funcusage
* \snippet sysclk/snippet/main.c snippet_Cy_SysClk_PllManualConfigure
*
*******************************************************************************/
__STATIC_INLINE bool Cy_SysClk_PllIsEnabled(uint32_t pllNum)
{
    bool retVal = false;
    CY_ASSERT_L1(0UL == pllNum);
    if (0UL == pllNum)
    {
        retVal = _FLD2BOOL(EXCO_PLL_CONFIG_ENABLE, EXCO_PLL_CONFIG);
    } /*  Else return false (disabled) */
    return (retVal);
}


/*******************************************************************************
* Function Name: Cy_SysClk_PllLocked
****************************************************************************//**
*
* Reports whether or not the selected PLL is locked.
*
* \param pllNum the number of PLL instance, starting from 0.
* If there is only one PLL in device - the 0 is the only valid number.
* For other pllNum values the false (unlocked) is returned.
*
* \return
* false = not locked \n
* true = locked
*
* \funcusage
* \snippet sysclk/snippet/main.c snippet_Cy_SysClk_PllManualConfigure
*
*******************************************************************************/
__STATIC_INLINE bool Cy_SysClk_PllIsLocked(uint32_t pllNum)
{
    bool retVal = false;
    CY_ASSERT_L1(0UL == pllNum);
    if (0UL == pllNum)
    {
        retVal = _FLD2BOOL(EXCO_PLL_STATUS_LOCKED, EXCO_PLL_STATUS);
    } /*  Else return false (unlocked) */
    return (retVal);
}


/*******************************************************************************
* Function Name: Cy_SysClk_PllLostLock
****************************************************************************//**
*
* Reports whether or not the selected PLL lost its lock since the last time this
* function was called. Clears the lost lock indicator.
*
* \param pllNum the number of PLL instance, starting from 0.
* If there is only one PLL in device - the 0 is the only valid number.
* For other pllNum values the false (did not lose lock) is returned.
*
* \return
* false = did not lose lock \n
* true = lost lock
*
* \funcusage
* \snippet sysclk/snippet/main.c snippet_Cy_SysClk_PllLostLock
*
*******************************************************************************/
__STATIC_INLINE bool Cy_SysClk_PllLostLock(uint32_t pllNum)
{
    bool retVal = false;
    CY_ASSERT_L1(0UL == pllNum);
    if (0UL == pllNum)
    {
        uint32_t locReg = EXCO_PLL_TEST;
        /* write a 1 to clear the UNLOCK_OCCURRED bit */
        EXCO_PLL_TEST = locReg;
        retVal = _FLD2BOOL(EXCO_PLL_TEST_UNLOCK_OCCURRED, locReg);
    } /*  Else return false (did not lose lock) */
    return (retVal);
}


/*******************************************************************************
* Function Name: Cy_SysClk_PllDisable
****************************************************************************//**
*
* Disables the selected PLL.
*
* \param pllNum the number of PLL instance, starting from 0.
* If there is only one PLL in device - the 0 is the only valid number.
*
* \return Error / status code: \n
* CY_SYSCLK_SUCCESS - PLL successfully disabled \n
* CY_SYSCLK_BAD_PARAM - invalid clock path number
*
* \note
* Call \ref SystemCoreClockUpdate after this function calling
* if it affects the CLK_HF0 frequency.
*
* \note
* Call \ref Cy_SysLib_SetWaitStates before calling this function if
* the PLL is the source of CLK_HF0 and the CLK_HF0 frequency is increasing.
*
* \note
* Call \ref Cy_SysLib_SetWaitStates after calling this function if
* the PLL is the source of CLK_HF0 and the CLK_HF0 frequency is decreasing.
*
* \funcusage
* \snippet sysclk/snippet/main.c snippet_Cy_SysClk_PllManualConfigure
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_PllDisable(uint32_t pllNum)
{
    CY_ASSERT_L1(0UL == pllNum);
    if (0UL == pllNum)
    {
        /* First bypass PLL */
        if (CY_SYSCLK_PLL_OUTPUT_OUTPUT == Cy_SysClk_PllGetBypassState(pllNum))
        {
            Cy_SysClk_PllBypass(pllNum, CY_SYSCLK_PLL_OUTPUT_INPUT);
        }
        /* Wait at least 6 PLL clock cycles */
        Cy_SysLib_DelayUs(1U);
        /* And now disable the PLL itself */
        EXCO_PLL_CONFIG &= ~(EXCO_PLL_CONFIG_ENABLE_Msk | EXCO_PLL_CONFIG_ISOLATE_N_Msk);
    }
}


/*******************************************************************************
* Function Name: Cy_SysClk_PllBypass
****************************************************************************//**
*
* Sets PLL bypass mode.
*
* \param pllNum the number of PLL instance, starting from 0.
* If there is only one PLL in device - the 0 is the only valid number.
*
* \param mode the bypass mode \ref cy_en_sysclk_pll_bypass_t.
*
* \return Error / status code: \n
* CY_SYSCLK_SUCCESS - PLL successfully disabled \n
* CY_SYSCLK_BAD_PARAM - invalid clock path number
*
* \note
* Call \ref SystemCoreClockUpdate after this function calling
* if it affects the CLK_HF0 frequency.
*
* \note
* Call \ref Cy_SysLib_SetWaitStates before calling this function if
* the PLL is the source of CLK_HF0 and the CLK_HF0 frequency is increasing.
*
* \note
* Call \ref Cy_SysLib_SetWaitStates after calling this function if
* the PLL is the source of CLK_HF0 and the CLK_HF0 frequency is decreasing.
*
* \funcusage
* \snippet sysclk/snippet/main.c snippet_Cy_SysClk_PllDisable
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_PllBypass(uint32_t pllNum, cy_en_sysclk_pll_bypass_t mode)
{
    CY_ASSERT_L1(0UL == pllNum);
    if (0UL == pllNum)
    {
        CY_REG32_CLR_SET(EXCO_PLL_CONFIG, EXCO_PLL_CONFIG_BYPASS_SEL, mode);
    }
}


/*******************************************************************************
* Function Name: Cy_SysClk_PllGetBypassState
****************************************************************************//**
*
* Returns the PLL bypass state.
*
* \param pllNum the number of PLL instance, starting from 0.
* If there is only one PLL in device - the 0 is the only valid number.
*
* \return The bypass state \ref cy_en_sysclk_pll_bypass_t.
*
* \funcusage
* \snippet sysclk/snippet/main.c snippet_Cy_SysClk_PllDisable
*
*******************************************************************************/
__STATIC_INLINE cy_en_sysclk_pll_bypass_t Cy_SysClk_PllGetBypassState(uint32_t pllNum)
{
    cy_en_sysclk_pll_bypass_t retVal = CY_SYSCLK_PLL_OUTPUT_AUTO;
    CY_ASSERT_L1(0UL == pllNum);
    if (0UL == pllNum)
    {
        retVal = (cy_en_sysclk_pll_bypass_t)_FLD2VAL(EXCO_PLL_CONFIG_BYPASS_SEL, EXCO_PLL_CONFIG);
    } /*  Else return the default value */
    return (retVal);
}

/** \} group_sysclk_pll_funcs */
#endif /* EXCO_PLL_PRESENT */
#endif /* (CY_IP_M0S8EXCO) */


/* ========================================================================== */
/* ===========================    ILO SECTION    ============================ */
/* ========================================================================== */
/**
* \addtogroup group_sysclk_ilo_funcs
* \{
*/
__STATIC_INLINE void Cy_SysClk_IloEnable(void);
__STATIC_INLINE bool Cy_SysClk_IloIsEnabled(void);
__STATIC_INLINE cy_en_sysclk_status_t Cy_SysClk_IloDisable(void);
                 void Cy_SysClk_IloStartMeasurement(void);
                 void Cy_SysClk_IloStopMeasurement(void);
cy_en_sysclk_status_t Cy_SysClk_IloCompensate(uint32_t desiredDelay , uint32_t * compensatedCycles);


/*******************************************************************************
* Function Name: Cy_SysClk_IloEnable
****************************************************************************//**
*
* Enables the ILO.
*
* \funcusage
* \snippet sysclk/snippet/main.c snippet_Cy_SysClk_WcoDisable
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_IloEnable(void)
{
    SRSSLT_CLK_ILO_CONFIG |= SRSSLT_CLK_ILO_CONFIG_ENABLE_Msk;
}


/*******************************************************************************
* Function Name: Cy_SysClk_IloIsEnabled
****************************************************************************//**
*
* Returns the ILO enable/disable state.
*
* \funcusage
* \snippet sysclk/snippet/main.c snippet_Cy_SysClk_WcoDisable
*
*******************************************************************************/
__STATIC_INLINE bool Cy_SysClk_IloIsEnabled(void)
{
    return(_FLD2BOOL(SRSSLT_CLK_ILO_CONFIG_ENABLE, SRSSLT_CLK_ILO_CONFIG));
}


/*******************************************************************************
* Function Name: Cy_SysClk_IloDisable
****************************************************************************//**
*
* Disables the ILO. ILO can't be disabled if WDT is enabled.
*
* \return Error / status code: \n
* CY_SYSCLK_SUCCESS - ILO successfully disabled \n
* CY_SYSCLK_INVALID_STATE - Cannot disable the ILO if the WDT is enabled.
*
* \note Do not call this function if the WDT is enabled, because the WDT is clocked by
* the ILO.
*
* \funcusage
* \snippet sysclk/snippet/main.c snippet_Cy_SysClk_IloDisable
*
*******************************************************************************/
__STATIC_INLINE cy_en_sysclk_status_t Cy_SysClk_IloDisable(void)
{
    cy_en_sysclk_status_t retVal = CY_SYSCLK_INVALID_STATE;

    if (!Cy_WDT_IsEnabled())
    {
        SRSSLT_CLK_ILO_CONFIG &= ~SRSSLT_CLK_ILO_CONFIG_ENABLE_Msk;
        retVal = CY_SYSCLK_SUCCESS;
    }

    return (retVal);
}
/** \} group_sysclk_ilo_funcs */


/* ========================================================================== */
/* ===========================    WCO SECTION    ============================ */
/* ========================================================================== */

#define CY_SYSCLK_WCO_CONFIG_DPLL_LF_LIMIT_MAX         (0xFFuL)
#define CY_SYSCLK_WCO_CONFIG_DPLL_LF_LIMIT_STEP        (16uL)
#define CY_SYSCLK_WCO_IMO_TIMEOUT_MS                   (20uL)
#define CY_SYSCLK_WCO_DPLL_TIMEOUT_MS                  (1uL)
#define CY_SYSCLK_WCO_TRIM_GM_HPM                      (0x1uL)
#define CY_SYSCLK_WCO_TRIM_XGM_2620NA                  (0x01uL)

/** Recommended WCO startup timeout for blocking \ref Cy_SysClk_WcoEnable execution */
#define CY_SYSCLK_WCO_TIMEOUT_US                       (20000UL)

/**
* \addtogroup group_sysclk_wco_funcs
* \{
*/
__STATIC_INLINE void Cy_SysClk_WcoEnable(uint32_t timeoutUs);
__STATIC_INLINE void Cy_SysClk_WcoDisable(void);
__STATIC_INLINE bool Cy_SysClk_WcoIsEnabled(void);
__STATIC_INLINE void Cy_SysClk_WcoBypass(bool bypass);


 /*******************************************************************************
 * Function Name: Cy_SysClk_WcoEnable
 ****************************************************************************//**
 *
 * Enables the WCO.
 *
 * \param timeoutUs - WCO startup delay in microseconds. Valid range 0..65535.
 *
 * \funcusage
 * \snippet sysclk/snippet/main.c snippet_Cy_SysClk_WcoEnable
 *
 *******************************************************************************/
 __STATIC_INLINE void Cy_SysClk_WcoEnable(uint32_t timeoutUs)
 {
     WCO_CONFIG |= WCO_CONFIG_IP_ENABLE_Msk;
     Cy_SysLib_DelayUs((uint16_t)timeoutUs);
 }


/*******************************************************************************
* Function Name: Cy_SysClk_WcoIsEnabled
****************************************************************************//**
*
* Returns the WCO enable/disable state.
*
* \funcusage
* \snippet sysclk/snippet/main.c snippet_Cy_SysClk_WcoBypass
*
*******************************************************************************/
__STATIC_INLINE bool Cy_SysClk_WcoIsEnabled(void)
{
    return(_FLD2BOOL(WCO_CONFIG_IP_ENABLE, WCO_CONFIG));
}


/*******************************************************************************
* Function Name: Cy_SysClk_WcoDisable
****************************************************************************//**
*
* Disables the WCO.
*
* \funcusage
* \snippet sysclk/snippet/main.c snippet_Cy_SysClk_WcoDisable
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_WcoDisable(void)
{
    WCO_CONFIG &= ~WCO_CONFIG_IP_ENABLE_Msk;
}


/*******************************************************************************
* Function Name: Cy_SysClk_WcoBypass
****************************************************************************//**
*
* Enables/disables external clock input through the WCO input pin.
*
* \param bypass:
* - true - external clock input through the WCO input pin (WCO bypass enabled)
* - false - regular WCO operation (bypass disabled)
*
* \funcusage
* \snippet sysclk/snippet/main.c snippet_Cy_SysClk_WcoBypass
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_WcoBypass(bool bypass)
{
    if (bypass)
    {
        WCO_CONFIG |= WCO_CONFIG_EXT_INPUT_EN_Msk;
    }
    else
    {
        WCO_CONFIG &= ~WCO_CONFIG_EXT_INPUT_EN_Msk;
    }
}

/** \} group_sysclk_wco_funcs */


/* ========================================================================== */
/* ==========================    ClkHf SECTION    =========================== */
/* ========================================================================== */
/**
* \addtogroup group_sysclk_clk_hf_enums
* \{
*/
/**
* Selects which ClkHf input to configure.
* Used with functions \ref Cy_SysClk_ClkHfSetSource and \ref Cy_SysClk_ClkHfGetSource.
*/
typedef enum
{
    CY_SYSCLK_CLKHF_IN_IMO  = 0U,  /**< IMO - Internal R/C Oscillator */
    CY_SYSCLK_CLKHF_IN_EXT  = 1U,  /**< EXTCLK - External Clock Pin  */
#if defined (CY_IP_M0S8EXCO)
    CY_SYSCLK_CLKHF_IN_ECO  = 2U,  /**< ECO - External-Crystal Oscillator */
#if defined(EXCO_PLL_PRESENT)
    CY_SYSCLK_CLKHF_IN_PLL  = 6U,  /**< PLL subsystem output */
#endif /* EXCO_PLL_PRESENT */
#endif /* CY_IP_M0S8EXCO */
} cy_en_sysclk_clkhf_src_t;


/**
* Clock divider values.
* Used with functions \ref Cy_SysClk_ClkHfSetDivider, \ref Cy_SysClk_ClkHfGetDivider,
*                     \ref Cy_SysClk_ClkSysSetDivider and \ref Cy_SysClk_ClkSysGetDivider.
*/
typedef enum
{
    CY_SYSCLK_NO_DIV = 0U,    /**< don't divide */
    CY_SYSCLK_DIV_2  = 1U,    /**< divide by 2 */
    CY_SYSCLK_DIV_4  = 2U,    /**< divide by 4 */
    CY_SYSCLK_DIV_8  = 3U     /**< divide by 8 */
} cy_en_sysclk_dividers_t;
/** \} group_sysclk_clk_hf_enums */

/** \cond */
#define CY_SYSCLK_IS_DIV_VALID(div) (((div) == CY_SYSCLK_NO_DIV) || \
                                     ((div) == CY_SYSCLK_DIV_2)  || \
                                     ((div) == CY_SYSCLK_DIV_4)  || \
                                     ((div) == CY_SYSCLK_DIV_8))
/** \endcond */

/**
* \addtogroup group_sysclk_clk_hf_funcs
* \{
*/
                  cy_en_sysclk_status_t Cy_SysClk_ClkHfSetSource(cy_en_sysclk_clkhf_src_t source);
               cy_en_sysclk_clkhf_src_t Cy_SysClk_ClkHfGetSource(void);
__STATIC_INLINE                    void Cy_SysClk_ClkHfSetDivider(cy_en_sysclk_dividers_t divider);
__STATIC_INLINE cy_en_sysclk_dividers_t Cy_SysClk_ClkHfGetDivider(void);
                               uint32_t Cy_SysClk_ClkHfGetFrequency(void);


/*******************************************************************************
* Function Name: Cy_SysClk_ClkHfSetDivider
****************************************************************************//**
*
* Sets a divider value for ClkHf.
*
* \param divider \ref cy_en_sysclk_dividers_t
*
* \return \ref cy_en_sysclk_status_t
*
* \note Also call \ref Cy_SysClk_ClkHfSetSource to set the clkHf source.
*
* \note Call \ref SystemCoreClockUpdate after this function calling.
*
* \note Call \ref Cy_SysLib_SetWaitStates before calling this function if
* ClkSys frequency is increasing.
*
* \note Call \ref Cy_SysLib_SetWaitStates after calling this function if
* ClkSys frequency is decreasing.
*
* \funcusage
* \snippet sysclk/snippet/main.c snippet_Cy_SysClk_ClkHfSetDivider
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_ClkHfSetDivider(cy_en_sysclk_dividers_t divider)
{
    if (CY_SYSCLK_IS_DIV_VALID(divider))
    {
        CY_REG32_CLR_SET(SRSSLT_CLK_SELECT, SRSSLT_CLK_SELECT_HFCLK_DIV, divider);
    }
}


/*******************************************************************************
* Function Name: Cy_SysClk_ClkHfGetDivider
****************************************************************************//**
*
* Returns the ClkHf divider value.
*
* \return \ref cy_en_sysclk_dividers_t
*
* \funcusage
* \snippet sysclk/snippet/main.c snippet_Cy_SysClk_ClkHfSetDivider
*
*******************************************************************************/
__STATIC_INLINE cy_en_sysclk_dividers_t Cy_SysClk_ClkHfGetDivider(void)
{
    return ((cy_en_sysclk_dividers_t)(_FLD2VAL(SRSSLT_CLK_SELECT_HFCLK_DIV, SRSSLT_CLK_SELECT)));
}
/** \} group_sysclk_clk_hf_funcs */


/* ========================================================================== */
/* =========================    clk_sys SECTION    ========================== */
/* ========================================================================== */

/**
* \addtogroup group_sysclk_clk_sys_funcs
* \{
*/
__STATIC_INLINE void Cy_SysClk_ClkSysSetDivider(cy_en_sysclk_dividers_t divider);
__STATIC_INLINE cy_en_sysclk_dividers_t Cy_SysClk_ClkSysGetDivider(void);
__STATIC_INLINE uint32_t Cy_SysClk_ClkSysGetFrequency(void);


/*******************************************************************************
* Function Name: Cy_SysClk_ClkSysGetFrequency
****************************************************************************//**
*
* Reports the frequency of the fast clock.
*
* \return The frequency, in Hz.
*
* \funcusage
* \snippet sysclk/snippet/main.c snippet_Cy_SysClk_ClkSysSetDivider
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SysClk_ClkSysGetFrequency(void)
{
    /* Convert the ClkSys divider enumeration value into a natural number */
    uint32_t locDiv = 1UL << (uint32_t)Cy_SysClk_ClkSysGetDivider();
    /* Divide the ClkSys input frequency by the ClkSys divider */
    return (CY_SYSLIB_DIV_ROUND(Cy_SysClk_ClkHfGetFrequency(), locDiv));
}


/*******************************************************************************
* Function Name: Cy_SysClk_ClkSysSetDivider
****************************************************************************//**
*
* Sets the ClkSys clock divider, which sources the main processor.
* The source of this divider is ClkHf.
*
* \param divider - ClkSys divider value \ref cy_en_sysclk_dividers_t.
*
* \note
* Call \ref SystemCoreClockUpdate after this function calling.
*
* \note
* Call \ref Cy_SysLib_SetWaitStates before calling this function if
* CLK_FAST frequency is increasing.
*
* \note
* Call \ref Cy_SysLib_SetWaitStates after calling this function if
* CLK_FAST frequency is decreasing.
*
* \funcusage
* \snippet sysclk/snippet/main.c snippet_Cy_SysClk_ClkSysSetDivider
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_ClkSysSetDivider(cy_en_sysclk_dividers_t divider)
{
    if (CY_SYSCLK_IS_DIV_VALID(divider))
    {
        CY_REG32_CLR_SET(SRSSLT_CLK_SELECT, SRSSLT_CLK_SELECT_SYSCLK_DIV, divider);
    }
}


/*******************************************************************************
* Function Name: Cy_SysClk_ClkSysGetDivider
****************************************************************************//**
*
* Returns the ClkSys clock divider.
*
* \return The ClkSys divider value, \ref cy_en_sysclk_dividers_t.
*
* \funcusage
* \snippet sysclk/snippet/main.c snippet_Cy_SysClk_ClkSysSetDivider
*
*******************************************************************************/
__STATIC_INLINE cy_en_sysclk_dividers_t Cy_SysClk_ClkSysGetDivider(void)
{
    return ((cy_en_sysclk_dividers_t)_FLD2VAL(SRSSLT_CLK_SELECT_SYSCLK_DIV, SRSSLT_CLK_SELECT));
}
/** \} group_sysclk_clk_sys_funcs */


/* ========================================================================== */
/* =====================    clk_peripherals SECTION    ====================== */
/* ========================================================================== */
/**
* \addtogroup group_sysclk_clk_peripheral_enums
* \{
*/
/** Programmable clock divider types */
typedef enum
{
    CY_SYSCLK_DIV_8_BIT    = 0U, /**< Divider Type is an 8 bit integer divider. Note the divider of this type may be absent on some devices */
    CY_SYSCLK_DIV_16_BIT   = 1U, /**< Divider Type is a 16 bit integer divider. Note the divider of this type may be absent on some devices */
    CY_SYSCLK_DIV_16_5_BIT = 2U, /**< Divider Type is a 16.5 bit fractional divider. Note the divider of this type may be absent on some devices */
    CY_SYSCLK_DIV_24_5_BIT = 3U  /**< Divider Type is a 24.5 bit fractional divider. Note the divider of this type may be absent on some devices */
} cy_en_sysclk_divider_types_t;
/** \} group_sysclk_clk_peripheral_enums */

/** \cond */
#define cy_en_divider_types_t cy_en_sysclk_divider_types_t /* BWC macro for capsense!!! */
/** \endcond */

/**
* \addtogroup group_sysclk_clk_peripheral_funcs
* \{
*/
cy_en_sysclk_status_t Cy_SysClk_PeriphSetDivider(cy_en_sysclk_divider_types_t dividerType, uint32_t dividerNum, uint32_t dividerValue);
             uint32_t Cy_SysClk_PeriphGetDivider(cy_en_sysclk_divider_types_t dividerType, uint32_t dividerNum);
cy_en_sysclk_status_t Cy_SysClk_PeriphSetFracDivider(cy_en_sysclk_divider_types_t dividerType, uint32_t dividerNum, uint32_t dividerIntValue, uint32_t dividerFracValue);
                 void Cy_SysClk_PeriphGetFracDivider(cy_en_sysclk_divider_types_t dividerType, uint32_t dividerNum, uint32_t *dividerIntValue, uint32_t *dividerFracValue);
cy_en_sysclk_status_t Cy_SysClk_PeriphAssignDivider(en_clk_dst_t periphNum, cy_en_sysclk_divider_types_t dividerType, uint32_t dividerNum);
cy_en_sysclk_status_t Cy_SysClk_PeriphEnableDivider(cy_en_sysclk_divider_types_t dividerType, uint32_t dividerNum);
cy_en_sysclk_status_t Cy_SysClk_PeriphDisableDivider(cy_en_sysclk_divider_types_t dividerType, uint32_t dividerNum);
cy_en_sysclk_status_t Cy_SysClk_PeriphEnablePhaseAlignDivider(cy_en_sysclk_divider_types_t dividerType, uint32_t dividerNum, cy_en_sysclk_divider_types_t dividerTypePA, uint32_t dividerNumPA);
                 bool Cy_SysClk_PeriphDividerIsEnabled(cy_en_sysclk_divider_types_t dividerType, uint32_t dividerNum);
             uint32_t Cy_SysClk_PeriphGetFrequency(cy_en_sysclk_divider_types_t dividerType, uint32_t dividerNum);
__STATIC_INLINE uint32_t Cy_SysClk_PeriphGetAssignedDivider(en_clk_dst_t periphNum);


/*******************************************************************************
* Function Name: Cy_SysClk_PeriphGetAssignedDivider
****************************************************************************//**
*
* Reports which clock divider is assigned to a selected IP block.
*
* \param periphNum specifies the peripheral block \ref en_clk_dst_t.
*
* \return The divider type and number, where bits [7:6] = type, bits[5:0] = divider
* number within that type
*
* \funcusage
* \snippet sysclk/snippet/main.c snippet_Cy_SysClk_PeriphAssignDivider
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SysClk_PeriphGetAssignedDivider(en_clk_dst_t periphNum)
{
    CY_ASSERT_L1(PERI_PCLK_CLOCK_NR > (uint32_t)periphNum);
    return (PERI_PCLK_CTL[periphNum] & (PERI_PCLK_CTL_SEL_DIV_Msk | PERI_PCLK_CTL_SEL_TYPE_Msk));
}

/** \} group_sysclk_clk_peripheral_funcs */


/* ========================================================================== */
/* =========================    clk_pump SECTION    ========================= */
/* ========================================================================== */
/**
* \addtogroup group_sysclk_clk_pump_enums
* \{
*/
/**
* Pump clock (clk_pump) input sources. See CLK_SELECT register, PUMP_SEL bits.
* Used with functions \ref Cy_SysClk_ClkPumpSetSource, and \ref Cy_SysClk_ClkPumpGetSource.
*/
typedef enum
{
    CY_SYSCLK_PUMP_IN_GND   = 0UL,  /**< No clock, connect to gnd */
    CY_SYSCLK_PUMP_IN_IMO   = 1UL,  /**< Use main IMO output */
    CY_SYSCLK_PUMP_IN_HFCLK = 2UL   /**< Use clk_hf (using selected source after predivider but before prescaler) */
} cy_en_sysclk_clkpump_src_t;
/** \} group_sysclk_clk_pump_enums */

/** \cond */
#define  CY_SYSCLK_IS_PUMP_SRC_VALID(src) (((src) == CY_SYSCLK_PUMP_IN_GND) || \
                                           ((src) == CY_SYSCLK_PUMP_IN_IMO) || \
                                           ((src) == CY_SYSCLK_PUMP_IN_HFCLK))
/** \endcond */

/**
* \addtogroup group_sysclk_clk_pump_funcs
* \{
*/
                     cy_en_sysclk_status_t Cy_SysClk_ClkPumpSetSource(cy_en_sysclk_clkpump_src_t source);
__STATIC_INLINE cy_en_sysclk_clkpump_src_t Cy_SysClk_ClkPumpGetSource(void);
__STATIC_INLINE                   uint32_t Cy_SysClk_ClkPumpGetFrequency(void);


/*******************************************************************************
* Function Name: Cy_SysClk_ClkPumpGetSource
****************************************************************************//**
*
* Reports the source for the pump clock (clk_pump).
*
* \return \ref cy_en_sysclk_clkpump_src_t
*
* \funcusage
* \snippet sysclk/snippet/main.c snippet_Cy_SysClk_ClkPumpSetSource
*
*******************************************************************************/
__STATIC_INLINE cy_en_sysclk_clkpump_src_t Cy_SysClk_ClkPumpGetSource(void)
{
    return ((cy_en_sysclk_clkpump_src_t)_FLD2VAL(SRSSLT_CLK_SELECT_PUMP_SEL, SRSSLT_CLK_SELECT));
}


/*******************************************************************************
* Function Name: Cy_SysClk_ClkPumpGetFrequency
****************************************************************************//**
*
* Reports the frequency of the pump clock (clk_pump).
*
* \return The frequency of the pump clock in Hz.
*
* \funcusage
* \snippet sysclk/snippet/main.c snippet_Cy_SysClk_ClkPumpSetSource
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SysClk_ClkPumpGetFrequency(void)
{
    uint32_t freq = 0UL;

    switch (Cy_SysClk_ClkPumpGetSource())
    {
        case CY_SYSCLK_PUMP_IN_IMO:
            freq = Cy_SysClk_ImoGetFrequency();
            break;

        case CY_SYSCLK_PUMP_IN_HFCLK:
            freq = Cy_SysClk_ClkHfGetFrequency();
            break;

        default: /* return zero for CY_SYSCLK_PUMP_IN_GND */
            break;
    }

    return (freq);
}


/** \} group_sysclk_clk_pump_funcs */


#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* CY_SYSCLK_H */

/** \} group_sysclk */


/* [] END OF FILE */
