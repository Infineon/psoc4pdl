/***************************************************************************//**
* \file cy_syspm.c
* \version 1.0
*
* This driver provides the source code for API power management.
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
#include "cy_syspm.h"

/*******************************************************************************
*       Internal Defines
*******************************************************************************/

/* The define for number of callback roots */
#define CALLBACK_ROOT_NR                (5U)

/*******************************************************************************
*       Internal Variables
*******************************************************************************/

/* Array of the callback roots */
static cy_stc_syspm_callback_t* pmCallbackRoot[CALLBACK_ROOT_NR] = {NULL, NULL, NULL, NULL, NULL};

/* The array of the pointers to failed callback */
static cy_stc_syspm_callback_t* failedCallback[CALLBACK_ROOT_NR] = {NULL, NULL, NULL, NULL, NULL};


/*******************************************************************************
* Function Name: Cy_SysPm_CpuEnterSleep
****************************************************************************//**
*
* Sets executing CPU to Sleep mode.
*
* Puts the CPU executing this function into CPU Sleep power mode. If callback 
* functions were registered they are also executed.
*
* For more detail about switching into CPU Sleep power mode and debug,
* refer to the device technical reference manual (TRM).
*
* If at least one callback function with the CY_SYSPM_SLEEP type was registered,
* the following algorithm is executed:
* Prior to entering CPU Sleep mode, all callback functions of the CY_SYSPM_SLEEP
* type with the CY_SYSPM_CHECK_READY parameter are called. This allows the 
* driver to signal whether it is ready to enter the low power mode. If any of 
* the callbacks of the CY_SYSPM_SLEEP type with the CY_SYSPM_CHECK_READY 
* parameter returns CY_SYSPM_FAIL, the remaining callbacks of the 
* CY_SYSPM_SLEEP type with the CY_SYSPM_CHECK_READY parameter are skipped.
* After the first CY_SYSPM_FAIL, all the CY_SYSPM_SLEEP callbacks that were 
* previously executed before getting the CY_SYSPM_CHECK_FAIL are executed with 
* the CY_SYSPM_CHECK_FAIL parameter. The CPU Sleep mode is not entered and the 
* Cy_SysPm_CpuEnterSleep() function returns CY_SYSPM_FAIL.
*
* If all of the callbacks of the CY_SYSPM_SLEEP type with the 
* CY_SYSPM_CHECK_READY parameter return CY_SYSPM_SUCCESS, then all 
* callbacks of the CY_SYSPM_SLEEP type with the CY_SYSPM_CHECK_FAIL parameters 
* calls are skipped. All callbacks of the CY_SYSPM_SLEEP type and then
* CY_SYSPM_BEFORE_TRANSITION parameter calls are executed, allowing the 
* peripherals to prepare for CPU Sleep. The CPU then enters Sleep mode.
* This is a CPU-centric power mode. This means that the CPU has entered Sleep
* mode and its main clock is removed. Any enabled interrupt can cause a CPU 
* wakeup from Sleep mode. 
*
* For multi-core devices, CPU wakeup can also be performed using the Send Event 
* (SEV) assembly instruction executed from the other active CPU. Such wakeup is 
* expected only if the CPU Sleep power mode is done with WFE assembly 
* instruction.
*
* After a wakeup from CPU Sleep, all of the registered callbacks of the 
* CY_SYSPM_SLEEP type and with the CY_SYSPM_AFTER_TRANSITION parameter are 
* executed to return the peripherals to CPU active operation. 
* The Cy_SysPm_CpuEnterSleep() function returns CY_SYSPM_SUCCESS.
* No callbacks of the CY_SYSPM_SLEEP type with the CY_SYSPM_BEFORE_TRANSITION 
* parameter or callbacks of the CY_SYSPM_SLEEP type and 
* CY_SYSPM_AFTER_TRANSITION parameter callbacks are executed if CPU Sleep mode 
* is not entered.
*
* \note The last callback that returns CY_SYSPM_FAIL is not executed with the 
* CY_SYSPM_CHECK_FAIL parameter because of the FAIL. The callback generating 
* CY_SYSPM_FAIL is expected to not make any changes that require being undone.
*
* To support control of callback execution order th following method is 
* implemented. Callback function with the CY_SYSPM_CHECK_READY and 
* CY_SYSPM_BEFORE_TRANSITION parameter are executed in the same order they are 
* registered. Callback function with the CY_SYSPM_CHECK_FAIL and 
* CY_SYSPM_AFTER_TRANSITION parameter are executed in the reverse order they 
* are registered. 

* The return value from executed callback functions with the 
* CY_SYSPM_CHECK_FAIL, CY_SYSPM_BEFORE_TRANSITION, and CY_SYSPM_AFTER_TRANSITION
* modes are ignored.
*
* \ref cy_en_syspm_callback_mode_t, except the CY_SYSPM_CHECK_READY, are ignored
*
* \note The Arm BSD assembly instruction is not required in this function 
* because the function implementation ensures the SLEEPDEEP bit of SCS register 
* is settled prior executing WFI/WFE instruction.
*
* \param waitFor
* Selects wait for action. See \ref cy_en_syspm_waitfor_t.
*
* \return
* Entered status, see \ref cy_en_syspm_status_t.
*
* \sideeffect
* For CY8C6xx6, CY8C6xx7 devices this function clears the Event Register of the 
* CM4 CPU after wakeup from WFE.
*
* \funcusage
* \snippet syspm/snippet/main.c snippet_Cy_SysPm_CpuEnterSleep
*
*******************************************************************************/
cy_en_syspm_status_t Cy_SysPm_CpuEnterSleep(cy_en_syspm_waitfor_t waitFor)
{
    uint32_t interruptState;
    uint32_t cbSleepRootIdx = (uint32_t) CY_SYSPM_SLEEP;
    cy_en_syspm_status_t retVal = CY_SYSPM_SUCCESS;
    
    CY_ASSERT_L3(CY_SYSPM_IS_WAIT_FOR_VALID(waitFor));

    /* Call registered callback functions with CY_SYSPM_CHECK_READY parameter */
    if (pmCallbackRoot[cbSleepRootIdx] != NULL)
    {
        retVal = Cy_SysPm_ExecuteCallback(CY_SYSPM_SLEEP, CY_SYSPM_CHECK_READY);
    }

    /* The CPU can switch into the Sleep power mode only when
    *  all executed registered callback functions with the CY_SYSPM_CHECK_READY 
    *  parameter return CY_SYSPM_SUCCESS.
    */
    if(retVal == CY_SYSPM_SUCCESS)
    {
        /* Call the registered callback functions with 
        * CY_SYSPM_BEFORE_TRANSITION parameter
        */
        interruptState = Cy_SysLib_EnterCriticalSection();
        if (pmCallbackRoot[cbSleepRootIdx] != NULL)
        {
            (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_SLEEP, CY_SYSPM_BEFORE_TRANSITION);
        }

        /* The CPU enters the Sleep power mode upon execution of WFI/WFE */
        SCB_SCR &= (uint32_t) ~SCB_SCR_SLEEPDEEP_Msk;

        if(waitFor != CY_SYSPM_WAIT_FOR_EVENT)
        {
            __WFI();
        }
        else
        {
            __WFE();

        #if (CY_CPU_CORTEX_M4)
            if (Cy_SysLib_GetDevice() == CY_SYSLIB_DEVICE_PSOC6ABLE2)
            {
                /* For the CM4 CPU, the WFE instruction is called twice. 
                *  The second WFE call clears the Event Register of CM4 CPU.
                *  Cypress ID #279077.
                */
                if(wasEventSent)
                {
                    __WFE();
                }

                wasEventSent = true;
            }
        #endif /* (CY_CPU_CORTEX_M4) */
        }
        Cy_SysLib_ExitCriticalSection(interruptState);

        /* Call the registered callback functions with the 
        *  CY_SYSPM_AFTER_TRANSITION parameter
        */
        if (pmCallbackRoot[cbSleepRootIdx] != NULL)
        {
            (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_SLEEP, CY_SYSPM_AFTER_TRANSITION);
        }
    }
    else
    {
        /* Execute callback functions with the CY_SYSPM_CHECK_FAIL parameter to
        *  undo everything done in the callback with the CY_SYSPM_CHECK_READY 
        *  parameter
        */
        (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_SLEEP, CY_SYSPM_CHECK_FAIL);
        retVal = CY_SYSPM_FAIL;
    }
    return retVal;
}


