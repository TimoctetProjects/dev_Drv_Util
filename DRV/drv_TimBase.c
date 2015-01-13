/***************************************************************************************************
 ***************************************************************************************************
 *
 * @author	Duclos Timothe
 * @date	16/09/2014
 * @file	drv_PWM.c
 * @brief	Timer software
 *
 * @note	Driver gerant les different TimBase du systeme
 *
 ***************************************************************************************************/

/***************************************************************************************************
 * Includes
 */
#include "drv_TimBase.h"

/***************************************************************************************************
 * Private defines
 */


/***************************************************************************************************
 * Private macros
 */

/***************************************************************************************************
 * Private Types
 */
typedef struct {
	uint32_t	Periode;
	Bool_e		IsConfigured;
}Timbase_s;

/***************************************************************************************************
 * Private Variables
 */
Timbase_s Etat_TimBase[nb_TimPeriph];

/***************************************************************************************************
 * Private Function prototypes
 */
static inline void 		TimeBase_RccInit(uint32_t Periph);
static inline ListePeriphTim_e 	GetTimPeriphEnum(uint32_t Periph);


/***************************************************************************************************
 * Constructors Functions Definition
 */
__attribute__((constructor))
void initTimBaseStruct(void) {
	uint8_t b_Periph = 0;

	for(b_Periph=nb_TimPeriph; b_Periph>0; b_Periph--)
		Etat_TimBase[b_Periph] = ((Timbase_s) {0, FALSE});
}


/***************************************************************************************************
 * Exported Fonctions Definition
 */

/**-------------------------------------------------------------------------------------------------
 * @brief	Initialisation du TimeBase
 * @note	La pin IdPinPwm n'a besoin sur d'avoir son periph de
 * 		configurer afin de fonctionner
 */
void
TimeBase_init(
		uint32_t	Periph,			/**<[in] ID de la Pin ou générer la PWM*/
		uint32_t 	TIM_Periode,		/**<[in] Periode de comptage */
		uint32_t 	TIM_Prescaler 		/**<[in] Prescaler du TimeBase */
) {

	//----------------------------------------------------------------------------
	//------------------------ Déclaration et Initialisation ---------------------
	//----------------------------------------------------------------------------
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

	//--------- Initialisation des structures aux valeurs par défaut
	TIM_TimeBaseStructInit	(&TIM_TimeBaseInitStruct);


	//--------- Initialisation de l'horloge
	TimeBase_RccInit(Periph);

	//---------- Remplissage de la structure d'init TimBase
	TIM_TimeBaseInitStruct.TIM_Prescaler		= TIM_Prescaler;
	TIM_TimeBaseInitStruct.TIM_Period		= TIM_Periode;
	TIM_TimeBaseInitStruct.TIM_CounterMode		= TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_ClockDivision	= TIM_CKD_DIV1;

	//--------- Initialisation de la base de temps
	TIM_TimeBaseInit( (TIM_TypeDef*) Periph, &TIM_TimeBaseInitStruct );
}


/***************************************************************************************************
 * Private Fonctions Definition
 */
/**-------------------------------------------------------------------------------------------------
 * @brief	Init Clock APB1 ou APB2 en fonction du TIM
 */
static inline void
TimeBase_RccInit(
		uint32_t Periph
) {
	switch(Periph) {

		case (uint32_t) TIM1: 	RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM1,  ENABLE); 	break;
		case (uint32_t) TIM2: 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,  ENABLE); 	break;
		case (uint32_t) TIM3: 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,  ENABLE); 	break;
		case (uint32_t) TIM4: 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,  ENABLE); 	break;
		case (uint32_t) TIM5: 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,  ENABLE); 	break;
		case (uint32_t) TIM6: 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,  ENABLE); 	break;
		case (uint32_t) TIM7: 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,  ENABLE); 	break;
		case (uint32_t) TIM8: 	RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM8,  ENABLE); 	break;
		case (uint32_t) TIM9: 	RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM9,  ENABLE); 	break;
		case (uint32_t) TIM10: 	RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE); 	break;
		case (uint32_t) TIM11: 	RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE); 	break;
		case (uint32_t) TIM12: 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE); 	break;
		case (uint32_t) TIM13: 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE); 	break;
		case (uint32_t) TIM14: 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE); 	break;
	}
}

/**-------------------------------------------------------------------------------------------------
 * @brief	Lecture de l'enum periph TIM a partir de son adresse
 * @return	Periph
 */
static inline
ListePeriphTim_e
GetTimPeriphEnum(
	uint32_t Periph
) {
	switch(Periph) {

		case (uint32_t) TIM1: 	return Periph_Tim1; 		break;
		case (uint32_t) TIM2: 	return Periph_Tim2; 		break;
		case (uint32_t) TIM3: 	return Periph_Tim3;  		break;
		case (uint32_t) TIM4: 	return Periph_Tim4;  		break;
		case (uint32_t) TIM5: 	return Periph_Tim5;  		break;
		case (uint32_t) TIM6: 	return Periph_Tim6;  		break;
		case (uint32_t) TIM7: 	return Periph_Tim7;  		break;
		case (uint32_t) TIM8: 	return Periph_Tim8;  		break;
		case (uint32_t) TIM9: 	return Periph_Tim9;  		break;
		case (uint32_t) TIM10: 	return Periph_Tim10; 		break;
		case (uint32_t) TIM11: 	return Periph_Tim11; 		break;
		case (uint32_t) TIM12: 	return Periph_Tim12; 		break;
		case (uint32_t) TIM13: 	return Periph_Tim13; 		break;
		case (uint32_t) TIM14: 	return Periph_Tim14; 		break;

		default:		return err_NotATimPeriph;	break;
	}
}
