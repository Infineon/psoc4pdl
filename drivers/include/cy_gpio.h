/***************************************************************************//**
* \file cy_gpio.h
* \version 1.0
*
* Provides an API declaration of the GPIO driver
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
* \addtogroup group_gpio
* \{
* The GPIO driver provides an API to configure and access the device Input/Output pins.
*
* The Peripheral Driver Library(PDL) GPIO functions and other declarations used
* in this driver are located in cy_gpio.h. You can optionally include cy_pdl.h
* (ModusToolbox only) to get access to all the functions and declarations in the PDL.
*
* IO pins include all general purpose types such as GPIO, SIO, HSIO, AUXIO, and
* their variants.
*
* Initialization can be performed either at the port level or by configuring
* individual pins. Use the port configuration to efficiently use the code space.
* For the list of supported ports and pins, refer to the product device header files
*
* - Single pin configuration is performed by using \ref Cy_GPIO_Pin_FastInit
*   (provide specific values) or \ref Cy_GPIO_Pin_Init (provide a filled
*   cy_stc_gpio_pin_config_t structure).
* - An entire port can be configured using \ref Cy_GPIO_Port_Init. Provide a filled
*   cy_stc_gpio_prt_config_t structure. The values in the structure are
*   bitfields representing the desired value for each pin in the port.
* - Pin configuration and management is based on the port address and pin number.
*   \ref Cy_GPIO_PortToAddr function can optionally be used to calculate the port
*   address from the port number at run-time.
*
* Once the pin/port initialization is complete, each pin can be accessed by
* specifying the port (GPIO_PRT_Type) and the pin (0-7) in the provided API
* functions.
*
* \section group_gpio_configuration Configuration Considerations
*
* 1. Pin multiplexing is controlled through the High-Speed IO Matrix (HSIOM) selection.
*    This allows the pin to connect to signal sources/sinks throughout the device,
*    as defined by the pin HSIOM selection options (en_hsiom_sel_t).
*    Refer to a device-specific gpio hearder file, e.g.
*    \e \<PDL_DIR\>/devices/include/gpio_psoc4100sp_48_tqfp.\e h
*    and the device datasheet for the supported HSIOM options.
* 2. All the pins are initialized to High-Z drive mode with HSIOM connected to CPU (SW
*    control digital pin only) at Power-On-Reset(POR).
* 3. Some API functions perform read-modify-write operations on shared port
*    registers. These functions are not thread safe, so the application must call them
*    with precaution.
* 4. Each pin provides a digital input buffer that provides a high-impedance buffer for
*    external digital inputs. The input buffer is connected to the HSIOM for routing to
*    the CPU port registers and selected peripherals. Enabling the input buffer
*    provides the possibility to read the pin state via the CPU.
*    If a pin is connected to an analog signal, the input buffer should be
*    disabled to avoid crowbar currents. For detail, refer to the device
*    TRM and device datasheet.
*
* If multiple pins on a port are updated at the same time, using direct port register writes
* with an appropriate port mask is more efficient than using the PDL single-pin functions.
* An example is shown below. Highlighted - different ways of configuring Port 1 pins using:
*
* - Port output data register
* - Port output data set register
* - Port output data clear register
*
* \snippet gpio/snippet/main.c Cy_GPIO_Snippet
*
* \section group_gpio_more_information More Information
*
* Refer to the technical reference manual (TRM) and the device datasheet.
*
* \section group_gpio_MISRA MISRA-C Compliance
*
* The GPIO driver has the following specific deviations:
*
* <table class="doxtable">
*   <tr>
*     <th>MISRA Rule</th>
*     <th>Rule Class (Required/Advisory)</th>
*     <th>Rule Description</th>
*     <th>Description of Deviation(s)</th>
*   </tr>
*   <tr>
*     <td>10.5</td>
*     <td>A</td>
*     <td>The value of an expression should not be cast to an inappropriate
*         essential type.</td>
*     <td>This code is safe because value is checked before conversion to the
*         enumeration.</td>
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
* \section group_gpio_changelog Changelog
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
*   <tr>
*     <td>1.0</td>
*     <td>Initial version</td>
*     <td></td>
*   </tr>
* </table>
*
* \defgroup group_gpio_macros Macros
* \defgroup group_gpio_functions Functions
* \{
*   \defgroup group_gpio_functions_init       Initialization Functions
*   \defgroup group_gpio_functions_gpio       GPIO Functions
*   \defgroup group_gpio_functions_sio        SIO Functions
*   \defgroup group_gpio_functions_interrupt  Port Interrupt Functions
* \}
* \defgroup group_gpio_data_structures Data Structures
* \defgroup group_gpio_enums Enumerated Types
*/

#if !defined(CY_GPIO_H)
#define CY_GPIO_H

