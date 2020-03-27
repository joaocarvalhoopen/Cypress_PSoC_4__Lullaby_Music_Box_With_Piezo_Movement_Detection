/*******************************************************************************
* File Name: Pin_P0_7_switch.h  
* Version 2.10
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Pin_P0_7_switch_H) /* Pins Pin_P0_7_switch_H */
#define CY_PINS_Pin_P0_7_switch_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Pin_P0_7_switch_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    Pin_P0_7_switch_Write(uint8 value) ;
void    Pin_P0_7_switch_SetDriveMode(uint8 mode) ;
uint8   Pin_P0_7_switch_ReadDataReg(void) ;
uint8   Pin_P0_7_switch_Read(void) ;
uint8   Pin_P0_7_switch_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Pin_P0_7_switch_DRIVE_MODE_BITS        (3)
#define Pin_P0_7_switch_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Pin_P0_7_switch_DRIVE_MODE_BITS))

#define Pin_P0_7_switch_DM_ALG_HIZ         (0x00u)
#define Pin_P0_7_switch_DM_DIG_HIZ         (0x01u)
#define Pin_P0_7_switch_DM_RES_UP          (0x02u)
#define Pin_P0_7_switch_DM_RES_DWN         (0x03u)
#define Pin_P0_7_switch_DM_OD_LO           (0x04u)
#define Pin_P0_7_switch_DM_OD_HI           (0x05u)
#define Pin_P0_7_switch_DM_STRONG          (0x06u)
#define Pin_P0_7_switch_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define Pin_P0_7_switch_MASK               Pin_P0_7_switch__MASK
#define Pin_P0_7_switch_SHIFT              Pin_P0_7_switch__SHIFT
#define Pin_P0_7_switch_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Pin_P0_7_switch_PS                     (* (reg32 *) Pin_P0_7_switch__PS)
/* Port Configuration */
#define Pin_P0_7_switch_PC                     (* (reg32 *) Pin_P0_7_switch__PC)
/* Data Register */
#define Pin_P0_7_switch_DR                     (* (reg32 *) Pin_P0_7_switch__DR)
/* Input Buffer Disable Override */
#define Pin_P0_7_switch_INP_DIS                (* (reg32 *) Pin_P0_7_switch__PC2)


#if defined(Pin_P0_7_switch__INTSTAT)  /* Interrupt Registers */

    #define Pin_P0_7_switch_INTSTAT                (* (reg32 *) Pin_P0_7_switch__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define Pin_P0_7_switch_DRIVE_MODE_SHIFT       (0x00u)
#define Pin_P0_7_switch_DRIVE_MODE_MASK        (0x07u << Pin_P0_7_switch_DRIVE_MODE_SHIFT)


#endif /* End Pins Pin_P0_7_switch_H */


/* [] END OF FILE */