/*******************************************************************************
* Function Name: Cy_SysPm_SystemEnterDeepSleep
****************************************************************************//**
* 
* \warning Not supported in Alpha release
*
* Sets the system into deep sleep power mode. 
*
* Prior to entering the deep sleep mode, all callbacks of the CY_SYSPM_DEEPSLEEP 
* type with the 
* CY_SYSPM_CHECK_READY parameter registered callbacks are called, allowing the 
* driver to signal whether it is ready to enter the power mode. If any 
* CY_SYSPM_DEEPSLEEP type with the CY_SYSPM_CHECK_READY parameter call returns 
* CY_SYSPM_FAIL, the remaining callback CY_SYSPM_DEEPSLEEP type with the 
* CY_SYSPM_CHECK_READY parameter calls are skipped. After a CY_SYSPM_FAIL, all 
* of the callbacks of the CY_SYSPM_DEEPSLEEP type with the CY_SYSPM_CHECK_FAIL 
* parameter are executed that correspond to the callbacks with 
* CY_SYSPM_DEEPSLEEP type with CY_SYSPM_CHECK_READY parameter calls that 
* occurred up to the point of failure. The deep sleep mode is not entered and 
* the Cy_SysPm_SystemEnterDeepSleep() function returns CY_SYSPM_FAIL. If all 
* allbacks of the CY_SYSPM_DEEPSLEEP type with the CY_SYSPM_CHECK_READY 
* parameter calls return CY_SYSPM_SUCCESS, then all callbacks of the 
* CY_SYSPM_DEEPSLEEP type with the CY_SYSPM_CHECK_FAIL parameter calls are 
* skipped and all callbacks of the CY_SYSPM_DEEPSLEEP type with the 
* CY_SYSPM_BEFORE_TRANSITION parameter calls are executed, allowing the 
* peripherals to prepare for deep sleep. The deep sleep mode is then entered. 
* Any enabled interrupt can cause a wakeup from the deep sleep mode. 
* 
* \note The last callback which returned CY_SYSPM_FAIL is not executed with the 
* CY_SYSPM_CHECK_FAIL parameter because of the FAIL. The return values from 
* executed callback functions with the CY_SYSPM_CHECK_FAIL, 
* CY_SYSPM_BEFORE_TRANSITION, and CY_SYSPM_AFTER_TRANSITION modes are ignored.
*  
* Peripherals that do not need a clock or that receive a clock from their 
* external interface (e.g. I2C/SPI) continue operating. Wakeup occurs when an 
* interrupt asserts from an active deep sleep peripheral. For more details, see 
* the corresponding peripheral's datasheet. For more details about switching 
* into the deep sleep power mode and debug, refer to the device TRM. After 
* wakeup from Deep Sleep, all of the registered callbacks with 
* CY_SYSPM_DEEPSLEEP type with CY_SYSPM_AFTER_TRANSITION are executed to return 
* peripherals to Active operation. The Cy_SysPm_SystemEnterDeepSleep() function 
* returns CY_SYSPM_SUCCESS. No callbacks are executed with CY_SYSPM_DEEPSLEEP 
* type with CY_SYSPM_BEFORE_TRANSITION or CY_SYSPM_AFTER_TRANSITION parameter, 
* if deep sleep mode was not entered.  
*
* \param waitFor 
* Selects wait for action. See \ref cy_en_syspm_waitfor_t.
*
* \return
* Entered status, see \ref cy_en_syspm_status_t.
*
*******************************************************************************/
cy_en_syspm_status_t Cy_SysPm_SystemEnterDeepSleep(cy_en_syspm_waitfor_t waitFor)
{
    uint32_t interruptState;
    uint32_t cbDeepSleepRootIdx = (uint32_t) CY_SYSPM_DEEPSLEEP;
    cy_en_syspm_status_t retVal = CY_SYSPM_SUCCESS;

    CY_ASSERT_L3(CY_SYSPM_IS_WAIT_FOR_VALID(waitFor));

    /* Call the registered callback functions with the CY_SYSPM_CHECK_READY 
    *  parameter
    */
    if (pmCallbackRoot[cbDeepSleepRootIdx] != NULL)
    {
        retVal = Cy_SysPm_ExecuteCallback(CY_SYSPM_DEEPSLEEP, CY_SYSPM_CHECK_READY);
    }

    /* The CPU can switch into the Deep Sleep power mode only when
    *  all executed registered callback functions with the CY_SYSPM_CHECK_READY
    *  parameter return CY_SYSPM_SUCCESS
    */
    if (retVal == CY_SYSPM_SUCCESS)
    {
        /* System Deep Sleep indicator */
        //bool wasSystemDeepSleep = false;
        
        /* Call the registered callback functions with the 
        * CY_SYSPM_BEFORE_TRANSITION parameter
        */
        interruptState = Cy_SysLib_EnterCriticalSection();
        if (pmCallbackRoot[cbDeepSleepRootIdx] != NULL)
        {
            (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_DEEPSLEEP, CY_SYSPM_BEFORE_TRANSITION);
        }

        /* Adjust delay to wait for references to settle on wakeup from Deep Sleep */
        SRSSLT_PWR_KEY_DELAY = SFLASH_DPSLP_KEY_DELAY;

        /* The CPU enters Deep Sleep mode upon execution of WFI/WFE */
        SCB_SCR |= SCB_SCR_SLEEPDEEP_Msk;

        if(waitFor != CY_SYSPM_WAIT_FOR_EVENT)
        {
            __WFI();
        }
        else
        {
            __WFE();
        }

        Cy_SysLib_ExitCriticalSection(interruptState);
    }
    
    if (retVal == CY_SYSPM_SUCCESS)
    {
        /* Call the registered callback functions with the CY_SYSPM_AFTER_TRANSITION 
        *  parameter
        */
        if (pmCallbackRoot[cbDeepSleepRootIdx] != NULL)
        {
            (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_DEEPSLEEP, CY_SYSPM_AFTER_TRANSITION);
        }  
    }
    else 
    {
        /* Execute callback functions with the CY_SYSPM_CHECK_FAIL parameter to 
        *  undo everything done in the callback with the CY_SYSPM_CHECK_READY 
        *  parameter
        */
        if (pmCallbackRoot[cbDeepSleepRootIdx] != NULL)
        {
            (void) Cy_SysPm_ExecuteCallback(CY_SYSPM_DEEPSLEEP, CY_SYSPM_CHECK_FAIL);
        }
        
        /* Rewrite return value to indicate fail */
        if (retVal != CY_SYSPM_SYSCALL_PENDING)
        {
            retVal = CY_SYSPM_FAIL;  
        }
    }
    return retVal;
}