#include <stddef.h>
#include "cy_device.h"
#include "cy_device_headers.h"
#include "cy_syslib.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** \addtogroup group_gpio_macros
* \{
*/

/** Driver major version */
#define CY_GPIO_DRV_VERSION_MAJOR       1

/** Driver minor version */
#define CY_GPIO_DRV_VERSION_MINOR       0

/** GPIO driver ID */
#define CY_GPIO_ID CY_PDL_DRV_ID(0x16U)

/** \} group_gpio_macros */


/***************************************
*       Enumerations
***************************************/
/**
* \addtogroup group_gpio_enums
* \{
*/

/** GPIO Driver error codes */
typedef enum
{
    CY_GPIO_SUCCESS   = 0x00U,                                    /**< Returned successful */
    CY_GPIO_BAD_PARAM = CY_GPIO_ID | CY_PDL_STATUS_ERROR | 0x01U, /**< Bad parameter was passed */
} cy_en_gpio_status_t;

/** AMux segment splitter switch open/close config */
typedef enum
{
    CY_GPIO_AMUX_OPENALL,   /**< Open ground switch. Open right switch. Open left switch */
    CY_GPIO_AMUX_L,         /**< Open ground switch. Open right switch. Close left switch */
    CY_GPIO_AMUX_R,         /**< Open ground switch. Close right switch. Open left switch */
    CY_GPIO_AMUX_LR,        /**< Open ground switch. Close right switch. Close left switch */
    CY_GPIO_AMUX_G,         /**< Close ground switch. Open right switch. Open left switch */
    CY_GPIO_AMUX_GL,        /**< Close ground switch. Open right switch. Close left switch */
    CY_GPIO_AMUX_GR,        /**< Close ground switch. Close right switch. Open left switch */
    CY_GPIO_AMUX_GLR,       /**< Close ground switch. Close right switch. Close left switch */
}cy_en_gpio_amuxconnect_t;

/**
* AMux Bus pin connection selection
*/
typedef enum
{
    CY_GPIO_AMUXBUSA,       /**< AMuxBus A */
    CY_GPIO_AMUXBUSB        /**< AMuxBus B */
}cy_en_gpio_amuxselect_t;

/** \} group_gpio_enums */


/***************************************
*       Configuration Structures
***************************************/

/**
* \addtogroup group_gpio_data_structures
* \{
*/

/** This structure is used to initialize a port of GPIO pins */
typedef struct
{
    uint32_t dr;             /**< Initial output data for the IO pins in the port */
    uint32_t intrCfg;        /**< Port pin interrupt edge detection configuration */
    uint32_t pc;             /**< Port pin drive modes configuration */
    uint32_t pc2;            /**< Port pin input buffer state configuration */
    uint32_t sio;            /**< Port SIO pins configuration */
    uint32_t selActive;      /**< HSIOM selection for port pins*/
} cy_stc_gpio_prt_config_t;

/** This structure is used to initialize a single GPIO pin */
typedef struct
{
    uint32_t outVal;         /**< Pin output state */
    uint32_t driveMode;      /**< Drive mode */
    en_hsiom_sel_t hsiom;    /**< HSIOM selection */
    uint32_t intEdge;        /**< Interrupt Edge type */
    uint32_t vtrip;          /**< Input buffer voltage trip type */
    uint32_t slewRate;       /**< Output buffer slew rate */
    uint32_t vregEn;         /**< SIO pair output buffer mode */
    uint32_t ibufMode;       /**< SIO pair input buffer mode */
    uint32_t vtripSel;       /**< SIO pair input buffer trip point */
    uint32_t vrefSel;        /**< SIO pair reference voltage for input buffer trip point */
    uint32_t vohSel;         /**< SIO pair regulated voltage output level */
} cy_stc_gpio_pin_config_t;

/** \} group_gpio_data_structures */

/***************************************
*        Constants
***************************************/

/** \cond INTERNAL */

/* General Constants */
#define CY_GPIO_PRT_DEINIT                     (0UL)      /**< De-init value for port registers */

/* GPIO Masks */
#define CY_GPIO_HSIOM_MASK                     (0x0FUL)   /**< HSIOM selection mask */
#define CY_GPIO_DR_MASK                        (0x01UL)   /**< Single pin mask for DR register */
#define CY_GPIO_PS_MASK                        (0x01UL)   /**< Single pin mask for PS register */
#define CY_GPIO_PC_DM_MASK                     (0x07UL)   /**< Single pin mask for drive mode in PC register */
#define CY_GPIO_PC_DM_IBUF_MASK                (0x01UL)   /**< Single pin mask for input buffer state in PC2 register */
#define CY_GPIO_DM_VAL_IBUF_DISABLE_MASK       (0x08UL)   /**< The input buffer disable mask for the Pin Drive mode value */
#define CY_GPIO_INTR_STATUS_MASK               (0x01UL)   /**< Single pin mask for interrupt status in INTR register */
#define CY_GPIO_INTR_EDGE_MASK                 (0x03UL)   /**< Single pin mask for interrupt edge type in INTR_EDGE register */
#define CY_GPIO_INTR_FLT_EDGE_MASK             (0x07UL)   /**< Single pin mask for setting filtered interrupt */

/* SIO Masks */
#define CY_GPIO_VREG_EN_MASK                   (0x01UL)   /**< Single SIO pin mask for voltage regulation enable */
#define CY_GPIO_IBUF_MASK                      (0x01UL)   /**< Single SIO pin mask for input buffer */
#define CY_GPIO_IBUF_SHIFT                     (0x01UL)   /**< Single SIO pin shift for input buffer */
#define CY_GPIO_VTRIP_SEL_MASK                 (0x01UL)   /**< Single SIO pin mask for the input buffer trip point */
#define CY_GPIO_VTRIP_SEL_SHIFT                (0x02UL)   /**< Single SIO pin shift for the input buffer trip point */
#define CY_GPIO_VREF_SEL_MASK                  (0x03UL)   /**< Single SIO pin mask for voltage reference */
#define CY_GPIO_VREF_SEL_SHIFT                 (0x03UL)   /**< Single SIO pin shift for voltage reference */
#define CY_GPIO_VOH_SEL_MASK                   (0x07UL)   /**< Single SIO pin mask for VOH */
#define CY_GPIO_VOH_SEL_SHIFT                  (0x05UL)   /**< Single SIO pin shift for VOH */

/* Special mask for SIO pin pair setting */
#define CY_GPIO_SIO_ODD_PIN_MASK               (0x00FEUL) /**< SIO pin pair selection mask */
#define CY_GPIO_SIO_PIN_MASK                   (0x00FFUL) /**< SIO pin pair mask */

/* Offsets */
#define CY_GPIO_HSIOM_OFFSET                   (2UL)      /**< Offset for HSIOM */
#define CY_GPIO_DRIVE_MODE_OFFSET              (3UL)      /**< Offset for Drive mode */
#define CY_GPIO_INBUF_OFFSET                   (3UL)      /**< Offset for input buffer */
#define CY_GPIO_INTR_CFG_OFFSET                (1UL)      /**< Offset for interrupt config */
#define CY_GPIO_INTR_FILT_OFFSET               (18UL)     /**< Offset for filtered interrupt config */
#define CY_GPIO_CFG_SIO_OFFSET                 (2UL)      /**< Offset for SIO config */

/* Parameter validation constants */
#define CY_GPIO_PINS_MAX                       (8UL)      /**< Number of pins in the port */
#define CY_GPIO_PRT_PINS_MASK                  (0x0000000FFUL)
#define CY_GPIO_PRT_INTR_CFG_EDGE_SEL_MASK     (GPIO_PRT_INTR_CFG_EDGE0_SEL_Msk | \
                                                GPIO_PRT_INTR_CFG_EDGE1_SEL_Msk | \
                                                GPIO_PRT_INTR_CFG_EDGE2_SEL_Msk | \
                                                GPIO_PRT_INTR_CFG_EDGE3_SEL_Msk | \
                                                GPIO_PRT_INTR_CFG_EDGE4_SEL_Msk | \
                                                GPIO_PRT_INTR_CFG_EDGE5_SEL_Msk | \
                                                GPIO_PRT_INTR_CFG_EDGE6_SEL_Msk | \
                                                GPIO_PRT_INTR_CFG_EDGE7_SEL_Msk)
#define CY_GPIO_PRT_INTR_CFG_RANGE_MASK        (CY_GPIO_PRT_INTR_CFG_EDGE_SEL_MASK | \
                                                GPIO_PRT_INTR_CFG_FLT_EDGE_SEL_Msk | \
                                                GPIO_PRT_INTR_CFG_FLT_SEL_Msk)
#define CY_GPIO_PRT_INT_MASK_MASK              (0x0000001FFUL)
#define CY_GPIO_PRT_SEL_ACTIVE_MASK            (0x1FFFFFFFUL)

#define GPIO_MAX_SPLIT_CELL_SEGMENTS           (9U)

/* Parameter validation macros */
#define CY_GPIO_IS_PIN_VALID(pinNum)           (CY_GPIO_PINS_MAX > (pinNum))
#define CY_GPIO_IS_FILTER_PIN_VALID(pinNum)    (CY_GPIO_PINS_MAX >= (pinNum))
#define CY_GPIO_IS_VALUE_VALID(outVal)         (1UL >= (outVal))
#define CY_GPIO_IS_DM_VALID(driveMode)         (0U == ((driveMode) & \
                                               (uint32_t)~(CY_GPIO_PC_DM_MASK | CY_GPIO_DM_VAL_IBUF_DISABLE_MASK)))

#define CY_GPIO_IS_HSIOM_VALID(hsiom)          (0U == ((hsiom) & (uint32_t)~CY_GPIO_HSIOM_MASK))

#define CY_GPIO_IS_INT_EDGE_VALID(intEdge)     ((CY_GPIO_INTR_DISABLE  == (intEdge)) || \
                                                (CY_GPIO_INTR_RISING   == (intEdge)) || \
                                                (CY_GPIO_INTR_FALLING  == (intEdge)) || \
                                                (CY_GPIO_INTR_BOTH     == (intEdge)))

#define CY_GPIO_IS_VREF_SEL_VALID(vrefSel)     ((CY_SIO_VREF_PINREF    == (vrefSel)) || \
                                                (CY_SIO_VREF_1_2V      == (vrefSel)) || \
                                                (CY_SIO_VREF_AMUX_A    == (vrefSel)) || \
                                                (CY_SIO_VREF_AMUX_B    == (vrefSel)))

#define CY_GPIO_IS_VOH_SEL_VALID(vrefSel)      ((CY_SIO_VOH_1_00       == (vrefSel)) || \
                                                (CY_SIO_VOH_1_25       == (vrefSel)) || \
                                                (CY_SIO_VOH_1_49       == (vrefSel)) || \
                                                (CY_SIO_VOH_1_67       == (vrefSel)) || \
                                                (CY_SIO_VOH_2_08       == (vrefSel)) || \
                                                (CY_SIO_VOH_2_50       == (vrefSel)) || \
                                                (CY_SIO_VOH_2_78       == (vrefSel)) || \
                                                (CY_SIO_VOH_4_16       == (vrefSel)))

#define CY_GPIO_IS_PIN_BIT_VALID(pinBit)       (0U == ((pinBit) & (uint32_t)~CY_GPIO_PRT_PINS_MASK))
#define CY_GPIO_IS_INTR_CFG_VALID(intrCfg)     (0U == ((intrCfg) & (uint32_t)~CY_GPIO_PRT_INTR_CFG_RANGE_MASK))
#define CY_GPIO_IS_SEL_ACT_VALID(selActive)    (0U == ((selActive) & (uint32_t)~CY_GPIO_PRT_SEL_ACTIVE_MASK))

#define CY_GPIO_IS_AMUX_SPLIT_VALID(switchCtrl)       (((uint32_t) (switchCtrl)) < GPIO_MAX_SPLIT_CELL_SEGMENTS)

#define CY_GPIO_IS_AMUX_CONNECT_VALID(amuxConnect)    ((CY_GPIO_AMUX_OPENALL == (amuxConnect)) || \
                                                       (CY_GPIO_AMUX_L       == (amuxConnect)) || \
                                                       (CY_GPIO_AMUX_R       == (amuxConnect)) || \
                                                       (CY_GPIO_AMUX_LR      == (amuxConnect)) || \
                                                       (CY_GPIO_AMUX_G       == (amuxConnect)) || \
                                                       (CY_GPIO_AMUX_GL      == (amuxConnect)) || \
                                                       (CY_GPIO_AMUX_GR      == (amuxConnect)) || \
                                                       (CY_GPIO_AMUX_GLR     == (amuxConnect)))

