/*******************************************************************************
* File Name: LPComp_movement_detector.c
* Version 2.0
*
* Description:
*  This file provides the source code to the API for the Low Power Comparator
*  component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2013-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "LPComp_movement_detector.h"
#include "CyLib.h"
#include "cyPm.h"

uint8 LPComp_movement_detector_initVar = 0u;
static uint32 LPComp_movement_detector_internalIntr = 0u;
#if(LPComp_movement_detector_CY_LPCOMP_V2)
    /* This variable saves INTR_MASK register since it get reset after wakeup 
    * from Hibernate. Attribute CY_NOINIT puts SRAM variable in memory section 
    * which is retained in low power modes 
    */
    CY_NOINIT static uint32 LPComp_movement_detector_intrMaskRegState;
#endif /* (LPComp_movement_detector_CY_LPCOMP_V2) */


/*******************************************************************************
* Function Name: LPComp_movement_detector_Start
********************************************************************************
*
* Summary:
*  Performs all of the required initialization for the component and enables
*  power to the block. The first time the routine is executed, the component is
*  initialized to the configuration from the customizer. When called to restart
*  the comparator following a LPComp_movement_detector_Stop() call, the current
*  component parameter settings are retained.
*
* Parameters:
*   None
*
* Return:
*  None
*
* Global variables:
*  LPComp_movement_detector_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void LPComp_movement_detector_Start(void)
{
    if(0u == LPComp_movement_detector_initVar)
    {
        LPComp_movement_detector_Init();
        LPComp_movement_detector_initVar = 1u;
    }
    LPComp_movement_detector_Enable();
}


/*******************************************************************************
* Function Name: LPComp_movement_detector_Init
********************************************************************************
*
* Summary:
*  Initializes or restores the component according to the customizer settings.
*  It is not necessary to call LPComp_movement_detector_Init() because the
*  LPComp_movement_detector_Start() API calls this function and is the preferred method
*  to begin component operation.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void LPComp_movement_detector_Init(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    LPComp_movement_detector_CONFIG_REG |= LPComp_movement_detector_CONFIG_REG_DEFAULT;
    
    #if (LPComp_movement_detector_CY_LPCOMP_V2)
        if (CySysPmGetResetReason() == CY_PM_RESET_REASON_WAKEUP_HIB)
        {
            /* Restore the INTR_MASK register state since it get reset after 
            * wakeup from Hibernate.  
            */
            LPComp_movement_detector_INTR_MASK_REG |= LPComp_movement_detector_intrMaskRegState;  
        }
        else
        {
            LPComp_movement_detector_INTR_MASK_REG |= LPComp_movement_detector_INTR_MASK_REG_DEFAULT;    
            LPComp_movement_detector_intrMaskRegState = LPComp_movement_detector_INTR_MASK_REG;
        }
    #endif /* (LPComp_movement_detector_CY_LPCOMP_V2) */
    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: LPComp_movement_detector_Enable