/*******************************************************************************
* Function Name: Cy_SysPm_RegisterCallback
****************************************************************************//**
*
* \warning Not supported in Alpha release
* 
* Sets the system into hibernate power mode. 
* Prior to entering hibernate mode, all callbacks of the CY_SYSPM_HIBERNATE type 
* are executed. First, callbacks of the CY_SYSPM_HIBERNATE type and with 
* CY_SYSPM_CHECK_READY parameter are called allowing the driver to signal if it 
* is not ready to enter the power mode. If any of the callbacks of the 
* CY_SYSPM_HIBERNATE type with the CY_SYSPM_CHECK_READY parameter call returns 
* CY_SYSPM_FAIL, the remaining CY_SYSPM_HIBERNATE callbacks with the 
* CY_SYSPM_CHECK_READY parameter calls are skipped. After CY_SYSPM_FAIL, all of 
* the CY_SYSPM_HIBERNATE callbacks with CY_SYSPM_CHECK_FAIL parameter are 
* executed that correspond to the CY_SYSPM_HIBERNATE callbacks with 
* CY_SYSPM_CHECK_READY parameter calls that occurred up to the point of failure. 
* The hibernate mode is not entered and the Cy_SysPm_EnterHibernate() function 
* returns CY_SYSPM_FAIL. If all CY_SYSPM_HIBERNATE callbacks with the 
* CY_SYSPM_CHECK_READY parameter calls return CY_SYSPM_SUCCESS, then all 
* CY_SYSPM_HIBERNATE callbacks with CY_SYSPM_CHECK_FAIL calls are skipped and 
* all CY_SYSPM_HIBERNATE callbacks CY_SYSPM_BEFORE_TRANSITION parameter calls 
8 are executed allowing the peripherals to prepare for hibernate. All of the 
* I/O output states are frozen and the hibernate mode is then entered. In 
* hibernate mode, all internal supplies are off and only SRAM and UDB state is 
* retained. 
* 
* \note The last callback which returned CY_SYSPM_FAIL is not executed with the 
* CY_SYSPM_CHECK_FAIL parameter because of the FAIL. The return values from 
* executed callback functions with the CY_SYSPM_CHECK_FAIL, 
* CY_SYSPM_BEFORE_TRANSITION, and CY_SYSPM_AFTER_TRANSITION modes are ignored. 
* 
* A wakeup from hibernate is triggered by toggling any GPIO pin interrupt, the 
* wakeup pin, or a low-power comparator. A wakeup causes a normal boot procedure. 
* To distinguish a wakeup from hibernate mode and a general reset event, the 
* Cy_SysLib_GetResetReason() function can be used. The GPIO pin interrupt or 
* low-power comparator's polarity can be changed with the 
* Cy_SysPm_SetHibernateWakeupSource function. This function call will not return 
* if hibernate mode is entered. The CY_SYSPM_HIBERNATE callbacks with the 
* CY_SYSPM_AFTER_TRANSITION parameter are never executed. This function freezes 
* the IO cells. The IO cells remain frozen after waking from hibernate mode 
* until the firmware unfreezes them with a Cy_SysPm_IoUnfreeze() function call. 
*
* * \return Entered status, see cy_en_syspm_status_t.
*
*******************************************************************************/
cy_en_syspm_status_t Cy_SysPm_SystemEnterHibernate(void)
{
    cy_en_syspm_status_t retVal = CY_SYSPM_FAIL;

    return retVal;
}