#define CY_GPIO_IS_AMUX_SELECT_VALID(amuxBus)       ((CY_GPIO_AMUXBUSA       == (amuxBus)) || \
                                                     (CY_GPIO_AMUXBUSB       == (amuxBus)))
/** \endcond */


/***************************************
*        Function Constants
***************************************/

/**
* \addtogroup group_gpio_macros
* \{
*/

/**
* \defgroup group_gpio_driveModes Pin drive mode
* \{
* Constants to be used for setting the drive mode of the pin.
*/
#define CY_GPIO_DM_HIGHZ                       (0x01UL) /**< Digital High-Z. Input buffer on */
#define CY_GPIO_DM_PULLUP                      (0x02UL) /**< Resistive Pull-Up. Input buffer on */
#define CY_GPIO_DM_PULLDOWN                    (0x03UL) /**< Resistive Pull-Down. Input buffer on */
#define CY_GPIO_DM_OD_DRIVESLOW                (0x04UL) /**< Open Drain, Drives Low. Input buffer on */
#define CY_GPIO_DM_OD_DRIVESHIGH               (0x05UL) /**< Open Drain, Drives High. Input buffer on */
#define CY_GPIO_DM_STRONG                      (0x06UL) /**< Strong Drive. Input buffer on */
#define CY_GPIO_DM_PULLUP_DOWN                 (0x07UL) /**< Resistive Pull-Up/Down. Input buffer on */
#define CY_GPIO_DM_ANALOG                      (0x09UL) /**< Analog High-Z. Input buffer off */
#define CY_GPIO_DM_PULLUP_IN_OFF               (0x0AUL) /**< Resistive Pull-Up. Input buffer off */
#define CY_GPIO_DM_PULLDOWN_IN_OFF             (0x0BUL) /**< Resistive Pull-Down. Input buffer off */
#define CY_GPIO_DM_OD_DRIVESLOW_IN_OFF         (0x0CUL) /**< Open Drain, Drives Low. Input buffer off */
#define CY_GPIO_DM_OD_DRIVESHIGH_IN_OFF        (0x0DUL) /**< Open Drain, Drives High. Input buffer off */
#define CY_GPIO_DM_STRONG_IN_OFF               (0x0EUL) /**< Strong Drive. Input buffer off */
#define CY_GPIO_DM_PULLUP_DOWN_IN_OFF          (0x0FUL) /**< Resistive Pull-Up/Down. Input buffer off */
/** \} */

/**
* \defgroup group_gpio_vtrip Voltage trip mode
* \{
* Constants to be used for setting the voltage trip type on the pin.
*/
#define CY_GPIO_VTRIP_CMOS                     (0x00UL) /**< Input buffer compatible with CMOS and I2C interfaces */
#define CY_GPIO_VTRIP_TTL                      (0x01UL) /**< Input buffer compatible with TTL and MediaLB interfaces */
/** \} */

/**
* \defgroup group_gpio_slewRate Slew Rate Mode
* \{
* Constants to be used for setting the slew rate of the pin.
*/
#define CY_GPIO_SLEW_FAST                      (0x00UL) /**< Fast slew rate */
#define CY_GPIO_SLEW_SLOW                      (0x01UL) /**< Slow slew rate */
/** \} */


/**
* \defgroup group_gpio_interruptTrigger Interrupt trigger type
* \{
* Constants to be used for setting the interrupt trigger type on the pin.
*/
#define CY_GPIO_INTR_DISABLE                   (0x00UL) /**< Disable the pin interrupt generation */
#define CY_GPIO_INTR_RISING                    (0x01UL) /**< Rising-Edge interrupt */
#define CY_GPIO_INTR_FALLING                   (0x02UL) /**< Falling-Edge interrupt */
#define CY_GPIO_INTR_BOTH                      (0x03UL) /**< Both-Edge interrupt */
/** \} */

/**
* \defgroup group_gpio_sioVreg SIO output buffer mode
* \{
* Constants to be used for setting the SIO output buffer mode on the pin.
*/
#define CY_SIO_VREG_UNREGULATED                (0x00UL) /**< Unregulated output buffer */
#define CY_SIO_VREG_REGULATED                  (0x01UL) /**< Regulated output buffer */
/** \} */

/**
* \defgroup group_gpio_sioIbuf SIO input buffer mode
* \{
* Constants to be used for setting the SIO input buffer mode on the pin.
*/
#define CY_SIO_IBUF_SINGLEENDED                (0x00UL) /**< Single ended input buffer */
#define CY_SIO_IBUF_DIFFERENTIAL               (0x01UL) /**< Differential input buffer */
/** \} */

/**
* \defgroup group_gpio_sioVtrip SIO input buffer trip-point
* \{
* Constants to be used for setting the SIO input buffer trip-point of the pin.
*/
#define CY_SIO_VTRIP_CMOS                      (0x00UL) /**< CMOS input buffer (single-ended) */
#define CY_SIO_VTRIP_TTL                       (0x01UL) /**< TTL input buffer (single-ended) */
#define CY_SIO_VTRIP_0_5VDDIO                  (0x00UL) /**< 0.5xVddio (differential) */
#define CY_SIO_VTRIP_0_4VDDIO                  (0x01UL) /**< 0.4xVddio (differential) */
#define CY_SIO_VTRIP_0_5VOH                    (0x00UL) /**< 0.5xVoh (differential) */
#define CY_SIO_VTRIP_1_0VREF                   (0x01UL) /**< 1xVref (differential) */
/** \} */

/**
* \defgroup group_gpio_sioVref SIO reference voltage for input buffer trip-point
* \{
* Constants to be used for setting the reference voltage of SIO input buffer trip-point.
*/
#define CY_SIO_VREF_PINREF                     (0x00UL) /**< Vref from analog pin */
#define CY_SIO_VREF_1_2V                       (0x01UL) /**< Vref from internal 1.2V reference */
#define CY_SIO_VREF_AMUX_A                     (0x02UL) /**< Vref from AMUXBUS_A */
#define CY_SIO_VREF_AMUX_B                     (0x03UL) /**< Vref from AMUXBUS_B */
/** \} */

/**
* \defgroup group_gpio_sioVoh Regulated output voltage level (Voh) and input buffer trip-point of an SIO pair
* \{
* Constants to be used for setting the Voh and input buffer trip-point of an SIO pair
*/
#define CY_SIO_VOH_1_00                        (0x00UL) /**< Voh = 1 x Reference */
#define CY_SIO_VOH_1_25                        (0x01UL) /**< Voh = 1.25 x Reference */
#define CY_SIO_VOH_1_49                        (0x02UL) /**< Voh = 1.49 x Reference */
#define CY_SIO_VOH_1_67                        (0x03UL) /**< Voh = 1.67 x Reference */
#define CY_SIO_VOH_2_08                        (0x04UL) /**< Voh = 2.08 x Reference */
#define CY_SIO_VOH_2_50                        (0x05UL) /**< Voh = 2.50 x Reference */
#define CY_SIO_VOH_2_78                        (0x06UL) /**< Voh = 2.78 x Reference */
#define CY_SIO_VOH_4_16                        (0x07UL) /**< Voh = 4.16 x Reference */
/** \} */

/** \} group_gpio_macros */

/***************************************
*        Function Prototypes
***************************************/

/**
* \addtogroup group_gpio_functions
* \{
*/

/**
* \addtogroup group_gpio_functions_init
* \{
*/

cy_en_gpio_status_t Cy_GPIO_Pin_Init(GPIO_PRT_Type* base, uint32_t pinNum, const cy_stc_gpio_pin_config_t *config);
cy_en_gpio_status_t Cy_GPIO_Port_Init(GPIO_PRT_Type* base, const cy_stc_gpio_prt_config_t *config);
__STATIC_INLINE void Cy_GPIO_Pin_FastInit(GPIO_PRT_Type* base, uint32_t pinNum, uint32_t driveMode, uint32_t outVal, en_hsiom_sel_t hsiom);
__STATIC_INLINE void Cy_GPIO_Port_Deinit(GPIO_PRT_Type* base);
__STATIC_INLINE void Cy_GPIO_SetHSIOM(const GPIO_PRT_Type* base, uint32_t pinNum, en_hsiom_sel_t value);
__STATIC_INLINE en_hsiom_sel_t Cy_GPIO_GetHSIOM(const GPIO_PRT_Type* base, uint32_t pinNum);
__STATIC_INLINE GPIO_PRT_Type* Cy_GPIO_PortToAddr(uint32_t portNum);

