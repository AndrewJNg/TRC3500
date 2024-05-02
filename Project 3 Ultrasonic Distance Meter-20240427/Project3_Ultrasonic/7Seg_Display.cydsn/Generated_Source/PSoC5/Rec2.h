/*******************************************************************************
* File Name: Rec2.h  
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

#if !defined(CY_PINS_Rec2_H) /* Pins Rec2_H */
#define CY_PINS_Rec2_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Rec2_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Rec2__PORT == 15 && ((Rec2__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Rec2_Write(uint8 value);
void    Rec2_SetDriveMode(uint8 mode);
uint8   Rec2_ReadDataReg(void);
uint8   Rec2_Read(void);
void    Rec2_SetInterruptMode(uint16 position, uint16 mode);
uint8   Rec2_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Rec2_SetDriveMode() function.
     *  @{
     */
        #define Rec2_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Rec2_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Rec2_DM_RES_UP          PIN_DM_RES_UP
        #define Rec2_DM_RES_DWN         PIN_DM_RES_DWN
        #define Rec2_DM_OD_LO           PIN_DM_OD_LO
        #define Rec2_DM_OD_HI           PIN_DM_OD_HI
        #define Rec2_DM_STRONG          PIN_DM_STRONG
        #define Rec2_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Rec2_MASK               Rec2__MASK
#define Rec2_SHIFT              Rec2__SHIFT
#define Rec2_WIDTH              1u

/* Interrupt constants */
#if defined(Rec2__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Rec2_SetInterruptMode() function.
     *  @{
     */
        #define Rec2_INTR_NONE      (uint16)(0x0000u)
        #define Rec2_INTR_RISING    (uint16)(0x0001u)
        #define Rec2_INTR_FALLING   (uint16)(0x0002u)
        #define Rec2_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Rec2_INTR_MASK      (0x01u) 
#endif /* (Rec2__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Rec2_PS                     (* (reg8 *) Rec2__PS)
/* Data Register */
#define Rec2_DR                     (* (reg8 *) Rec2__DR)
/* Port Number */
#define Rec2_PRT_NUM                (* (reg8 *) Rec2__PRT) 
/* Connect to Analog Globals */                                                  
#define Rec2_AG                     (* (reg8 *) Rec2__AG)                       
/* Analog MUX bux enable */
#define Rec2_AMUX                   (* (reg8 *) Rec2__AMUX) 
/* Bidirectional Enable */                                                        
#define Rec2_BIE                    (* (reg8 *) Rec2__BIE)
/* Bit-mask for Aliased Register Access */
#define Rec2_BIT_MASK               (* (reg8 *) Rec2__BIT_MASK)
/* Bypass Enable */
#define Rec2_BYP                    (* (reg8 *) Rec2__BYP)
/* Port wide control signals */                                                   
#define Rec2_CTL                    (* (reg8 *) Rec2__CTL)
/* Drive Modes */
#define Rec2_DM0                    (* (reg8 *) Rec2__DM0) 
#define Rec2_DM1                    (* (reg8 *) Rec2__DM1)
#define Rec2_DM2                    (* (reg8 *) Rec2__DM2) 
/* Input Buffer Disable Override */
#define Rec2_INP_DIS                (* (reg8 *) Rec2__INP_DIS)
/* LCD Common or Segment Drive */
#define Rec2_LCD_COM_SEG            (* (reg8 *) Rec2__LCD_COM_SEG)
/* Enable Segment LCD */
#define Rec2_LCD_EN                 (* (reg8 *) Rec2__LCD_EN)
/* Slew Rate Control */
#define Rec2_SLW                    (* (reg8 *) Rec2__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Rec2_PRTDSI__CAPS_SEL       (* (reg8 *) Rec2__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Rec2_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Rec2__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Rec2_PRTDSI__OE_SEL0        (* (reg8 *) Rec2__PRTDSI__OE_SEL0) 
#define Rec2_PRTDSI__OE_SEL1        (* (reg8 *) Rec2__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Rec2_PRTDSI__OUT_SEL0       (* (reg8 *) Rec2__PRTDSI__OUT_SEL0) 
#define Rec2_PRTDSI__OUT_SEL1       (* (reg8 *) Rec2__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Rec2_PRTDSI__SYNC_OUT       (* (reg8 *) Rec2__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Rec2__SIO_CFG)
    #define Rec2_SIO_HYST_EN        (* (reg8 *) Rec2__SIO_HYST_EN)
    #define Rec2_SIO_REG_HIFREQ     (* (reg8 *) Rec2__SIO_REG_HIFREQ)
    #define Rec2_SIO_CFG            (* (reg8 *) Rec2__SIO_CFG)
    #define Rec2_SIO_DIFF           (* (reg8 *) Rec2__SIO_DIFF)
#endif /* (Rec2__SIO_CFG) */

/* Interrupt Registers */
#if defined(Rec2__INTSTAT)
    #define Rec2_INTSTAT            (* (reg8 *) Rec2__INTSTAT)
    #define Rec2_SNAP               (* (reg8 *) Rec2__SNAP)
    
	#define Rec2_0_INTTYPE_REG 		(* (reg8 *) Rec2__0__INTTYPE)
#endif /* (Rec2__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Rec2_H */


/* [] END OF FILE */
