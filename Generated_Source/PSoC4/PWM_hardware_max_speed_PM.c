/*******************************************************************************
* File Name: PWM_hardware_max_speed_PM.c
* Version 2.0
*
* Description:
*  This file contains the setup, control, and status commands to support
*  the component operations in the low power mode.
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

#include "PWM_hardware_max_speed.h"

static PWM_hardware_max_speed_BACKUP_STRUCT PWM_hardware_max_speed_backup;


/*******************************************************************************
* Function Name: PWM_hardware_max_speed_SaveConfig
********************************************************************************
*
* Summary:
*  All configuration registers are retention. Nothing to save here.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PWM_hardware_max_speed_SaveConfig(void)
{

}


/*******************************************************************************
* Function Name: PWM_hardware_max_speed_Sleep
********************************************************************************
*
* Summary:
*  Stops the component operation and saves the user configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PWM_hardware_max_speed_Sleep(void)
{
    if(0u != (PWM_hardware_max_speed_BLOCK_CONTROL_REG & PWM_hardware_max_speed_MASK))
    {
        PWM_hardware_max_speed_backup.enableState = 1u;
    }
    else
    {
        PWM_hardware_max_speed_backup.enableState = 0u;
    }

    PWM_hardware_max_speed_Stop();
    PWM_hardware_max_speed_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_hardware_max_speed_RestoreConfig
********************************************************************************
*
* Summary:
*  All configuration registers are retention. Nothing to restore here.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PWM_hardware_max_speed_RestoreConfig(void)
{

}


/*******************************************************************************
* Function Name: PWM_hardware_max_speed_Wakeup
********************************************************************************
*
* Summary:
*  Restores the user configuration and restores the enable state.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PWM_hardware_max_speed_Wakeup(void)
{
    PWM_hardware_max_speed_RestoreConfig();

    if(0u != PWM_hardware_max_speed_backup.enableState)
    {
        PWM_hardware_max_speed_Enable();
    }
}


/* [] END OF FILE */