/** \} group_gpio_functions_init */

/**
* \addtogroup group_gpio_functions_gpio
* \{
*/

void Cy_GPIO_SetAmuxSplit(cy_en_amux_split_t switchCtrl, cy_en_gpio_amuxconnect_t amuxConnect, cy_en_gpio_amuxselect_t amuxBus);
cy_en_gpio_amuxconnect_t Cy_GPIO_GetAmuxSplit(cy_en_amux_split_t switchCtrl, cy_en_gpio_amuxselect_t amuxBus);

__STATIC_INLINE uint32_t Cy_GPIO_Read(const GPIO_PRT_Type* base, uint32_t pinNum);
__STATIC_INLINE void Cy_GPIO_Write(GPIO_PRT_Type* base, uint32_t pinNum, uint32_t value);
__STATIC_INLINE uint32_t Cy_GPIO_ReadOut(const GPIO_PRT_Type* base, uint32_t pinNum);
__STATIC_INLINE void Cy_GPIO_Set(GPIO_PRT_Type* base, uint32_t pinNum);
__STATIC_INLINE void Cy_GPIO_Clr(GPIO_PRT_Type* base, uint32_t pinNum);
__STATIC_INLINE void Cy_GPIO_Inv(GPIO_PRT_Type* base, uint32_t pinNum);
__STATIC_INLINE void Cy_GPIO_SetDrivemode(GPIO_PRT_Type* base, uint32_t pinNum, uint32_t value);
__STATIC_INLINE uint32_t Cy_GPIO_GetDrivemode(const GPIO_PRT_Type* base, uint32_t pinNum);
__STATIC_INLINE void Cy_GPIO_SetVtrip(GPIO_PRT_Type* base, uint32_t value);
__STATIC_INLINE uint32_t Cy_GPIO_GetVtrip(const GPIO_PRT_Type* base);
__STATIC_INLINE void Cy_GPIO_SetSlewRate(GPIO_PRT_Type* base, uint32_t value);
__STATIC_INLINE uint32_t Cy_GPIO_GetSlewRate(const GPIO_PRT_Type* base);

/** \} group_gpio_functions_gpio */

/**
* \addtogroup group_gpio_functions_sio
* \{
*/

__STATIC_INLINE void Cy_GPIO_SetVregEn(GPIO_PRT_Type* base, uint32_t pinNum, uint32_t value);
__STATIC_INLINE uint32_t Cy_GPIO_GetVregEn(const GPIO_PRT_Type* base, uint32_t pinNum);
__STATIC_INLINE void Cy_GPIO_SetIbufMode(GPIO_PRT_Type* base, uint32_t pinNum, uint32_t value);
__STATIC_INLINE uint32_t Cy_GPIO_GetIbufMode(const GPIO_PRT_Type* base, uint32_t pinNum);
__STATIC_INLINE void Cy_GPIO_SetVtripSel(GPIO_PRT_Type* base, uint32_t pinNum, uint32_t value);
__STATIC_INLINE uint32_t Cy_GPIO_GetVtripSel(const GPIO_PRT_Type* base, uint32_t pinNum);
__STATIC_INLINE void Cy_GPIO_SetVrefSel(GPIO_PRT_Type* base, uint32_t pinNum, uint32_t value);
__STATIC_INLINE uint32_t Cy_GPIO_GetVrefSel(const GPIO_PRT_Type* base, uint32_t pinNum);
__STATIC_INLINE void Cy_GPIO_SetVohSel(GPIO_PRT_Type* base, uint32_t pinNum, uint32_t value);
__STATIC_INLINE uint32_t Cy_GPIO_GetVohSel(const GPIO_PRT_Type* base, uint32_t pinNum);

/** \} group_gpio_functions_sio */

/**
* \addtogroup group_gpio_functions_interrupt
* \{
*/

__STATIC_INLINE uint32_t Cy_GPIO_GetInterruptStatus(const GPIO_PRT_Type* base, uint32_t pinNum);
__STATIC_INLINE void Cy_GPIO_ClearInterrupt(GPIO_PRT_Type* base, uint32_t pinNum);
__STATIC_INLINE void Cy_GPIO_SetInterruptEdge(GPIO_PRT_Type* base, uint32_t pinNum, uint32_t value);
__STATIC_INLINE uint32_t Cy_GPIO_GetInterruptEdge(const GPIO_PRT_Type* base, uint32_t pinNum);
__STATIC_INLINE void Cy_GPIO_SetFilter(GPIO_PRT_Type* base, uint32_t value);
__STATIC_INLINE uint32_t Cy_GPIO_GetFilter(const GPIO_PRT_Type* base);

__STATIC_INLINE uint32_t Cy_GPIO_GetInterruptCause(void);

/** \} group_gpio_functions_interrupt */


/**
* \addtogroup group_gpio_functions_init
* \{
*/

/*******************************************************************************
* Function Name: Cy_GPIO_SetHSIOM
****************************************************************************//**
*
* Configures the HSIOM connection to the pin.
*
* Connects the specified High-Speed Input Output Multiplexer (HSIOM) selection
* to the pin.
*
* \param base
* Pointer to the pin's port register base address
*
* \param pinNum
* Position of the pin bit-field within the port register
*
* \param value
* HSIOM input selection
*
* \note
* This function modifies a port register in a read-modify-write operation. It is
* not thread safe as the resource is shared among multiple pins on a port.
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_SetHSIOM
*
*******************************************************************************/
__STATIC_INLINE void Cy_GPIO_SetHSIOM(const GPIO_PRT_Type* base, uint32_t pinNum, en_hsiom_sel_t value)
{
    uint32_t portNum;
    uint32_t tempReg;
    HSIOM_PRT_Type* portAddrHSIOM;

    CY_ASSERT_L2(CY_GPIO_IS_PIN_VALID(pinNum));
    CY_ASSERT_L2(CY_GPIO_IS_HSIOM_VALID(value));

    portNum = ((uint32_t)(base) - GPIO_BASE) / GPIO_PRT_SECTION_SIZE;
    portAddrHSIOM = (HSIOM_PRT_Type*)(HSIOM_BASE + (HSIOM_PRT_SECTION_SIZE * portNum));

    tempReg = HSIOM_PRT_PORT_SEL(portAddrHSIOM) & ~(CY_GPIO_HSIOM_MASK << (pinNum << CY_GPIO_HSIOM_OFFSET));
    HSIOM_PRT_PORT_SEL(portAddrHSIOM) = tempReg | (( (uint32_t) value & CY_GPIO_HSIOM_MASK) << (pinNum << CY_GPIO_HSIOM_OFFSET));
}


/*******************************************************************************
* Function Name: Cy_GPIO_GetHSIOM
****************************************************************************//**
*
* Returns the current HSIOM multiplexer connection to the pin.
*
* \param base
* Pointer to the pin's port register base address
*
* \param pinNum
* Position of the pin bit-field within the port register
*
* \return
* HSIOM input selection
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_SetHSIOM
*
*******************************************************************************/
__STATIC_INLINE en_hsiom_sel_t Cy_GPIO_GetHSIOM(const GPIO_PRT_Type* base, uint32_t pinNum)
{
    uint32_t returnValue;
    uint32_t portNum;
    const HSIOM_PRT_Type* portAddrHSIOM;

    CY_ASSERT_L2(CY_GPIO_IS_PIN_VALID(pinNum));

    portNum = ((uint32_t)(base) - GPIO_BASE) / GPIO_PRT_SECTION_SIZE;
    portAddrHSIOM = (HSIOM_PRT_Type*)(HSIOM_BASE + (HSIOM_PRT_SECTION_SIZE * portNum));

    returnValue = (HSIOM_PRT_PORT_SEL(portAddrHSIOM) >> (pinNum << CY_GPIO_HSIOM_OFFSET)) & CY_GPIO_HSIOM_MASK;

    return (en_hsiom_sel_t)returnValue;
}


/*******************************************************************************
* Function Name: Cy_GPIO_PortToAddr
****************************************************************************//**
*
* Retrieves the port address based on the given port number.
*
* This is a helper function to calculate the port base address when given a port
* number. It is to be used when pin access needs to be calculated at runtime.
*
* \param portNum
* Port number
*
* \return
* Base address of the port register structure
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_PortToAddr
*
*******************************************************************************/
__STATIC_INLINE GPIO_PRT_Type* Cy_GPIO_PortToAddr(uint32_t portNum)
{
    GPIO_PRT_Type* portBase;

    if(portNum < (uint32_t)IOSS_GPIO_GPIO_PORT_NR)
    {
        portBase = (GPIO_PRT_Type *)(GPIO_BASE + (GPIO_PRT_SECTION_SIZE * portNum));
    }
    else
    {
        /* Error: Return default base address */
        portBase = (GPIO_PRT_Type *)(GPIO_BASE);
    }

    return (portBase);
}