********************************************************************************
*
* Summary:
*  Activates the hardware and begins component operation. It is not necessary
*  to call LPComp_movement_detector_Enable() because the LPComp_movement_detector_Start() API
*  calls this function, which is the preferred method to begin component
*  operation.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void LPComp_movement_detector_Enable(void)
{
    uint8 enableInterrupts;
    
    enableInterrupts = CyEnterCriticalSection();
    LPComp_movement_detector_CONFIG_REG |= LPComp_movement_detector_CONFIG_EN;
    
    /*******************************************************
    * When the Enable() API is called the CONFIG_EN bit is set. 
    * This can cause fake interrupt because of the output delay 
    * of the analog. This requires setting the CONFIG_INTR bits 
    * after the CONFIG_EN bit is set.
    *******************************************************/
    LPComp_movement_detector_CONFIG_REG |= LPComp_movement_detector_internalIntr;
    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: LPComp_movement_detector_Stop
********************************************************************************
*
* Summary:
*  Turns off the LP Comparator.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  The function doesn't change component Speed settings.
*
*******************************************************************************/
void LPComp_movement_detector_Stop(void)
{
    uint8 enableInterrupts;
    
    enableInterrupts = CyEnterCriticalSection();
    #if (LPComp_movement_detector_CY_LPCOMP_V0)
        /*******************************************************
        * When the Stop() API is called the CONFIG_EN bit is 
        * cleared. This causes the output of the comparator to go 
        * low. If the comparator output was high and the Interrupt 
        * Configuration is set for Falling edge disabling the 
        * comparator will cause an fake interrupt. This requires 
        * to clear the CONFIG_INTR bits before the CONFIG_EN bit 
        * is cleared.
        *******************************************************/
        LPComp_movement_detector_CONFIG_REG &= (uint32)~LPComp_movement_detector_CONFIG_INTR_MASK;
    #endif /* (LPComp_movement_detector_CY_LPCOMP_V0) */

    LPComp_movement_detector_CONFIG_REG &= (uint32)~LPComp_movement_detector_CONFIG_EN;
    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: LPComp_movement_detector_SetSpeed
*
* Summary:
*  Sets the drive power and speed to one of three settings.
*
* Parameters:
*  uint32 speed: Sets operation mode of the component:
*   LPComp_movement_detector_LOW_SPEED  - Slow/ultra low 
*   LPComp_movement_detector_MED_SPEED  - Medium/low 
*   LPComp_movement_detector_HIGH_SPEED - Fast/normal 
*
* Return:
*  None
*
*******************************************************************************/
void LPComp_movement_detector_SetSpeed(uint32 speed)
{
    uint32 config;
    uint8 enableInterrupts;
    
    enableInterrupts = CyEnterCriticalSection();
    config = LPComp_movement_detector_CONFIG_REG & (uint32)~LPComp_movement_detector_CONFIG_SPEED_MODE_MASK;
    LPComp_movement_detector_CONFIG_REG = config | LPComp_movement_detector_GET_CONFIG_SPEED_MODE(speed);
    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: LPComp_movement_detector_SetInterruptMode
********************************************************************************
*
* Summary:
*  Sets the interrupt edge detect mode. This also controls the value provided
*  on the output.
*
* Parameters:
*  uint32 mode: Enumerated edge detect mode value:
*  LPComp_movement_detector_INTR_DISABLE - Disable
*  LPComp_movement_detector_INTR_RISING  - Rising edge detect
*  LPComp_movement_detector_INTR_FALLING - Falling edge detect
*  LPComp_movement_detector_INTR_BOTH    - Detect both edges
*
* Return:
*  None
*
*******************************************************************************/
void LPComp_movement_detector_SetInterruptMode(uint32 mode)
{
    uint32 config;
    uint8 enableInterrupts;
    
    LPComp_movement_detector_internalIntr = LPComp_movement_detector_GET_CONFIG_INTR(mode);

    enableInterrupts = CyEnterCriticalSection();
    config = LPComp_movement_detector_CONFIG_REG & (uint32)~LPComp_movement_detector_CONFIG_INTR_MASK;
    LPComp_movement_detector_CONFIG_REG = config | LPComp_movement_detector_internalIntr;
    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: LPComp_movement_detector_GetInterruptSource
********************************************************************************
*
* Summary:
*  Gets the interrupt requests. This function is for use when using the combined
*  interrupt signal from the global signal reference. This function from either
*  component instance can be used to determine the interrupt source for both the
*  interrupts combined.
*
* Parameters:
*  None
*
* Return:
*  uint32: Interrupt source. Each component instance has a mask value:
*  LPComp_movement_detector_INTR.
*
*******************************************************************************/
uint32 LPComp_movement_detector_GetInterruptSource(void)
{
    return (LPComp_movement_detector_INTR_REG); 
}


/*******************************************************************************
* Function Name: LPComp_movement_detector_ClearInterrupt
********************************************************************************
*
* Summary:
*  Clears the interrupt request. This function is for use when using the
*  combined interrupt signal from the global signal reference. This function
*  from either component instance can be used to clear either or both
*  interrupts.
*
* Parameters:
*  uint32 interruptMask: Mask of interrupts to clear. Each component instance
*  has a mask value: LPComp_movement_detector_INTR.
*
* Return:
*  None
*
*******************************************************************************/
void LPComp_movement_detector_ClearInterrupt(uint32 interruptMask)
{   
    LPComp_movement_detector_INTR_REG = interruptMask;
}


/*******************************************************************************
* Function Name: LPComp_movement_detector_SetInterrupt
********************************************************************************
*
* Summary:
*  Sets a software interrupt request. This function is for use when using the
*  combined interrupt signal from the global signal reference. This function
*  from either component instance can be used to trigger either or both software
*  interrupts.
*
* Parameters:
*  uint32 interruptMask: Mask of interrupts to set. Each component instance has
*  a mask value: LPComp_movement_detector_INTR_SET.
*
* Return:
*  None
*
*******************************************************************************/
void LPComp_movement_detector_SetInterrupt(uint32 interruptMask)
{
    uint8 enableInterrupts;
    
    enableInterrupts = CyEnterCriticalSection();
    LPComp_movement_detector_INTR_SET_REG |= interruptMask;
    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: LPComp_movement_detector_SetHysteresis
********************************************************************************
*
* Summary:
*  Enables or disables the hysteresis.
*
* Parameters:
*  hysteresis: (uint32) Enables or disables the hysteresis setting.
*  LPComp_movement_detector_HYST_ENABLE     - Enable hysteresis
*  LPComp_movement_detector_HYST_DISABLE    - Disable hysteresis
*
* Return:
*  None
*
*******************************************************************************/
void LPComp_movement_detector_SetHysteresis(uint32 hysteresis)
{
    uint8 enableInterrupts;
    
    enableInterrupts = CyEnterCriticalSection();
    if(0u != hysteresis)
    {
        LPComp_movement_detector_CONFIG_REG |= LPComp_movement_detector_CONFIG_HYST;
    }
    else
    {
        LPComp_movement_detector_CONFIG_REG &= (uint32)~LPComp_movement_detector_CONFIG_HYST;
    }
    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: LPComp_movement_detector_SetFilter
********************************************************************************
*
* Summary:
*  Enables or disables the digital filter. This still exists for saving backward 
*  compatibility, but not recommended for using.
*  This API is DEPRECATED and should not be used in new projects.
*
* Parameters:
*  uint32 filter: filter enable.
*  LPComp_movement_detector_FILT_ENABLE  - Enable filter
*  LPComp_movement_detector_FILT_DISABLE - Disable filter
*
* Return:
*  None
*
*******************************************************************************/
void LPComp_movement_detector_SetFilter(uint32 filter)
{
    uint8 enableInterrupts;
    
    enableInterrupts = CyEnterCriticalSection();
    if( 0u != filter)
    {
        LPComp_movement_detector_CONFIG_REG |= LPComp_movement_detector_CONFIG_FILT;
    }
    else
    {
        LPComp_movement_detector_CONFIG_REG &= (uint32)~LPComp_movement_detector_CONFIG_FILT;
    }
    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: LPComp_movement_detector_GetCompare
********************************************************************************
*
* Summary:
*  This function returns a nonzero value when the voltage connected to the
*  positive input is greater than the negative input voltage.
*  This function reads the direct (unflopped) comparator output which can also be 
*  metastable (since it may result in incorrect data).
*
* Parameters:
*  None
*
* Return:
*  (uint32) Comparator output state. This value is not impacted by the interrupt
*  edge detect setting:
*   0 - if positive input is less than negative one.
*   1 - if positive input greater than negative one.
*
*******************************************************************************/
uint32 LPComp_movement_detector_GetCompare(void)
{
    return((uint32)((0u == (LPComp_movement_detector_CONFIG_REG & LPComp_movement_detector_CONFIG_OUT)) ? 0u : 1u));
}


/*******************************************************************************
* Function Name: LPComp_movement_detector_ZeroCal
********************************************************************************
*
* Summary:
*  Performs custom calibration of the input offset to minimize error for a
*  specific set of conditions: comparator reference voltage, supply voltage,
*  and operating temperature. A reference voltage in the range at which the
*  comparator will be used must be applied to one of the inputs. The two inputs
*  will be shorted internally to perform the offset calibration.
*
* Parameters:
*  None
*
* Return:
*  The value from the comparator trim register after the offset calibration is
*  complete. This value has the same format as the input parameter for the
*  LPComp_movement_detector_LoadTrim() API routine.
*
*******************************************************************************/
uint32 LPComp_movement_detector_ZeroCal(void)
{
    uint32 cmpOut;
    uint32 i;

    LPComp_movement_detector_DFT_REG |= LPComp_movement_detector_DFT_CAL_EN;
    LPComp_movement_detector_TRIMA_REG = 0u;
    LPComp_movement_detector_TRIMB_REG = 0u;

    CyDelayUs(1u);

    cmpOut = LPComp_movement_detector_GetCompare();

    if(0u != cmpOut)
    {
        LPComp_movement_detector_TRIMA_REG = ((uint32) 0x01u << LPComp_movement_detector_TRIMA_SIGNBIT);
    }

    for(i = (LPComp_movement_detector_TRIMA_MAX_VALUE +1u ); i != 0u; i--)
    {
        LPComp_movement_detector_TRIMA_REG++;
        CyDelayUs(1u);
        if(cmpOut != LPComp_movement_detector_GetCompare())
        {
            break;
        }
    }

    if(((uint32)(LPComp_movement_detector_CONFIG_REG >> LPComp_movement_detector_CONFIG_REG_SHIFT) &
        LPComp_movement_detector_SPEED_PARAM_MASK) == LPComp_movement_detector_MED_SPEED)
    {
        cmpOut = LPComp_movement_detector_GetCompare();

        if(0u == cmpOut)
        {
            LPComp_movement_detector_TRIMB_REG = ((uint32)1u << LPComp_movement_detector_TRIMB_SIGNBIT);
        }

        for(i = (LPComp_movement_detector_TRIMB_MAX_VALUE +1u ); i != 0u; i--)
        {
            LPComp_movement_detector_TRIMB_REG++;
            CyDelayUs(1u);
            if(cmpOut != LPComp_movement_detector_GetCompare())
            {
                break;
            }
        }
    }

    LPComp_movement_detector_DFT_REG &= ((uint32)~LPComp_movement_detector_DFT_CAL_EN);

    return (LPComp_movement_detector_TRIMA_REG | ((uint32)LPComp_movement_detector_TRIMB_REG << LPComp_movement_detector_TRIMB_SHIFT));
}


/*******************************************************************************
* Function Name: LPComp_movement_detector_LoadTrim
********************************************************************************
*
* Summary:
*  This function writes a value into the comparator offset trim register.
*
* Parameters:
*  trimVal: Value to be stored in the comparator offset trim register. This
*  value has the same format as the parameter returned by the
*  LPComp_movement_detector_ZeroCal() API routine.
*
* Return:
*  None
*
*******************************************************************************/
void LPComp_movement_detector_LoadTrim(uint32 trimVal)
{
    LPComp_movement_detector_TRIMA_REG = (trimVal & LPComp_movement_detector_TRIMA_MASK);
    LPComp_movement_detector_TRIMB_REG = ((trimVal >> LPComp_movement_detector_TRIMB_SHIFT) & LPComp_movement_detector_TRIMB_MASK);
}


#if (LPComp_movement_detector_CY_LPCOMP_V2)
    
/*******************************************************************************
* Function Name: LPComp_movement_detector_SetOutputMode
********************************************************************************
*
* Summary:
*  Set comparator output mode. 
*
* Parameters:
*  uint32 mode: Comparator output mode value
*  LPComp_movement_detector_OUT_DIRECT - Direct output
*  LPComp_movement_detector_OUT_SYNC   - Synchronized output
*  LPComp_movement_detector_OUT_PULSE  - Pulse output
*
* Return:
*  None
*
*******************************************************************************/
void LPComp_movement_detector_SetOutputMode(uint32 mode)
{
    uint32 config;
    uint8 enableInterrupts;
    
    enableInterrupts = CyEnterCriticalSection();
    config = LPComp_movement_detector_CONFIG_REG & ((uint32)~(LPComp_movement_detector_CONFIG_DSI_BYPASS | \
                                                     LPComp_movement_detector_CONFIG_DSI_LEVEL));
    LPComp_movement_detector_CONFIG_REG = config | LPComp_movement_detector_GET_CONFIG_DSI_BYPASS(mode) | \
                                           LPComp_movement_detector_GET_CONFIG_DSI_LEVEL(mode);
    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: LPComp_movement_detector_SetInterruptMask
********************************************************************************
*
* Summary:
*  Configures which bits of interrupt request register will trigger an interrupt 
*  event.
*
* Parameters:
*  uint32 interruptMask: Mask of interrupts to set. Each component instance has
*  a mask value: LPComp_movement_detector_INTR_MASK.
*
* Return:
*  None
*
*******************************************************************************/
void LPComp_movement_detector_SetInterruptMask(uint32 interruptMask)
{
    LPComp_movement_detector_intrMaskRegState = interruptMask;
    LPComp_movement_detector_INTR_MASK_REG = LPComp_movement_detector_intrMaskRegState;
}


/*******************************************************************************
* Function Name: LPComp_movement_detector_GetInterruptMask
********************************************************************************
*
* Summary:
*  Returns interrupt mask.
*
* Parameters:
*  None
*
* Return:
*  uint32:  Mask of enabled interrupt source. Each component instance 
*  has a mask value: LPComp_movement_detector_INTR_MASK.
*
*******************************************************************************/
uint32 LPComp_movement_detector_GetInterruptMask(void)
{
    return (LPComp_movement_detector_INTR_MASK_REG);
}


/*******************************************************************************
* Function Name: LPComp_movement_detector_GetInterruptSourceMasked
********************************************************************************
*
* Summary:
*  Returns interrupt request register masked by interrupt mask. Returns the result 
*  of bitwise AND operation between corresponding interrupt request and mask bits.
*
* Parameters:
*  None
*
* Return:
*  uint32: Status of enabled interrupt source. Each component instance 
*  has a mask value: LPComp_movement_detector_INTR_MASKED.
*
*******************************************************************************/
uint32 LPComp_movement_detector_GetInterruptSourceMasked(void)
{
    return (LPComp_movement_detector_INTR_MASKED_REG);
}

#endif /* (LPComp_movement_detector_CY_LPCOMP_V2) */


/* [] END OF FILE */
