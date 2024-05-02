/*******************************************************************************
* File Name: Rec1.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Rec1_H) /* Pins Rec1_H */
#define CY_PINS_Rec1_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Rec1_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Rec1__PORT == 15 && ((Rec1__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Rec1_Write(uint8 value);
void    Rec1_SetDriveMode(uint8 mode);
uint8   Rec1_ReadDataReg(void);
uint8   Rec1_Read(void);
void    Rec1_SetInterruptMode(uint16 position, uint16 mode);
uint8   Rec1_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Rec1_SetDriveMode() function.
     *  @{
     */
        #define Rec1_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Rec1_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Rec1_DM_RES_UP          PIN_DM_RES_UP
        #define Rec1_DM_RES_DWN         PIN_DM_RES_DWN
        #define Rec1_DM_OD_LO           PIN_DM_OD_LO
        #define Rec1_DM_OD_HI           PIN_DM_OD_HI
        #define Rec1_DM_STRONG          PIN_DM_STRONG
        #define Rec1_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Rec1_MASK               Rec1__MASK
#define Rec1_SHIFT              Rec1__SHIFT
#define Rec1_WIDTH              1u

/* Interrupt constants */
#if defined(Rec1__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Rec1_SetInterruptMode() function.
     *  @{
     */
        #define Rec1_INTR_NONE      (uint16)(0x0000u)
        #define Rec1_INTR_RISING    (uint16)(0x0001u)
        #define Rec1_INTR_FALLING   (uint16)(0x0002u)
        #define Rec1_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Rec1_INTR_MASK      (0x01u) 
#endif /* (Rec1__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Rec1_PS                     (* (reg8 *) Rec1__PS)
/* Data Register */
#define Rec1_DR                     (* (reg8 *) Rec1__DR)
/* Port Number */
#define Rec1_PRT_NUM                (* (reg8 *) Rec1__PRT) 
/* Connect to Analog Globals */                                                  
#define Rec1_AG                     (* (reg8 *) Rec1__AG)                       
/* Analog MUX bux enable */
#define Rec1_AMUX                   (* (reg8 *) Rec1__AMUX) 
/* Bidirectional Enable */                                                        
#define Rec1_BIE                    (* (reg8 *) Rec1__BIE)
/* Bit-mask for Aliased Register Access */
#define Rec1_BIT_MASK               (* (reg8 *) Rec1__BIT_MASK)
/* Bypass Enable */
#define Rec1_BYP                    (* (reg8 *) Rec1__BYP)
/* Port wide control signals */                                                   
#define Rec1_CTL                    (* (reg8 *) Rec1__CTL)
/* Drive Modes */
#define Rec1_DM0                    (* (reg8 *) Rec1__DM0) 
#define Rec1_DM1                    (* (reg8 *) Rec1__DM1)
#define Rec1_DM2                    (* (reg8 *) Rec1__DM2) 
/* Input Buffer Disable Override */
#define Rec1_INP_DIS                (* (reg8 *) Rec1__INP_DIS)
/* LCD Common or Segment Drive */
#define Rec1_LCD_COM_SEG            (* (reg8 *) Rec1__LCD_COM_SEG)
/* Enable Segment LCD */
#define Rec1_LCD_EN                 (* (reg8 *) Rec1__LCD_EN)
/* Slew Rate Control */
#define Rec1_SLW                    (* (reg8 *) Rec1__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Rec1_PRTDSI__CAPS_SEL       (* (reg8 *) Rec1__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Rec1_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Rec1__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Rec1_PRTDSI__OE_SEL0        (* (reg8 *) Rec1__PRTDSI__OE_SEL0) 
#define Rec1_PRTDSI__OE_SEL1        (* (reg8 *) Rec1__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Rec1_PRTDSI__OUT_SEL0       (* (reg8 *) Rec1__PRTDSI__OUT_SEL0) 
#define Rec1_PRTDSI__OUT_SEL1       (* (reg8 *) Rec1__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Rec1_PRTDSI__SYNC_OUT       (* (reg8 *) Rec1__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Rec1__SIO_CFG)
    #define Rec1_SIO_HYST_EN        (* (reg8 *) Rec1__SIO_HYST_EN)
    #define Rec1_SIO_REG_HIFREQ     (* (reg8 *) Rec1__SIO_REG_HIFREQ)
    #define Rec1_SIO_CFG            (* (reg8 *) Rec1__SIO_CFG)
    #define Rec1_SIO_DIFF           (* (reg8 *) Rec1__SIO_DIFF)
#endif /* (Rec1__SIO_CFG) */

/* Interrupt Registers */
#if defined(Rec1__INTSTAT)
    #define Rec1_INTSTAT            (* (reg8 *) Rec1__INTSTAT)
    #define Rec1_SNAP               (* (reg8 *) Rec1__SNAP)
    
	#define Rec1_0_INTTYPE_REG 		(* (reg8 *) Rec1__0__INTTYPE)
#endif /* (Rec1__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Rec1_H */


/* [] END OF FILE */