/*******************************************************************************
* Function Name: Cy_GPIO_Pin_FastInit
****************************************************************************//**
*
* Initialize the most common configuration settings for all pin types.
*
* These include, drive mode, initial output value, and HSIOM connection.
*
* \param base
* Pointer to the pin's port register base address
*
* \param pinNum
* Position of the pin bit-field within the port register
*
* \param driveMode
* Pin drive mode. Options are detailed in \ref group_gpio_driveModes macros
*
* \param outVal
* Logic state of the output buffer driven to the pin (1 or 0)
*
* \param hsiom
* HSIOM input selection
*
* \note
* This function modifies port registers in read-modify-write operations. It is
* not thread safe as the resource is shared among multiple pins on a port.
* You can use the Cy_SysLib_EnterCriticalSection() and
* Cy_SysLib_ExitCriticalSection() functions to ensure that
* Cy_GPIO_Pin_FastInit() function execution is not interrupted.
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_Pin_FastInit
*
*******************************************************************************/
__STATIC_INLINE void Cy_GPIO_Pin_FastInit(GPIO_PRT_Type* base, uint32_t pinNum, uint32_t driveMode,
                                        uint32_t outVal, en_hsiom_sel_t hsiom)
{
    CY_ASSERT_L2(CY_GPIO_IS_PIN_VALID(pinNum));
    CY_ASSERT_L2(CY_GPIO_IS_DM_VALID(driveMode));
    CY_ASSERT_L2(CY_GPIO_IS_VALUE_VALID(outVal));
    CY_ASSERT_L2(CY_GPIO_IS_HSIOM_VALID(hsiom));

    uint32_t tempReg;

    tempReg = (GPIO_PRT_DR(base) & ~(CY_GPIO_DR_MASK << pinNum));
    GPIO_PRT_DR(base) = tempReg | ((outVal & CY_GPIO_DR_MASK) << pinNum);

    Cy_GPIO_SetDrivemode(base, pinNum, driveMode);

    Cy_GPIO_SetHSIOM(base, pinNum, hsiom);
}


/*******************************************************************************
* Function Name: Cy_GPIO_Port_Deinit
****************************************************************************//**
*
* Reset a complete port of pins back to power on reset defaults.
*
* \param base
* Pointer to the pin's port register base address
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_Port_Deinit
*
*******************************************************************************/
__STATIC_INLINE void Cy_GPIO_Port_Deinit(GPIO_PRT_Type* base)
{
    uint32_t portNum;
    HSIOM_PRT_Type* baseHSIOM;

    portNum = ((uint32_t)(base) - GPIO_BASE) / GPIO_PRT_SECTION_SIZE;
    baseHSIOM = (HSIOM_PRT_Type*)(HSIOM_BASE + (HSIOM_PRT_SECTION_SIZE * portNum));

    GPIO_PRT_DR(base)              = CY_GPIO_PRT_DEINIT;
    GPIO_PRT_PC(base)              = CY_GPIO_PRT_DEINIT;
    GPIO_PRT_PC2(base)             = CY_GPIO_PRT_DEINIT;
    GPIO_PRT_INTR_CFG(base)        = CY_GPIO_PRT_DEINIT;
    GPIO_PRT_SIO(base)             = CY_GPIO_PRT_DEINIT;
    HSIOM_PRT_PORT_SEL(baseHSIOM)  = CY_GPIO_PRT_DEINIT;
}
/** \} group_gpio_functions_init */

/**
* \addtogroup group_gpio_functions_gpio
* \{
*/

/*******************************************************************************
* Function Name: Cy_GPIO_Read
****************************************************************************//**
*
* Reads the current logic level on the input buffer of the pin.
*
* \param base
* Pointer to the pin's port register base address
*
* \param pinNum
* Position of the pin bit-field within the port register.
* Bit position 8 is the routed pin through the port glitch filter.
*
* \return
* Logic level present on the pin
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_Read
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_GPIO_Read(const GPIO_PRT_Type* base, uint32_t pinNum)
{
    CY_ASSERT_L2(CY_GPIO_IS_FILTER_PIN_VALID(pinNum));

    return (GPIO_PRT_IN(base) >> (pinNum)) & CY_GPIO_PS_MASK;
}


/*******************************************************************************
* Function Name: Cy_GPIO_Write
****************************************************************************//**
*
* Write a logic 0 or logic 1 state to the output driver.
*
* This function should be used only for software driven pins. It does not have
* any effect on peripheral driven pins.
*
* \param base
* Pointer to the pin's port register base address
*
* \param pinNum
* Position of the pin bit-field within the port register
*
* \param value
* Logic level to drive out on the pin
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_Write
*
*******************************************************************************/
__STATIC_INLINE void Cy_GPIO_Write(GPIO_PRT_Type* base, uint32_t pinNum, uint32_t value)
{
    CY_ASSERT_L2(CY_GPIO_IS_PIN_VALID(pinNum));
    CY_ASSERT_L2(CY_GPIO_IS_VALUE_VALID(value));

    /* Thread-safe: Directly access the pin registers instead of base->OUT */
    if(0UL == value)
    {
        GPIO_PRT_OUT_CLR(base) = CY_GPIO_DR_MASK << pinNum;
    }
    else
    {
        GPIO_PRT_OUT_SET(base) = CY_GPIO_DR_MASK << pinNum;
    }
}


/*******************************************************************************
* Function Name: Cy_GPIO_ReadOut
****************************************************************************//**
*
* Reads the current logic level on the pin output driver.
*
* \param base
* Pointer to the pin's port register base address
*
* \param pinNum
* Position of the pin bit-field within the port register
*
* \return
* Logic level on the pin output driver
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_ReadOut
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_GPIO_ReadOut(const GPIO_PRT_Type* base, uint32_t pinNum)
{
    CY_ASSERT_L2(CY_GPIO_IS_PIN_VALID(pinNum));

    return (GPIO_PRT_DR(base) >> pinNum) & CY_GPIO_DR_MASK;
}


/*******************************************************************************
* Function Name: Cy_GPIO_Set
****************************************************************************//**
*
* Set a pin output to logic state high.
*
* This function should be used only for software driven pins. It does not have
* any effect on peripheral driven pins.
*
* \param base
* Pointer to the pin's port register base address
*
* \param pinNum
* Position of the pin bit-field within the port register
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_Set
*
*******************************************************************************/
__STATIC_INLINE void Cy_GPIO_Set(GPIO_PRT_Type* base, uint32_t pinNum)
{
    CY_ASSERT_L2(CY_GPIO_IS_PIN_VALID(pinNum));

    GPIO_PRT_OUT_SET(base) = CY_GPIO_DR_MASK << pinNum;
}


/*******************************************************************************
* Function Name: Cy_GPIO_Clr
****************************************************************************//**
*
* Set a pin output to logic state Low.
*
* This function should be used only for software driven pins. It does not have
* any effect on peripheral driven pins.
*
* \param base
* Pointer to the pin's port register base address
*
* \param pinNum
* Position of the pin bit-field within the port register
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_Clr
*
*******************************************************************************/
__STATIC_INLINE void Cy_GPIO_Clr(GPIO_PRT_Type* base, uint32_t pinNum)
{
    CY_ASSERT_L2(CY_GPIO_IS_PIN_VALID(pinNum));

    GPIO_PRT_OUT_CLR(base) = CY_GPIO_DR_MASK << pinNum;
}


/*******************************************************************************
* Function Name: Cy_GPIO_Inv
****************************************************************************//**
*
* Set a pin output logic state to the inverse of the current output
* logic state.
*
* This function should be used only for software driven pins. It does not have
* any effect on peripheral driven pins.
*
* \param base
* Pointer to the pin's port register base address
*
* \param pinNum
* Position of the pin bit-field within the port register
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_Inv
*
*******************************************************************************/
__STATIC_INLINE void Cy_GPIO_Inv(GPIO_PRT_Type* base, uint32_t pinNum)
{
    CY_ASSERT_L2(CY_GPIO_IS_PIN_VALID(pinNum));

    GPIO_PRT_OUT_INV(base) = CY_GPIO_DR_MASK << pinNum;
}