/*******************************************************************************
* Function Name: Cy_SysPm_RegisterCallback
****************************************************************************//**
*
* \warning Not supported in Alpha release
* 
* Registers a new syspm callback.
*
* A callback is a function called after an event in the driver or
* middleware module has occurred. The handler callback API will be executed if
* the specific event occurs. SysPm callbacks are called when changing power 
* modes. See \ref cy_stc_syspm_callback_t.
*
* \note The registered callbacks are executed in two orders, based on callback 
* mode \ref cy_en_syspm_callback_mode_t. For modes CY_SYSPM_CHECK_READY and 
* CY_SYSPM_BEFORE_TRANSITION, the order is same order as callbacks were 
* registered.
* For modes CY_SYSPM_AFTER_TRANSITION and CY_SYSPM_CHECK_FAIL, the order is 
* reverse as the order callbacks were registered.
*
* \param handler
* The address of the syspm callback structure.
* See \ref cy_stc_syspm_callback_t.
*
* \return
* - True if a callback was registered.
* - False if a callback was not registered.
*
* \note Do not modify the registered structure in run-time.
* \warning After being registered, the SysPm callback structures must be 
* allocated during power mode transition.
*
*******************************************************************************/
bool Cy_SysPm_RegisterCallback(cy_stc_syspm_callback_t* handler)
{
    bool retVal = false;

    /* Verify the input parameters. */
    if ((handler != NULL) && (handler->callbackParams != NULL) && (handler->callback != NULL))
    {
        uint32_t callbackRootIdx = (uint32_t) handler->type;

        /* If the callback list is not empty. */
        if (pmCallbackRoot[callbackRootIdx] != NULL)
        {
            cy_stc_syspm_callback_t* curCallback = pmCallbackRoot[callbackRootIdx];
            cy_stc_syspm_callback_t* insertPos  = curCallback;

            /* Find the callback after which the new callback is to be
             * inserted. Ensure the given callback has not been registered.
             */
            while ((NULL != curCallback->nextItm) && (curCallback != handler))
            {
                curCallback = curCallback->nextItm;
                /* Callbacks with the same order value are stored in the order
                 * they are registered.
                 */
                if (curCallback->order <= handler->order)
                {
                    insertPos = curCallback;
                }
            }
            /* If the callback has not been registered. */
            if (curCallback != handler)
            {
                /* If the callback is to be inserted at the beginning of the list. */
                if ((insertPos->prevItm == NULL) && (handler->order < insertPos->order))
                {
                    handler->nextItm = insertPos;
                    handler->prevItm = NULL;
                    handler->nextItm->prevItm = handler;
                    pmCallbackRoot[callbackRootIdx] = handler;
                }
                else
                {
                    handler->nextItm = insertPos->nextItm;
                    handler->prevItm = insertPos;

                    /* If the callback is not inserted at the end of the list. */
                    if (handler->nextItm != NULL)
                    {
                        handler->nextItm->prevItm = handler;
                    }
                    insertPos->nextItm = handler;
                }
                retVal = true;
            }
        }
        else
        {
            /* The callback list is empty. */
            pmCallbackRoot[callbackRootIdx] = handler;
            handler->nextItm = NULL;
            handler->prevItm = NULL;
            retVal = true;
        }
    }
    return retVal;
}


