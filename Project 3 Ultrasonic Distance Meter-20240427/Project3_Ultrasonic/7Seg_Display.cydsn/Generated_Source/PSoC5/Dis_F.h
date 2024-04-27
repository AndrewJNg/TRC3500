/*******************************************************************************
* File Name: Dis_F.h  
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

#if !defined(CY_PINS_Dis_F_H) /* Pins Dis_F_H */
#define CY_PINS_Dis_F_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Dis_F_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Dis_F__PORT == 15 && ((Dis_F__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Dis_F_Write(uint8 value);
void    Dis_F_SetDriveMode(uint8 mode);
uint8   Dis_F_ReadDataReg(void);
uint8   Dis_F_Read(void);
void    Dis_F_SetInterruptMode(uint16 position, uint16 mode);
uint8   Dis_F_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Dis_F_SetDriveMode() function.
     *  @{
     */
        #define Dis_F_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Dis_F_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Dis_F_DM_RES_UP          PIN_DM_RES_UP
        #define Dis_F_DM_RES_DWN         PIN_DM_RES_DWN
        #define Dis_F_DM_OD_LO           PIN_DM_OD_LO
        #define Dis_F_DM_OD_HI           PIN_DM_OD_HI
        #define Dis_F_DM_STRONG          PIN_DM_STRONG
        #define Dis_F_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Dis_F_MASK               Dis_F__MASK
#define Dis_F_SHIFT              Dis_F__SHIFT
#define Dis_F_WIDTH              1u

/* Interrupt constants */
#if defined(Dis_F__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Dis_F_SetInterruptMode() function.
     *  @{
     */
        #define Dis_F_INTR_NONE      (uint16)(0x0000u)
        #define Dis_F_INTR_RISING    (uint16)(0x0001u)
        #define Dis_F_INTR_FALLING   (uint16)(0x0002u)
        #define Dis_F_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Dis_F_INTR_MASK      (0x01u) 
#endif /* (Dis_F__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Dis_F_PS                     (* (reg8 *) Dis_F__PS)
/* Data Register */
#define Dis_F_DR                     (* (reg8 *) Dis_F__DR)
/* Port Number */
#define Dis_F_PRT_NUM                (* (reg8 *) Dis_F__PRT) 
/* Connect to Analog Globals */                                                  
#define Dis_F_AG                     (* (reg8 *) Dis_F__AG)                       
/* Analog MUX bux enable */
#define Dis_F_AMUX                   (* (reg8 *) Dis_F__AMUX) 
/* Bidirectional Enable */                                                        
#define Dis_F_BIE                    (* (reg8 *) Dis_F__BIE)
/* Bit-mask for Aliased Register Access */
#define Dis_F_BIT_MASK               (* (reg8 *) Dis_F__BIT_MASK)
/* Bypass Enable */
#define Dis_F_BYP                    (* (reg8 *) Dis_F__BYP)
/* Port wide control signals */                                                   
#define Dis_F_CTL                    (* (reg8 *) Dis_F__CTL)
/* Drive Modes */
#define Dis_F_DM0                    (* (reg8 *) Dis_F__DM0) 
#define Dis_F_DM1                    (* (reg8 *) Dis_F__DM1)
#define Dis_F_DM2                    (* (reg8 *) Dis_F__DM2) 
/* Input Buffer Disable Override */
#define Dis_F_INP_DIS                (* (reg8 *) Dis_F__INP_DIS)
/* LCD Common or Segment Drive */
#define Dis_F_LCD_COM_SEG            (* (reg8 *) Dis_F__LCD_COM_SEG)
/* Enable Segment LCD */
#define Dis_F_LCD_EN                 (* (reg8 *) Dis_F__LCD_EN)
/* Slew Rate Control */
#define Dis_F_SLW                    (* (reg8 *) Dis_F__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Dis_F_PRTDSI__CAPS_SEL       (* (reg8 *) Dis_F__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Dis_F_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Dis_F__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Dis_F_PRTDSI__OE_SEL0        (* (reg8 *) Dis_F__PRTDSI__OE_SEL0) 
#define Dis_F_PRTDSI__OE_SEL1        (* (reg8 *) Dis_F__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Dis_F_PRTDSI__OUT_SEL0       (* (reg8 *) Dis_F__PRTDSI__OUT_SEL0) 
#define Dis_F_PRTDSI__OUT_SEL1       (* (reg8 *) Dis_F__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Dis_F_PRTDSI__SYNC_OUT       (* (reg8 *) Dis_F__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Dis_F__SIO_CFG)
    #define Dis_F_SIO_HYST_EN        (* (reg8 *) Dis_F__SIO_HYST_EN)
    #define Dis_F_SIO_REG_HIFREQ     (* (reg8 *) Dis_F__SIO_REG_HIFREQ)
    #define Dis_F_SIO_CFG            (* (reg8 *) Dis_F__SIO_CFG)
    #define Dis_F_SIO_DIFF           (* (reg8 *) Dis_F__SIO_DIFF)
#endif /* (Dis_F__SIO_CFG) */

/* Interrupt Registers */
#if defined(Dis_F__INTSTAT)
    #define Dis_F_INTSTAT            (* (reg8 *) Dis_F__INTSTAT)
    #define Dis_F_SNAP               (* (reg8 *) Dis_F__SNAP)
    
	#define Dis_F_0_INTTYPE_REG 		(* (reg8 *) Dis_F__0__INTTYPE)
#endif /* (Dis_F__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Dis_F_H */


/* [] END OF FILE */