/*******************************************************************************
* Function Name: Cy_GPIO_SetDrivemode
****************************************************************************//**
*
* Configures the pin output buffer drive mode and input buffer enable.
*
* The output buffer drive mode and input buffer enable are combined into a single
* parameter. The drive mode controls the behavior of the pin in general.
* Enabling the input buffer allows the digital pin state to be read but also
* contributes to extra current consumption.
*
* \param base
* Pointer to the pin's port register base address
*
* \param pinNum
* Position of the pin bit-field within the port register
*
* \param value
* Pin drive mode. Options are detailed in \ref group_gpio_driveModes macros
*
* \note
* This function modifies a port register in a read-modify-write operation. It is
* not thread safe as the resource is shared among multiple pins on a port.
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_SetDrivemode
*
*******************************************************************************/
__STATIC_INLINE void Cy_GPIO_SetDrivemode(GPIO_PRT_Type* base, uint32_t pinNum, uint32_t value)
{
    uint32_t tempReg;
    uint32_t pinLoc;

    CY_ASSERT_L2(CY_GPIO_IS_PIN_VALID(pinNum));
    CY_ASSERT_L2(CY_GPIO_IS_DM_VALID(value));

    pinLoc = pinNum * CY_GPIO_DRIVE_MODE_OFFSET;
    tempReg = (GPIO_PRT_PC(base) & ~(CY_GPIO_PC_DM_MASK << pinLoc));
    GPIO_PRT_PC(base) = tempReg | ((value & CY_GPIO_PC_DM_MASK) << pinLoc);

    tempReg = (GPIO_PRT_PC2(base) & ~(CY_GPIO_PC_DM_IBUF_MASK << pinNum));
    GPIO_PRT_PC2(base) = tempReg | (((value & CY_GPIO_DM_VAL_IBUF_DISABLE_MASK) >> CY_GPIO_DRIVE_MODE_OFFSET) << pinNum);
}


/*******************************************************************************
* Function Name: Cy_GPIO_GetDrivemode
****************************************************************************//**
*
* Returns the pin output buffer drive mode and input buffer enable state.
*
* \param base
* Pointer to the pin's port register base address
*
* \param pinNum
* Position of the pin bit-field within the port register
*
* \return
* Pin drive mode. Options are detailed in \ref group_gpio_driveModes macros
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_SetDrivemode
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_GPIO_GetDrivemode(const GPIO_PRT_Type* base, uint32_t pinNum)
{
    uint32_t pinLoc;
    uint32_t tempReg;

    CY_ASSERT_L2(CY_GPIO_IS_PIN_VALID(pinNum));

    pinLoc = pinNum * CY_GPIO_DRIVE_MODE_OFFSET;
    tempReg = ((GPIO_PRT_PC(base) >> pinLoc) & CY_GPIO_PC_DM_MASK);

    return (tempReg | (((GPIO_PRT_PC2(base) >> pinNum) & CY_GPIO_PC_DM_IBUF_MASK) << CY_GPIO_DRIVE_MODE_OFFSET));
}


/*******************************************************************************
* Function Name: Cy_GPIO_SetVtrip
****************************************************************************//**
*
* Configures the GPIO port input buffer voltage Threshold mode.
*
* \param base
* Pointer to the pin's port register base address
*
* \param value
* Pin voltage threshold mode. Options are detailed in \ref group_gpio_vtrip macros
*
* \note
* This function modifies a port register in a read-modify-write operation. It is
* not thread safe as the resource is shared among multiple pins on a port.
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_SetVtrip
*
*******************************************************************************/
__STATIC_INLINE void Cy_GPIO_SetVtrip(GPIO_PRT_Type* base, uint32_t value)
{
    CY_ASSERT_L2(CY_GPIO_IS_VALUE_VALID(value));

    CY_REG32_CLR_SET(GPIO_PRT_PC(base), GPIO_PRT_PC_PORT_VTRIP_SEL, value);
}


/*******************************************************************************
* Function Name: Cy_GPIO_GetVtrip
****************************************************************************//**
*
* Returns the port input buffer voltage Threshold mode.
*
* \param base
* Pointer to the pin's port register base address
*
* \return
* Pin voltage threshold mode. Options are detailed in \ref group_gpio_vtrip macros
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_SetVtrip
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_GPIO_GetVtrip(const GPIO_PRT_Type* base)
{
    return (_FLD2VAL(GPIO_PRT_PC_PORT_VTRIP_SEL, GPIO_PRT_PC(base)));
}


/*******************************************************************************
* Function Name: Cy_GPIO_SetSlewRate
****************************************************************************//**
*
* Configures the port output buffer slew rate. Affects all the pins on the port.
*
* \note
* This function has no effect on GPIO ports where slew rate
* configuration is not available. For detail, refer to the device datasheet.
*
* \param base
* The pointer to the port's register base address
*
* \param value
* Port slew rate. Options are detailed in \ref group_gpio_slewRate macros
*
* \note
* This function modifies a port register in the read-modify-write operation.
* It is not thread-safe.
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_SetSlewRate
*
*******************************************************************************/
__STATIC_INLINE void Cy_GPIO_SetSlewRate(GPIO_PRT_Type* base, uint32_t value)
{
    CY_ASSERT_L2(CY_GPIO_IS_VALUE_VALID(value));

    CY_REG32_CLR_SET(GPIO_PRT_PC(base), GPIO_PRT_PC_PORT_SLOW, value);
}


/*******************************************************************************
* Function Name: Cy_GPIO_GetSlewRate
****************************************************************************//**
*
* Returns the port output buffer slew rate.
*
* \param base
* The pointer to the port's register base address
*
* \return
* The port slew rate. The options are detailed in the
* \ref group_gpio_slewRate macros
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_SetSlewRate
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_GPIO_GetSlewRate(const GPIO_PRT_Type* base)
{
    return (_FLD2VAL(GPIO_PRT_PC_PORT_SLOW, GPIO_PRT_PC(base)));
}


/** \} group_gpio_functions_gpio */

/**
* \addtogroup group_gpio_functions_sio
* \{
*/

/*******************************************************************************
* Function Name: Cy_GPIO_SetVregEn
****************************************************************************//**
*
* Configures the SIO pin pair output buffer regulation mode.
*
* Note that this function has no effect on non-SIO pins.
*
* \param base
* Pointer to the pin's port register base address
*
* \param pinNum
* Position of the pin bit-field within the port register
*
* \param value
* SIO pair output buffer regulator mode. Options are detailed in \ref group_gpio_sioVreg macros
*
* \note
* This function modifies a port register in a read-modify-write operation. It is
* not thread safe as the resource is shared among multiple pins on a port.
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_SetVregEn
*
*******************************************************************************/
__STATIC_INLINE void Cy_GPIO_SetVregEn(GPIO_PRT_Type* base, uint32_t pinNum, uint32_t value)
{
    uint32_t tempReg;
    uint32_t pinLoc;

    CY_ASSERT_L2(CY_GPIO_IS_PIN_VALID(pinNum));
    CY_ASSERT_L2(CY_GPIO_IS_VALUE_VALID(value));

    pinLoc = (pinNum & CY_GPIO_SIO_ODD_PIN_MASK) << CY_GPIO_CFG_SIO_OFFSET;
    tempReg = GPIO_PRT_SIO(base) & ~(CY_GPIO_VREG_EN_MASK << pinLoc);
    GPIO_PRT_SIO(base) = tempReg | ((value & CY_GPIO_VREG_EN_MASK) << pinLoc);
}


/*******************************************************************************
* Function Name: Cy_GPIO_GetVregEn
****************************************************************************//**
*
* Returns the SIO pin pair output buffer regulation mode.
*
* Note that this function has no effect on non-SIO pins.
*
* \param base
* Pointer to the pin's port register base address
*
* \param pinNum
* Position of the pin bit-field within the port register
*
* \return
* SIO pair output buffer regulator mode. Options are detailed in \ref group_gpio_sioVreg macros
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_SetVregEn
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_GPIO_GetVregEn(const GPIO_PRT_Type* base, uint32_t pinNum)
{
    CY_ASSERT_L2(CY_GPIO_IS_PIN_VALID(pinNum));

    return (GPIO_PRT_SIO(base) >> ((pinNum & CY_GPIO_SIO_ODD_PIN_MASK) << CY_GPIO_CFG_SIO_OFFSET)) & CY_GPIO_VREG_EN_MASK;
}


/*******************************************************************************
* Function Name: Cy_GPIO_SetIbufMode
****************************************************************************//**
*
* Configures the SIO pin pair input buffer mode.
*
* Note that this function has no effect on non-SIO pins.
*
* \param base
* Pointer to the pin's port register base address
*
* \param pinNum
* Position of the pin bit-field within the port register
*
* \param value
* SIO pair input buffer mode. Options are detailed in \ref group_gpio_sioIbuf macros
*
* \note
* This function modifies a port register in a read-modify-write operation. It is
* not thread safe as the resource is shared among multiple pins on a port.
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_SetIbufMode
*
*******************************************************************************/
__STATIC_INLINE void Cy_GPIO_SetIbufMode(GPIO_PRT_Type* base, uint32_t pinNum, uint32_t value)
{
    uint32_t tempReg;
    uint32_t pinLoc;

    CY_ASSERT_L2(CY_GPIO_IS_PIN_VALID(pinNum));
    CY_ASSERT_L2(CY_GPIO_IS_VALUE_VALID(value));

    pinLoc = ((pinNum & CY_GPIO_SIO_ODD_PIN_MASK) << CY_GPIO_CFG_SIO_OFFSET) + CY_GPIO_IBUF_SHIFT;
    tempReg = (GPIO_PRT_SIO(base) & ~(CY_GPIO_IBUF_MASK << pinLoc));
    GPIO_PRT_SIO(base) = tempReg | ((value & CY_GPIO_IBUF_MASK) << pinLoc);
}