/*******************************************************************************
* Function Name: Cy_SysPm_UnregisterCallback
****************************************************************************//**
*
* This function unregisters a callback.
*
* The registered callback can be unregistered and the function returns true. 
* Otherwise, false is returned.
*
* \param handler The item that should be unregistered.
* See \ref cy_stc_syspm_callback_t.
*
* \return
* - True if callback was unregistered.
* - False if it was not unregistered or no callbacks are registered.
*
* \funcusage
* \snippet syspm/snippet/main.c snippet_Cy_SysPm_UnregisterCallback
*
*******************************************************************************/
bool Cy_SysPm_UnregisterCallback(cy_stc_syspm_callback_t const *handler)
{
    bool retVal = false;

    if (handler != NULL)
    {
        uint32_t callbackRootIdx = (uint32_t) handler->type;
        cy_stc_syspm_callback_t* curCallback = pmCallbackRoot[callbackRootIdx];
        
        /* Search requested callback item in the linked list */
        while (curCallback != NULL)
        {
            /* Requested callback is found */
            if (curCallback == handler)
            {
                retVal = true;
                break;
            }
            
            /* Go to next callback item in the linked list */
            curCallback = curCallback->nextItm;
        }
        
        if (retVal)
        {
            /* Requested callback is first in the list */
            if (pmCallbackRoot[callbackRootIdx] == handler)
            {
                /* Check whether this the only callback registered */
                if (pmCallbackRoot[callbackRootIdx]->nextItm != NULL)
                {
                    pmCallbackRoot[callbackRootIdx] = pmCallbackRoot[callbackRootIdx]->nextItm;
                    pmCallbackRoot[callbackRootIdx]->prevItm = NULL;
                }
                else
                {
                    /* We had only one callback */
                    pmCallbackRoot[callbackRootIdx] = NULL;
                }
            }
            else
            {
                /* Update links of related to unregistered callback items */
                curCallback->prevItm->nextItm = curCallback->nextItm;
                
                if (curCallback->nextItm != NULL)
                {
                    curCallback->nextItm->prevItm = curCallback->prevItm;
                }
            }
        }
    }

    return retVal;
}


