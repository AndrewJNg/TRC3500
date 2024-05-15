/*******************************************************************************
* File Name: TX2_Ultra.h  
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

#if !defined(CY_PINS_TX2_Ultra_H) /* Pins TX2_Ultra_H */
#define CY_PINS_TX2_Ultra_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "TX2_Ultra_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 TX2_Ultra__PORT == 15 && ((TX2_Ultra__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    TX2_Ultra_Write(uint8 value);
void    TX2_Ultra_SetDriveMode(uint8 mode);
uint8   TX2_Ultra_ReadDataReg(void);
uint8   TX2_Ultra_Read(void);
void    TX2_Ultra_SetInterruptMode(uint16 position, uint16 mode);
uint8   TX2_Ultra_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the TX2_Ultra_SetDriveMode() function.
     *  @{
     */
        #define TX2_Ultra_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define TX2_Ultra_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define TX2_Ultra_DM_RES_UP          PIN_DM_RES_UP
        #define TX2_Ultra_DM_RES_DWN         PIN_DM_RES_DWN
        #define TX2_Ultra_DM_OD_LO           PIN_DM_OD_LO
        #define TX2_Ultra_DM_OD_HI           PIN_DM_OD_HI
        #define TX2_Ultra_DM_STRONG          PIN_DM_STRONG
        #define TX2_Ultra_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define TX2_Ultra_MASK               TX2_Ultra__MASK
#define TX2_Ultra_SHIFT              TX2_Ultra__SHIFT
#define TX2_Ultra_WIDTH              1u

/* Interrupt constants */
#if defined(TX2_Ultra__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in TX2_Ultra_SetInterruptMode() function.
     *  @{
     */
        #define TX2_Ultra_INTR_NONE      (uint16)(0x0000u)
        #define TX2_Ultra_INTR_RISING    (uint16)(0x0001u)
        #define TX2_Ultra_INTR_FALLING   (uint16)(0x0002u)
        #define TX2_Ultra_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define TX2_Ultra_INTR_MASK      (0x01u) 
#endif /* (TX2_Ultra__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define TX2_Ultra_PS                     (* (reg8 *) TX2_Ultra__PS)
/* Data Register */
#define TX2_Ultra_DR                     (* (reg8 *) TX2_Ultra__DR)
/* Port Number */
#define TX2_Ultra_PRT_NUM                (* (reg8 *) TX2_Ultra__PRT) 
/* Connect to Analog Globals */                                                  
#define TX2_Ultra_AG                     (* (reg8 *) TX2_Ultra__AG)                       
/* Analog MUX bux enable */
#define TX2_Ultra_AMUX                   (* (reg8 *) TX2_Ultra__AMUX) 
/* Bidirectional Enable */                                                        
#define TX2_Ultra_BIE                    (* (reg8 *) TX2_Ultra__BIE)
/* Bit-mask for Aliased Register Access */
#define TX2_Ultra_BIT_MASK               (* (reg8 *) TX2_Ultra__BIT_MASK)
/* Bypass Enable */
#define TX2_Ultra_BYP                    (* (reg8 *) TX2_Ultra__BYP)
/* Port wide control signals */                                                   
#define TX2_Ultra_CTL                    (* (reg8 *) TX2_Ultra__CTL)
/* Drive Modes */
#define TX2_Ultra_DM0                    (* (reg8 *) TX2_Ultra__DM0) 
#define TX2_Ultra_DM1                    (* (reg8 *) TX2_Ultra__DM1)
#define TX2_Ultra_DM2                    (* (reg8 *) TX2_Ultra__DM2) 
/* Input Buffer Disable Override */
#define TX2_Ultra_INP_DIS                (* (reg8 *) TX2_Ultra__INP_DIS)
/* LCD Common or Segment Drive */
#define TX2_Ultra_LCD_COM_SEG            (* (reg8 *) TX2_Ultra__LCD_COM_SEG)
/* Enable Segment LCD */
#define TX2_Ultra_LCD_EN                 (* (reg8 *) TX2_Ultra__LCD_EN)
/* Slew Rate Control */
#define TX2_Ultra_SLW                    (* (reg8 *) TX2_Ultra__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define TX2_Ultra_PRTDSI__CAPS_SEL       (* (reg8 *) TX2_Ultra__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define TX2_Ultra_PRTDSI__DBL_SYNC_IN    (* (reg8 *) TX2_Ultra__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define TX2_Ultra_PRTDSI__OE_SEL0        (* (reg8 *) TX2_Ultra__PRTDSI__OE_SEL0) 
#define TX2_Ultra_PRTDSI__OE_SEL1        (* (reg8 *) TX2_Ultra__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define TX2_Ultra_PRTDSI__OUT_SEL0       (* (reg8 *) TX2_Ultra__PRTDSI__OUT_SEL0) 
#define TX2_Ultra_PRTDSI__OUT_SEL1       (* (reg8 *) TX2_Ultra__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define TX2_Ultra_PRTDSI__SYNC_OUT       (* (reg8 *) TX2_Ultra__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(TX2_Ultra__SIO_CFG)
    #define TX2_Ultra_SIO_HYST_EN        (* (reg8 *) TX2_Ultra__SIO_HYST_EN)
    #define TX2_Ultra_SIO_REG_HIFREQ     (* (reg8 *) TX2_Ultra__SIO_REG_HIFREQ)
    #define TX2_Ultra_SIO_CFG            (* (reg8 *) TX2_Ultra__SIO_CFG)
    #define TX2_Ultra_SIO_DIFF           (* (reg8 *) TX2_Ultra__SIO_DIFF)
#endif /* (TX2_Ultra__SIO_CFG) */

/* Interrupt Registers */
#if defined(TX2_Ultra__INTSTAT)
    #define TX2_Ultra_INTSTAT            (* (reg8 *) TX2_Ultra__INTSTAT)
    #define TX2_Ultra_SNAP               (* (reg8 *) TX2_Ultra__SNAP)
    
	#define TX2_Ultra_0_INTTYPE_REG 		(* (reg8 *) TX2_Ultra__0__INTTYPE)
#endif /* (TX2_Ultra__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_TX2_Ultra_H */


/* [] END OF FILE */
