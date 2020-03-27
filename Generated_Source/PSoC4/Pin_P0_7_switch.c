/*******************************************************************************
* File Name: Pin_P0_7_switch.c  
* Version 2.10
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "Pin_P0_7_switch.h"

#define SetP4PinDriveMode(shift, mode)  \
    do { \
        Pin_P0_7_switch_PC =   (Pin_P0_7_switch_PC & \
                                (uint32)(~(uint32)(Pin_P0_7_switch_DRIVE_MODE_IND_MASK << (Pin_P0_7_switch_DRIVE_MODE_BITS * (shift))))) | \
                                (uint32)((uint32)(mode) << (Pin_P0_7_switch_DRIVE_MODE_BITS * (shift))); \
    } while (0)


/*******************************************************************************
* Function Name: Pin_P0_7_switch_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None 
*  
*******************************************************************************/
void Pin_P0_7_switch_Write(uint8 value) 
{
    uint8 drVal = (uint8)(Pin_P0_7_switch_DR & (uint8)(~Pin_P0_7_switch_MASK));
    drVal = (drVal | ((uint8)(value << Pin_P0_7_switch_SHIFT) & Pin_P0_7_switch_MASK));
    Pin_P0_7_switch_DR = (uint32)drVal;
}


/*******************************************************************************
* Function Name: Pin_P0_7_switch_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  Pin_P0_7_switch_DM_STRONG     Strong Drive 
*  Pin_P0_7_switch_DM_OD_HI      Open Drain, Drives High 
*  Pin_P0_7_switch_DM_OD_LO      Open Drain, Drives Low 
*  Pin_P0_7_switch_DM_RES_UP     Resistive Pull Up 
*  Pin_P0_7_switch_DM_RES_DWN    Resistive Pull Down 
*  Pin_P0_7_switch_DM_RES_UPDWN  Resistive Pull Up/Down 
*  Pin_P0_7_switch_DM_DIG_HIZ    High Impedance Digital 
*  Pin_P0_7_switch_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void Pin_P0_7_switch_SetDriveMode(uint8 mode) 
{
	SetP4PinDriveMode(Pin_P0_7_switch__0__SHIFT, mode);
}


/*******************************************************************************
* Function Name: Pin_P0_7_switch_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro Pin_P0_7_switch_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 Pin_P0_7_switch_Read(void) 
{
    return (uint8)((Pin_P0_7_switch_PS & Pin_P0_7_switch_MASK) >> Pin_P0_7_switch_SHIFT);
}


/*******************************************************************************
* Function Name: Pin_P0_7_switch_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 Pin_P0_7_switch_ReadDataReg(void) 
{
    return (uint8)((Pin_P0_7_switch_DR & Pin_P0_7_switch_MASK) >> Pin_P0_7_switch_SHIFT);
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(Pin_P0_7_switch_INTSTAT) 

    /*******************************************************************************
    * Function Name: Pin_P0_7_switch_ClearInterrupt
    ********************************************************************************
    *
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 Pin_P0_7_switch_ClearInterrupt(void) 
    {
		uint8 maskedStatus = (uint8)(Pin_P0_7_switch_INTSTAT & Pin_P0_7_switch_MASK);
		Pin_P0_7_switch_INTSTAT = maskedStatus;
        return maskedStatus >> Pin_P0_7_switch_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */
