/*******************************************************************************
* File Name: Dis_D3.h  
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

#if !defined(CY_PINS_Dis_D3_H) /* Pins Dis_D3_H */
#define CY_PINS_Dis_D3_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Dis_D3_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Dis_D3__PORT == 15 && ((Dis_D3__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Dis_D3_Write(uint8 value);
void    Dis_D3_SetDriveMode(uint8 mode);
uint8   Dis_D3_ReadDataReg(void);
uint8   Dis_D3_Read(void);
void    Dis_D3_SetInterruptMode(uint16 position, uint16 mode);
uint8   Dis_D3_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Dis_D3_SetDriveMode() function.
     *  @{
     */
        #define Dis_D3_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Dis_D3_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Dis_D3_DM_RES_UP          PIN_DM_RES_UP
        #define Dis_D3_DM_RES_DWN         PIN_DM_RES_DWN
        #define Dis_D3_DM_OD_LO           PIN_DM_OD_LO
        #define Dis_D3_DM_OD_HI           PIN_DM_OD_HI
        #define Dis_D3_DM_STRONG          PIN_DM_STRONG
        #define Dis_D3_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Dis_D3_MASK               Dis_D3__MASK
#define Dis_D3_SHIFT              Dis_D3__SHIFT
#define Dis_D3_WIDTH              1u

/* Interrupt constants */
#if defined(Dis_D3__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Dis_D3_SetInterruptMode() function.
     *  @{
     */
        #define Dis_D3_INTR_NONE      (uint16)(0x0000u)
        #define Dis_D3_INTR_RISING    (uint16)(0x0001u)
        #define Dis_D3_INTR_FALLING   (uint16)(0x0002u)
        #define Dis_D3_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Dis_D3_INTR_MASK      (0x01u) 
#endif /* (Dis_D3__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Dis_D3_PS                     (* (reg8 *) Dis_D3__PS)
/* Data Register */
#define Dis_D3_DR                     (* (reg8 *) Dis_D3__DR)
/* Port Number */
#define Dis_D3_PRT_NUM                (* (reg8 *) Dis_D3__PRT) 
/* Connect to Analog Globals */                                                  
#define Dis_D3_AG                     (* (reg8 *) Dis_D3__AG)                       
/* Analog MUX bux enable */
#define Dis_D3_AMUX                   (* (reg8 *) Dis_D3__AMUX) 
/* Bidirectional Enable */                                                        
#define Dis_D3_BIE                    (* (reg8 *) Dis_D3__BIE)
/* Bit-mask for Aliased Register Access */
#define Dis_D3_BIT_MASK               (* (reg8 *) Dis_D3__BIT_MASK)
/* Bypass Enable */
#define Dis_D3_BYP                    (* (reg8 *) Dis_D3__BYP)
/* Port wide control signals */                                                   
#define Dis_D3_CTL                    (* (reg8 *) Dis_D3__CTL)
/* Drive Modes */
#define Dis_D3_DM0                    (* (reg8 *) Dis_D3__DM0) 
#define Dis_D3_DM1                    (* (reg8 *) Dis_D3__DM1)
#define Dis_D3_DM2                    (* (reg8 *) Dis_D3__DM2) 
/* Input Buffer Disable Override */
#define Dis_D3_INP_DIS                (* (reg8 *) Dis_D3__INP_DIS)
/* LCD Common or Segment Drive */
#define Dis_D3_LCD_COM_SEG            (* (reg8 *) Dis_D3__LCD_COM_SEG)
/* Enable Segment LCD */
#define Dis_D3_LCD_EN                 (* (reg8 *) Dis_D3__LCD_EN)
/* Slew Rate Control */
#define Dis_D3_SLW                    (* (reg8 *) Dis_D3__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Dis_D3_PRTDSI__CAPS_SEL       (* (reg8 *) Dis_D3__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Dis_D3_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Dis_D3__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Dis_D3_PRTDSI__OE_SEL0        (* (reg8 *) Dis_D3__PRTDSI__OE_SEL0) 
#define Dis_D3_PRTDSI__OE_SEL1        (* (reg8 *) Dis_D3__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Dis_D3_PRTDSI__OUT_SEL0       (* (reg8 *) Dis_D3__PRTDSI__OUT_SEL0) 
#define Dis_D3_PRTDSI__OUT_SEL1       (* (reg8 *) Dis_D3__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Dis_D3_PRTDSI__SYNC_OUT       (* (reg8 *) Dis_D3__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Dis_D3__SIO_CFG)
    #define Dis_D3_SIO_HYST_EN        (* (reg8 *) Dis_D3__SIO_HYST_EN)
    #define Dis_D3_SIO_REG_HIFREQ     (* (reg8 *) Dis_D3__SIO_REG_HIFREQ)
    #define Dis_D3_SIO_CFG            (* (reg8 *) Dis_D3__SIO_CFG)
    #define Dis_D3_SIO_DIFF           (* (reg8 *) Dis_D3__SIO_DIFF)
#endif /* (Dis_D3__SIO_CFG) */

/* Interrupt Registers */
#if defined(Dis_D3__INTSTAT)
    #define Dis_D3_INTSTAT            (* (reg8 *) Dis_D3__INTSTAT)
    #define Dis_D3_SNAP               (* (reg8 *) Dis_D3__SNAP)
    
	#define Dis_D3_0_INTTYPE_REG 		(* (reg8 *) Dis_D3__0__INTTYPE)
#endif /* (Dis_D3__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Dis_D3_H */


/* [] END OF FILE */
