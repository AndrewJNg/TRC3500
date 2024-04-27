/*******************************************************************************
* File Name: Dis_DP.h  
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

#if !defined(CY_PINS_Dis_DP_H) /* Pins Dis_DP_H */
#define CY_PINS_Dis_DP_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Dis_DP_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Dis_DP__PORT == 15 && ((Dis_DP__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Dis_DP_Write(uint8 value);
void    Dis_DP_SetDriveMode(uint8 mode);
uint8   Dis_DP_ReadDataReg(void);
uint8   Dis_DP_Read(void);
void    Dis_DP_SetInterruptMode(uint16 position, uint16 mode);
uint8   Dis_DP_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Dis_DP_SetDriveMode() function.
     *  @{
     */
        #define Dis_DP_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Dis_DP_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Dis_DP_DM_RES_UP          PIN_DM_RES_UP
        #define Dis_DP_DM_RES_DWN         PIN_DM_RES_DWN
        #define Dis_DP_DM_OD_LO           PIN_DM_OD_LO
        #define Dis_DP_DM_OD_HI           PIN_DM_OD_HI
        #define Dis_DP_DM_STRONG          PIN_DM_STRONG
        #define Dis_DP_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Dis_DP_MASK               Dis_DP__MASK
#define Dis_DP_SHIFT              Dis_DP__SHIFT
#define Dis_DP_WIDTH              1u

/* Interrupt constants */
#if defined(Dis_DP__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Dis_DP_SetInterruptMode() function.
     *  @{
     */
        #define Dis_DP_INTR_NONE      (uint16)(0x0000u)
        #define Dis_DP_INTR_RISING    (uint16)(0x0001u)
        #define Dis_DP_INTR_FALLING   (uint16)(0x0002u)
        #define Dis_DP_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Dis_DP_INTR_MASK      (0x01u) 
#endif /* (Dis_DP__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Dis_DP_PS                     (* (reg8 *) Dis_DP__PS)
/* Data Register */
#define Dis_DP_DR                     (* (reg8 *) Dis_DP__DR)
/* Port Number */
#define Dis_DP_PRT_NUM                (* (reg8 *) Dis_DP__PRT) 
/* Connect to Analog Globals */                                                  
#define Dis_DP_AG                     (* (reg8 *) Dis_DP__AG)                       
/* Analog MUX bux enable */
#define Dis_DP_AMUX                   (* (reg8 *) Dis_DP__AMUX) 
/* Bidirectional Enable */                                                        
#define Dis_DP_BIE                    (* (reg8 *) Dis_DP__BIE)
/* Bit-mask for Aliased Register Access */
#define Dis_DP_BIT_MASK               (* (reg8 *) Dis_DP__BIT_MASK)
/* Bypass Enable */
#define Dis_DP_BYP                    (* (reg8 *) Dis_DP__BYP)
/* Port wide control signals */                                                   
#define Dis_DP_CTL                    (* (reg8 *) Dis_DP__CTL)
/* Drive Modes */
#define Dis_DP_DM0                    (* (reg8 *) Dis_DP__DM0) 
#define Dis_DP_DM1                    (* (reg8 *) Dis_DP__DM1)
#define Dis_DP_DM2                    (* (reg8 *) Dis_DP__DM2) 
/* Input Buffer Disable Override */
#define Dis_DP_INP_DIS                (* (reg8 *) Dis_DP__INP_DIS)
/* LCD Common or Segment Drive */
#define Dis_DP_LCD_COM_SEG            (* (reg8 *) Dis_DP__LCD_COM_SEG)
/* Enable Segment LCD */
#define Dis_DP_LCD_EN                 (* (reg8 *) Dis_DP__LCD_EN)
/* Slew Rate Control */
#define Dis_DP_SLW                    (* (reg8 *) Dis_DP__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Dis_DP_PRTDSI__CAPS_SEL       (* (reg8 *) Dis_DP__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Dis_DP_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Dis_DP__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Dis_DP_PRTDSI__OE_SEL0        (* (reg8 *) Dis_DP__PRTDSI__OE_SEL0) 
#define Dis_DP_PRTDSI__OE_SEL1        (* (reg8 *) Dis_DP__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Dis_DP_PRTDSI__OUT_SEL0       (* (reg8 *) Dis_DP__PRTDSI__OUT_SEL0) 
#define Dis_DP_PRTDSI__OUT_SEL1       (* (reg8 *) Dis_DP__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Dis_DP_PRTDSI__SYNC_OUT       (* (reg8 *) Dis_DP__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Dis_DP__SIO_CFG)
    #define Dis_DP_SIO_HYST_EN        (* (reg8 *) Dis_DP__SIO_HYST_EN)
    #define Dis_DP_SIO_REG_HIFREQ     (* (reg8 *) Dis_DP__SIO_REG_HIFREQ)
    #define Dis_DP_SIO_CFG            (* (reg8 *) Dis_DP__SIO_CFG)
    #define Dis_DP_SIO_DIFF           (* (reg8 *) Dis_DP__SIO_DIFF)
#endif /* (Dis_DP__SIO_CFG) */

/* Interrupt Registers */
#if defined(Dis_DP__INTSTAT)
    #define Dis_DP_INTSTAT            (* (reg8 *) Dis_DP__INTSTAT)
    #define Dis_DP_SNAP               (* (reg8 *) Dis_DP__SNAP)
    
	#define Dis_DP_0_INTTYPE_REG 		(* (reg8 *) Dis_DP__0__INTTYPE)
#endif /* (Dis_DP__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Dis_DP_H */


/* [] END OF FILE */