/*******************************************************************************
* Function Name: Cy_GPIO_GetIbufMode
****************************************************************************//**
*
* Returns the SIO pin pair input buffer mode.
*
* Note that this function has no effect on non-SIO pins.
*
* \param base
* Pointer to the pin's port register base address
*
* \param pinNum
* Position of the pin bit-field within the port register
*
* \return
* SIO pair input buffer mode. Options are detailed in \ref group_gpio_sioIbuf macros
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_SetIbufMode
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_GPIO_GetIbufMode(const GPIO_PRT_Type* base, uint32_t pinNum)
{
    CY_ASSERT_L2(CY_GPIO_IS_PIN_VALID(pinNum));

    return (GPIO_PRT_SIO(base) >> (((pinNum & CY_GPIO_SIO_ODD_PIN_MASK) << CY_GPIO_CFG_SIO_OFFSET) + CY_GPIO_IBUF_SHIFT)) & CY_GPIO_IBUF_MASK;
}


/*******************************************************************************
* Function Name: Cy_GPIO_SetVtripSel
****************************************************************************//**
*
* Configures the SIO pin pair input buffer trip point.
*
* Note that this function has no effect on non-SIO pins.
*
* \param base
* Pointer to the pin's port register base address
*
* \param pinNum
* Position of the pin bit-field within the port register
*
* \param value
* SIO pair input buffer trip point. Options are detailed in \ref group_gpio_sioVtrip macros
*
* \note
* This function modifies a port register in a read-modify-write operation. It is
* not thread safe as the resource is shared among multiple pins on a port.
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_SetVtripSel
*
*******************************************************************************/
__STATIC_INLINE void Cy_GPIO_SetVtripSel(GPIO_PRT_Type* base, uint32_t pinNum, uint32_t value)
{
    uint32_t tempReg;
    uint32_t pinLoc;

    CY_ASSERT_L2(CY_GPIO_IS_PIN_VALID(pinNum));
    CY_ASSERT_L2(CY_GPIO_IS_VALUE_VALID(value));

    pinLoc = ((pinNum & CY_GPIO_SIO_ODD_PIN_MASK) << CY_GPIO_CFG_SIO_OFFSET) + CY_GPIO_VTRIP_SEL_SHIFT;
    tempReg = (GPIO_PRT_SIO(base) & ~(CY_GPIO_VTRIP_SEL_MASK << pinLoc));
    GPIO_PRT_SIO(base) = tempReg | ((value & CY_GPIO_VTRIP_SEL_MASK) << pinLoc);
}


/*******************************************************************************
* Function Name: Cy_GPIO_GetVtripSel
****************************************************************************//**
*
* Returns the SIO pin pair input buffer trip point.
*
* Note that this function has no effect on non-SIO pins.
*
* \param base
* Pointer to the pin's port register base address
*
* \param pinNum
* Position of the pin bit-field within the port register
*
* \return
* SIO pair input buffer trip point. Options are detailed in \ref group_gpio_sioVtrip macros
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_SetVtripSel
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_GPIO_GetVtripSel(const GPIO_PRT_Type* base, uint32_t pinNum)
{
    CY_ASSERT_L2(CY_GPIO_IS_PIN_VALID(pinNum));

    return (GPIO_PRT_SIO(base) >> (((pinNum & CY_GPIO_SIO_ODD_PIN_MASK) << CY_GPIO_CFG_SIO_OFFSET) + CY_GPIO_VTRIP_SEL_SHIFT)) & CY_GPIO_VTRIP_SEL_MASK;
}


/*******************************************************************************
* Function Name: Cy_GPIO_SetVrefSel
****************************************************************************//**
*
* Configures the SIO reference voltage for the input buffer trip point.
*
* Note that this function has no effect on non-SIO pins.
*
* \param base
* Pointer to the pin's port register base address
*
* \param pinNum
* Position of the pin bit-field within the port register
*
* \param value
* SIO pair reference voltage. Options are detailed in \ref group_gpio_sioVref macros
*
* \note
* This function modifies a port register in a read-modify-write operation. It is
* not thread safe as the resource is shared among multiple pins on a port.
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_SetVrefSel
*
*******************************************************************************/
__STATIC_INLINE void Cy_GPIO_SetVrefSel(GPIO_PRT_Type* base, uint32_t pinNum, uint32_t value)
{
    uint32_t tempReg;
    uint32_t pinLoc;

    CY_ASSERT_L2(CY_GPIO_IS_PIN_VALID(pinNum));
    CY_ASSERT_L2(CY_GPIO_IS_VREF_SEL_VALID(value));

    pinLoc = ((pinNum & CY_GPIO_SIO_ODD_PIN_MASK) << CY_GPIO_CFG_SIO_OFFSET) + CY_GPIO_VREF_SEL_SHIFT;
    tempReg = (GPIO_PRT_SIO(base) & ~(CY_GPIO_VREF_SEL_MASK << pinLoc));
    GPIO_PRT_SIO(base) = tempReg | ((value & CY_GPIO_VREF_SEL_MASK) << pinLoc);
}


/*******************************************************************************
* Function Name: Cy_GPIO_GetVrefSel
****************************************************************************//**
*
* Returns the SIO reference voltage for the input buffer trip point.
*
* Note that this function has no effect on non-SIO pins.
*
* \param base
* Pointer to the pin's port register base address
*
* \param pinNum
* Position of the pin bit-field within the port register
*
* \return
* SIO pair reference voltage. Options are detailed in \ref group_gpio_sioVref macros
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_SetVrefSel
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_GPIO_GetVrefSel(const GPIO_PRT_Type* base, uint32_t pinNum)
{
    CY_ASSERT_L2(CY_GPIO_IS_PIN_VALID(pinNum));

    return (GPIO_PRT_SIO(base) >> (((pinNum & CY_GPIO_SIO_ODD_PIN_MASK) << CY_GPIO_CFG_SIO_OFFSET) + CY_GPIO_VREF_SEL_SHIFT)) & CY_GPIO_VREF_SEL_MASK;
}


/*******************************************************************************
* Function Name: Cy_GPIO_SetVohSel
****************************************************************************//**
*
* Configures the regulated output reference multiplier for the SIO pin pair.
*
* The regulated output reference controls both the output level of digital output
* pin and the input trip point of digital input pin in the SIO pair.
*
* Note that this function has no effect on non-SIO pins.
*
* \param base
* Pointer to the pin's port register base address
*
* \param pinNum
* Position of the pin bit-field within the port register
*
* \param value
* SIO pair reference voltage. Options are detailed in \ref group_gpio_sioVoh macros
*
* \note
* This function modifies a port register in a read-modify-write operation. It is
* not thread safe as the resource is shared among multiple pins on a port.
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_SetVohSel
*
*******************************************************************************/
__STATIC_INLINE void Cy_GPIO_SetVohSel(GPIO_PRT_Type* base, uint32_t pinNum, uint32_t value)
{
    uint32_t tempReg;
    uint32_t pinLoc;

    CY_ASSERT_L2(CY_GPIO_IS_PIN_VALID(pinNum));
    CY_ASSERT_L2(CY_GPIO_IS_VOH_SEL_VALID(value));

    pinLoc = ((pinNum & CY_GPIO_SIO_ODD_PIN_MASK) << CY_GPIO_CFG_SIO_OFFSET) + CY_GPIO_VOH_SEL_SHIFT;
    tempReg = (GPIO_PRT_SIO(base) & ~(CY_GPIO_VOH_SEL_MASK << pinLoc));
    GPIO_PRT_SIO(base) = tempReg | ((value & CY_GPIO_VOH_SEL_MASK) << pinLoc);
}


