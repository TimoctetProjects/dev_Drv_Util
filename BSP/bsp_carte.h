/*********************************************************************
 * *******************************************************************
 *
 * @file	bsp_carte.h
 *
 * @author	Duclos Timothe
 *
 * @date	19/07/2014
 *
 * @brief	Config des pins
 *
 *********************************************************************/

#ifndef BSP_CARTE_H
#define BSP_CARTE_H

/********************************************************************
 * Includes
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "stm32f4xx_conf.h"
#include "core_cm4.h"



/********************************************************************
 * Exported defines
 */
#define CONFIG_SYSCLOCK_1ms	(SystemCoreClock / 1000)

#ifdef NULL
	#undef NULL
#endif
#ifndef NULL
	#define NULL 0
#endif

#ifndef WEAK
	#define WEAK __attribute__ ((weak))
#endif


#define __NOP				asm("nop")

#define __strncmp(sTR1, sTR2, args...)	 strncmp( (char *)sTR1,  (char *)sTR2, ## args)
#define __strncpy(sTR1, sTR2, args...)	 strncpy( (char *)sTR1,  (char *)sTR2, ## args)
#define __strlen(sTR1)			 strlen(  (char *)sTR1 			      )
#define __strncat(sTR1, sTR2, args...)	 strncat( (char *)sTR1,  (char *)sTR2, ## args)
#define __snprintf(sTRING, args...)	 snprintf((char *)sTRING, ## args)


/********************************************************************
 * Exported types
 */

/** Etats */
typedef enum {

	ETAT_INACTIF = 0,
	ETAT_ACTIF,

}Etat_e;



/** Status */
typedef enum {

	STATUS_FINIS = 0,
	STATUS_KO,
	STATUS_OK,
	STATUS_ENCOURS,

}Status_e;

/** Enum utile a affichage debug */
typedef enum {

	toString_Getpin = 0,
	toString_GetValue,

}toString_Possibilities_e;


/** Booleen */
typedef enum {

	FALSE = 0,
	TRUE,

}Bool_e;

/** Status interruption */
typedef enum  {
	Interrupt_OFF = 0,
	Interrupt_ON
}InterruptState_e;

/** Pointeur de fonction generique */
typedef void (*pFunction)(void*, ...);

/** Enumeration des Pins */
typedef enum {

	/* Pin NULL */
	PIN_NULL = 0,

	/* Console */
	CONSOLE_TX,
	CONSOLE_RX,

	/* Pin Camera */
	Broche_SI,
	BROCHE_CLK,

	/* Leds Discovery Board */
	PIN_SERVO_DRIECTION,

	/* Bouton Poussoir */
	BTN_WKP,

	LigneCam,


	nb_GPIO,

}Mapping_GPIO_e;


/** Structure pour l'init du Mapping */
typedef struct {

	/** Config Pin */
	GPIO_TypeDef*		GpioPeripheral;
	uint16_t		GpioPin;
	GPIOMode_TypeDef	GpioMode;
	Bool_e			Inverse;
	GPIOSpeed_TypeDef	GPIO_Speed;
	GPIOOType_TypeDef	GPIO_OType;
	GPIOPuPd_TypeDef	GPIO_PuPd;

	uint32_t		AlternateFunction;
	uint32_t		Parametre;
	InterruptState_e	Etat_Interruption;
	Etat_e			EtatInit;
	uint32_t		Periph;

}Mapping_GPIO_s;


/********************************************************************
 * Exported variables
 */

/** Mapping GPIO */
const static Mapping_GPIO_s Mapping_GPIO[nb_GPIO] = {

/** -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------  */
/**						Peripheral,	Pin			Mode: In/Out		Inverse		Spped				PP/OD				Pull-up/Pull-down		Alternate Function		Param			Etat Interrupt		Etat initial			Peripherique		*/
/** -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------  */
/** PIN_NULL			*/	{ 	NULL,		NULL,			NULL,			NULL,		NULL,				NULL,				NULL,				NULL,				NULL,			NULL,			NULL,				NULL,			},
/** -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------  */
/** CONSOLE_TX			*/	{ 	GPIOC,		GPIO_Pin_6,		GPIO_Mode_AF,		FALSE,		GPIO_Speed_50MHz,		GPIO_OType_PP,			GPIO_PuPd_NOPULL,		GPIO_AF_USART6,			NULL,			Interrupt_OFF,		NULL,				(uint32_t) USART6	},
/** CONSOLE_RX  		*/	{ 	GPIOC,		GPIO_Pin_7,		GPIO_Mode_AF,		FALSE,		GPIO_Speed_50MHz,		GPIO_OType_PP,			GPIO_PuPd_NOPULL,		GPIO_AF_USART6,			NULL,			Interrupt_ON,		NULL,				(uint32_t) USART6	},
/** -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------  */
/** Broche_SI			*/	{	GPIOB,		GPIO_Pin_4,		GPIO_Mode_AF,		FALSE,		GPIO_Speed_50MHz,		GPIO_OType_PP,			GPIO_PuPd_NOPULL,		GPIO_AF_TIM3,			TIM_Channel_1,		Interrupt_ON,		ETAT_INACTIF,			(uint32_t) TIM3		},
/** BROCHE_CLK  		*/	{ 	GPIOA,		GPIO_Pin_3,		GPIO_Mode_AF,		FALSE,		GPIO_Speed_50MHz,		GPIO_OType_PP,			GPIO_PuPd_NOPULL,		GPIO_AF_TIM2,			TIM_Channel_4,		Interrupt_ON,		ETAT_INACTIF,			(uint32_t) TIM2		},
/** ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------  */
/** PIN_SERVO_DRIECTION  	*/	{ 	GPIOD,		GPIO_Pin_15,		GPIO_Mode_AF,		FALSE,		GPIO_Speed_50MHz,		GPIO_OType_PP,			GPIO_PuPd_NOPULL,		GPIO_AF_TIM4,			TIM_Channel_4,		Interrupt_OFF,		ETAT_INACTIF,			(uint32_t) TIM4		},
/** ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------  */
/** BTN_WKP			*/	{	GPIOA,		GPIO_Pin_0,		GPIO_Mode_IN,		FALSE,		GPIO_Speed_2MHz,		NULL,				GPIO_PuPd_NOPULL,		NULL,				NULL,			NULL,			ETAT_INACTIF,			NULL			},
/** ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------  */
/** LigneCam			*/	{	GPIOC,		GPIO_Pin_1,		GPIO_Mode_AN,		FALSE,		GPIO_Speed_50MHz,		NULL,				GPIO_PuPd_NOPULL,		NULL,				ADC_Channel_11,		Interrupt_ON,		ETAT_INACTIF,			(uint32_t) ADC1		},
/** ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ */
};


/********************************************************************
 * Exported functions
 */
void BSP_Init			(void);
void toString_GetPeriphral	(GPIO_TypeDef* Peripheral, uint8_t*	String);
void toString_GetPin		(uint16_t GpioPin, uint8_t* String);



#endif // BSP_CARTE_H









