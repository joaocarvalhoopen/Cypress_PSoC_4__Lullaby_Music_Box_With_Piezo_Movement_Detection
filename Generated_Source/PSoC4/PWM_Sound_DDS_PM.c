/*******************************************************************************
* File Name: PWM_Sound_DDS_PM.c
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

#include "PWM_Sound_DDS.h"

static PWM_Sound_DDS_BACKUP_STRUCT PWM_Sound_DDS_backup;


/*******************************************************************************
* Function Name: PWM_Sound_DDS_SaveConfig
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
void PWM_Sound_DDS_SaveConfig(void)
{

}


/*******************************************************************************
* Function Name: PWM_Sound_DDS_Sleep
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
void PWM_Sound_DDS_Sleep(void)
{
    if(0u != (PWM_Sound_DDS_BLOCK_CONTROL_REG & PWM_Sound_DDS_MASK))
    {
        PWM_Sound_DDS_backup.enableState = 1u;
    }
    else
    {
        PWM_Sound_DDS_backup.enableState = 0u;
    }

    PWM_Sound_DDS_Stop();
    PWM_Sound_DDS_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_Sound_DDS_RestoreConfig
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
void PWM_Sound_DDS_RestoreConfig(void)
{

}


/*******************************************************************************
* Function Name: PWM_Sound_DDS_Wakeup
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
void PWM_Sound_DDS_Wakeup(void)
{
    PWM_Sound_DDS_RestoreConfig();

    if(0u != PWM_Sound_DDS_backup.enableState)
    {
        PWM_Sound_DDS_Enable();
    }
}


/* [] END OF FILE */
