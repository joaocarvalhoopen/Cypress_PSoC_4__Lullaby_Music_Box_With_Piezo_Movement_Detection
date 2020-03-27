/*******************************************************************************
* File Name: PWM_PIEZO_PM.c
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

#include "PWM_PIEZO.h"

static PWM_PIEZO_BACKUP_STRUCT PWM_PIEZO_backup;


/*******************************************************************************
* Function Name: PWM_PIEZO_SaveConfig
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
void PWM_PIEZO_SaveConfig(void)
{

}


/*******************************************************************************
* Function Name: PWM_PIEZO_Sleep
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
void PWM_PIEZO_Sleep(void)
{
    if(0u != (PWM_PIEZO_BLOCK_CONTROL_REG & PWM_PIEZO_MASK))
    {
        PWM_PIEZO_backup.enableState = 1u;
    }
    else
    {
        PWM_PIEZO_backup.enableState = 0u;
    }

    PWM_PIEZO_Stop();
    PWM_PIEZO_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_PIEZO_RestoreConfig
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
void PWM_PIEZO_RestoreConfig(void)
{

}


/*******************************************************************************
* Function Name: PWM_PIEZO_Wakeup
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
void PWM_PIEZO_Wakeup(void)
{
    PWM_PIEZO_RestoreConfig();

    if(0u != PWM_PIEZO_backup.enableState)
    {
        PWM_PIEZO_Enable();
    }
}


/* [] END OF FILE */