/*******************************************************************************
* Function Name: Cy_SysPm_ExecuteCallback
****************************************************************************//**
*
* The function executes all registered callbacks with provided type and mode.
*
* \note The registered callbacks will be executed in order based on 
* \ref cy_en_syspm_callback_type_t value. There are two possible callback 
* execution orders:
* * From first registered to last registered. This order applies to 
*   callbacks with mode CY_SYSPM_CHECK_READY and CY_SYSPM_BEFORE_TRANSITION.
* * Backward flow execution: 
*   - From last registered to the first registered. This order applies 
*     to callbacks with mode CY_SYSPM_AFTER_TRANSITION.
*   - From last called to the first registered callback. This order applies 
*     to callbacks with mode CY_SYSPM_CHECK_FAIL. Note that, the last called 
*     callback function  that generated the CY_SYSPM_CHECK_FAIL is skipped when 
*     mode CY_SYSPM_CHECK_FAIL. This is because the callback that returns 
*     CY_SYSPM_FAIL already knows that it failed and will not take any action 
*     that requires correction.
*
* If no callbacks are registered, returns CY_SYSPM_SUCCESS.
*
* \param type
* The callback type. See \ref cy_en_syspm_callback_type_t.
*
* \param mode
* The callback mode. See \ref cy_en_syspm_callback_mode_t.
*
* \return
* - CY_SYSPM_SUCCESS if callback successfully completed or nor callbacks 
*   registered.
* - CY_SYSPM_FAIL one of the executed callback(s) returned fail.
*
* \funcusage
* \snippet syspm/snippet/main.c snippet_Cy_SysPm_ExecuteCallback
*
*******************************************************************************/
cy_en_syspm_status_t Cy_SysPm_ExecuteCallback(cy_en_syspm_callback_type_t type, cy_en_syspm_callback_mode_t mode)
{
    CY_ASSERT_L3(CY_SYSPM_IS_CALLBACK_TYPE_VALID(type));
    CY_ASSERT_L3(CY_SYSPM_IS_CALLBACK_MODE_VALID(mode));
    
    static cy_stc_syspm_callback_t* lastExecutedCallback = NULL;
    cy_en_syspm_status_t retVal = CY_SYSPM_SUCCESS;
    cy_stc_syspm_callback_t* curCallback = pmCallbackRoot[(uint32_t) type];
    cy_stc_syspm_callback_params_t curParams;
    
    if ((mode == CY_SYSPM_BEFORE_TRANSITION) || (mode == CY_SYSPM_CHECK_READY))
    {
        /* Execute registered callbacks with order from first registered to the
        *  last registered. Stop executing if CY_SYSPM_FAIL was returned in 
        *  CY_SYSPM_CHECK_READY mode
        */
        while ((curCallback != NULL) && ((retVal != CY_SYSPM_FAIL) || (mode != CY_SYSPM_CHECK_READY)))
        {
            /* The modes defined in the .skipMode element are not executed */
            if (0UL == ((uint32_t) mode & curCallback->skipMode))
            {
                /* Update elements for local callback parameter values */
                curParams.base = curCallback->callbackParams->base;
                curParams.context = curCallback->callbackParams->context;

                retVal = curCallback->callback(&curParams, mode);
                
                /* Update callback pointer with value of executed callback. 
                * Such update is required to execute further callbacks in 
                * backward order after exit from LP mode or to undo 
                * configuration after callback returned fail: from last called
                * to first registered.
                */
                lastExecutedCallback = curCallback;
            }
            curCallback = curCallback->nextItm;
        }

        if (mode == CY_SYSPM_CHECK_READY)
        {
            /* Update the pointer to  the failed callback with the result of the callback execution.
            *  If the callback fails, the value of the pointer will be updated
            *  with the address of the callback which returned CY_SYSPM_FAIL, else,
            *  it will be updated with NULL.
            */
            if(retVal == CY_SYSPM_FAIL)
            {
                failedCallback[(uint32_t) type] = lastExecutedCallback;
            }
            else
            {
                failedCallback[(uint32_t) type] = NULL;
            }
        }
    }
    else
    {
        /* Execute registered callbacks with order from lastCallback or last 
        * executed to the first registered callback. Such a flow is required if 
        * a previous callback function returned CY_SYSPM_FAIL or a previous 
        * callback mode was CY_SYSPM_BEFORE_TRANSITION. Such an order is 
        * required to undo configurations in correct backward order.
        */
        if (mode != CY_SYSPM_CHECK_FAIL)
        {
            while (curCallback->nextItm != NULL)
            {
                curCallback = curCallback->nextItm;
            }
        }
        else
        {
            /* Skip last executed callback that returns CY_SYSPM_FAIL, as this 
            *  callback already knows that it failed.
            */
            curCallback = lastExecutedCallback;

            if (curCallback != NULL)
            {
                curCallback = curCallback->prevItm;
            }
        }

        /* Execute callback functions with required type and mode */
        while (curCallback != NULL)
        {
            /* The modes defined in the .skipMode element are not executed */
            if (0UL == ((uint32_t) mode & curCallback->skipMode))
            {
                /* Update elements for local callback parameter values */
                curParams.base = curCallback->callbackParams->base;
                curParams.context = curCallback->callbackParams->context;

                retVal = curCallback->callback(&curParams, mode);
            }
            curCallback = curCallback->prevItm;
        }
    }

    return retVal;
}


/*******************************************************************************
* Function Name: Cy_SysPm_GetFailedCallback
****************************************************************************//**
* 
* Reads the result of the callback execution after the power mode functions
* execution.
*
* This function reads the value of the pointer that stores the result of callback
* execution. It takes power mode as the parameter and returns the address of the
* callback configuration structure in the case of failure or NULL in the case of
* success. This address of the failed callback allows finding the callback that
* blocks entering power mode.
*
* \param type
* Power mode for which a callback execution result is required.
*
* \return
* - The address of the callback configuration structure if the callback handler
* function failed.
* - NULL if the callback skipped or executed successfully.
*
* \funcusage
* \snippet syspm/snippet/main.c snippet_Cy_SysPm_GetFailedCallback
*
*******************************************************************************/
cy_stc_syspm_callback_t* Cy_SysPm_GetFailedCallback(cy_en_syspm_callback_type_t type)
{
    return failedCallback[(uint32_t) type];
}

/* [] END OF FILE */