/*******************************************************************************
* Function Name: Cy_GPIO_GetVohSel
****************************************************************************//**
*
* Returns the regulated output reference multiplier for the SIO pin pair.
*
* Note that this function has no effect on non-SIO pins.
*
* \param base
* Pointer to the pin's port register base address
*
* \param pinNum
* Position of the pin bit-field within the port register
*
* \return
* SIO pair reference voltage. Options are detailed in \ref group_gpio_sioVoh macros
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_SetVohSel
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_GPIO_GetVohSel(const GPIO_PRT_Type* base, uint32_t pinNum)
{
    CY_ASSERT_L2(CY_GPIO_IS_PIN_VALID(pinNum));

    return (GPIO_PRT_SIO(base) >> (((pinNum & CY_GPIO_SIO_ODD_PIN_MASK) << CY_GPIO_CFG_SIO_OFFSET) + CY_GPIO_VOH_SEL_SHIFT)) & CY_GPIO_VOH_SEL_MASK;
}

/** \} group_gpio_functions_sio */

/**
* \addtogroup group_gpio_functions_interrupt
* \{
*/

/*******************************************************************************
* Function Name: Cy_GPIO_GetInterruptStatus
****************************************************************************//**
*
* Returns the current unmasked interrupt state of the pin.
*
* The core processor's NVIC is triggered by the masked interrupt bits. This
* function allows reading the unmasked interrupt state. Whether the bit
* positions actually trigger the interrupt are defined by the interrupt mask bits.
*
* \param base
* Pointer to the pin's port register base address
*
* \param pinNum
* Position of the pin bit-field within the port register
* Bit position 8 is the routed pin through the port glitch filter.
*
* \return
* 0 = Pin interrupt condition not detected
* 1 = Pin interrupt condition detected
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_GetInterruptStatus
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_GPIO_GetInterruptStatus(const GPIO_PRT_Type* base, uint32_t pinNum)
{
    CY_ASSERT_L2(CY_GPIO_IS_FILTER_PIN_VALID(pinNum));

    return (GPIO_PRT_INTR(base) >> pinNum) & CY_GPIO_INTR_STATUS_MASK;
}


/*******************************************************************************
* Function Name: Cy_GPIO_ClearInterrupt
****************************************************************************//**
*
* Clears the triggered pin interrupt.
*
* \param base
* Pointer to the pin's port register base address
*
* \param pinNum
* Position of the pin bit-field within the port register
* Bit position 8 is the routed pin through the port glitch filter.
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_ClearInterrupt
*
*******************************************************************************/
__STATIC_INLINE void Cy_GPIO_ClearInterrupt(GPIO_PRT_Type* base, uint32_t pinNum)
{
    CY_ASSERT_L2(CY_GPIO_IS_FILTER_PIN_VALID(pinNum));

    /* Any INTR MMIO registers AHB clearing must be preceded with an AHB read access */
    (void)GPIO_PRT_INTR(base);

    GPIO_PRT_INTR(base) = CY_GPIO_INTR_STATUS_MASK << pinNum;

    /* This read ensures that the initial write has been flushed out to the hardware */
    (void)GPIO_PRT_INTR(base);
}


/*******************************************************************************
* Function Name: Cy_GPIO_SetInterruptEdge
****************************************************************************//**
*
* Configures the type of edge that will trigger a pin interrupt.
*
* \param base
* Pointer to the pin's port register base address
*
* \param pinNum
* Position of the pin bit-field within the port register.
* Bit position 8 is the routed pin through the port glitch filter.
*
* \param value
* Pin interrupt mode. Options are detailed in \ref group_gpio_interruptTrigger macros
*
* \note
* This function modifies a port register in a read-modify-write operation. It is
* not thread safe as the resource is shared among multiple pins on a port.
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_SetInterruptEdge
*
*******************************************************************************/
__STATIC_INLINE void Cy_GPIO_SetInterruptEdge(GPIO_PRT_Type* base, uint32_t pinNum, uint32_t value)
{
    uint32_t tempReg;
    uint32_t pinLoc;

    CY_ASSERT_L2(CY_GPIO_IS_FILTER_PIN_VALID(pinNum));
    CY_ASSERT_L2(CY_GPIO_IS_INT_EDGE_VALID(value));

    pinLoc = pinNum << CY_GPIO_INTR_CFG_OFFSET;
    tempReg = GPIO_PRT_INTR_CFG(base) & ~(CY_GPIO_INTR_EDGE_MASK << pinLoc);
    GPIO_PRT_INTR_CFG(base) = tempReg | ((value & CY_GPIO_INTR_EDGE_MASK) << pinLoc);
}


/*******************************************************************************
* Function Name: Cy_GPIO_GetInterruptEdge
****************************************************************************//**
*
* Returns the current pin interrupt edge type.
*
* \param base
* Pointer to the pin's port register base address
*
* \param pinNum
* Position of the pin bit-field within the port register.
* Bit position 8 is the routed pin through the port glitch filter.
*
* \return
* Pin interrupt mode. Options are detailed in \ref group_gpio_interruptTrigger macros
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_SetInterruptEdge
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_GPIO_GetInterruptEdge(const GPIO_PRT_Type* base, uint32_t pinNum)
{
    CY_ASSERT_L2(CY_GPIO_IS_FILTER_PIN_VALID(pinNum));

    return (GPIO_PRT_INTR_CFG(base) >> (pinNum << CY_GPIO_INTR_CFG_OFFSET)) & CY_GPIO_INTR_EDGE_MASK;
}


/*******************************************************************************
* Function Name: Cy_GPIO_SetFilter
****************************************************************************//**
*
* Configures which pin on the port connects to the port-specific glitch filter.
*
* Each port contains a single 50ns glitch filter. Any of the pins on the port
* can be routed to this filter such that the input signal is filtered before
* reaching the edge-detect interrupt circuitry. The state of the filtered pin
* can also be read by calling the Cy_GPIO_Read() function.
*
* \param base
* Pointer to the pin's port register base address
*
* \param value
* The number of the port pin to route to the port filter (0...7)
*
* \note
* This function modifies a port register in a read-modify-write operation. It is
* not thread safe as the resource is shared among multiple pins on a port.
*
* \note
* The filtered pin does not have an associated HSIOM connection. Therefore
* it cannot be routed directly to other peripherals in hardware.
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_SetFilter
*
*******************************************************************************/
__STATIC_INLINE void Cy_GPIO_SetFilter(GPIO_PRT_Type* base, uint32_t value)
{
    uint32_t tempReg;

    CY_ASSERT_L2(CY_GPIO_IS_PIN_VALID(value));

    tempReg = GPIO_PRT_INTR_CFG(base) & ~(CY_GPIO_INTR_FLT_EDGE_MASK << CY_GPIO_INTR_FILT_OFFSET);
    GPIO_PRT_INTR_CFG(base) = tempReg | ((value & CY_GPIO_INTR_FLT_EDGE_MASK) << CY_GPIO_INTR_FILT_OFFSET);
}


/*******************************************************************************
* Function Name: Cy_GPIO_GetFilter
****************************************************************************//**
*
* Returns which pin is currently configured to connect to the port-specific
* glitch filter.
*
* Each port contains a single 50ns glitch filter. Any of the pins on the port
* can be routed to this filter such that the input signal is filtered before
* reaching the edge-detect interrupt circuitry. The state of the filtered pin
* can also be read by calling the Cy_GPIO_Read() function.
*
* \param base
* Pointer to the pin's port register base address
*
* \return
* The number of the port pin routed to the port filter (0...7)
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_SetFilter
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_GPIO_GetFilter(const GPIO_PRT_Type* base)
{
    return ((GPIO_PRT_INTR_CFG(base) >> CY_GPIO_INTR_FILT_OFFSET) & CY_GPIO_INTR_FLT_EDGE_MASK);
}


/*******************************************************************************
* Function Name: Cy_GPIO_GetInterruptCause
****************************************************************************//**
*
* Returns the interrupt status for ports 0 to 31.
*
* \return
* 0 = Interrupt not detected on port
* 1 = Interrupt detected on port
*
* \funcusage
* \snippet gpio/snippet/main.c snippet_Cy_GPIO_GetInterruptCause
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_GPIO_GetInterruptCause(void)
{
    return (GPIO_INTR_CAUSE);
}

/** \} group_gpio_functions_interrupt */

/** \} group_gpio_functions */

#if defined(__cplusplus)
}
#endif

#endif /* CY_GPIO_H */

/** \} group_gpio */

/* [] END OF FILE */
