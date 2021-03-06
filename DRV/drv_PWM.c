/*********************************************************************
 * *******************************************************************
 *
 * @file	drv_PWM.c
 *
 * @author	Duclos Timothe
 *
 * @date	16/09/2014
 *
 * @brief	Driver g�n�rant un signal PWM sur une broche pr�-configur�e
 *
 *********************************************************************/

/********************************************************************
 * Includes
 */
#include "drv_PWM.h"


/********************************************************************
 * Private defines
 */
#define SIZEOF_TIMER_16BITS	0xFFFF			/** Taille du Timer 16 bits */
#define SIZEOF_TIMER_32BITS	0xFFFFFFFF		/** Taille du Timer 32 bits */


/********************************************************************
 * Private macros
 */

#define __GetITChannel(cHAN)	( ( (cHAN) == (uint32_t)TIM_Channel_1 ) ? TIM_IT_CC1 :   \
				( ( (cHAN) == (uint32_t)TIM_Channel_2 ) ? TIM_IT_CC2 :    \
				( ( (cHAN) == (uint32_t)TIM_Channel_3 ) ? TIM_IT_CC3 :  TIM_IT_CC4 ) ) )

/*#define __IsPeriphTimer(pERIPH)		( 	((pERIPH) == (uint32_t) TIM2)  ?  TRUE : \
					(       ((pERIPH) == (uint32_t) TIM3)  ?  TRUE :  \
					(       ((pERIPH) == (uint32_t) TIM4)  ?  TRUE :   \
					(       ((pERIPH) == (uint32_t) TIM5)  ?  TRUE :    \
					(       ((pERIPH) == (uint32_t) TIM6)  ?  TRUE :     \
					(       ((pERIPH) == (uint32_t) TIM7)  ?  TRUE :      \
					(       ((pERIPH) == (uint32_t) TIM8)  ?  TRUE :       \
					(       ((pERIPH) == (uint32_t) TIM9)  ?  TRUE :        \
					(       ((pERIPH) == (uint32_t) TIM10) ?  TRUE :         \
					(       ((pERIPH) == (uint32_t) TIM11) ?  TRUE :          \
					(       ((pERIPH) == (uint32_t) TIM12) ?  TRUE :           \
					(       ((pERIPH) == (uint32_t) TIM13) ?  TRUE :            \
					( 	((pERIPH) == (uint32_t) TIM14) ?  TRUE :   FALSE ) ) ) ) ) ) ) ) ) ) ) ) )*/

#define __IsPeriphTimer(Periph)			( 	((Periph) 	!= (uint32_t) TIM2) 	&&	\
							((Periph) 	!= (uint32_t) TIM3) 	&&	\
							((Periph) 	!= (uint32_t) TIM4) 	&&	\
							((Periph) 	!= (uint32_t) TIM5) 	&&	\
							((Periph) 	!= (uint32_t) TIM6)	&&	\
							((Periph) 	!= (uint32_t) TIM7)	&&	\
							((Periph) 	!= (uint32_t) TIM8)	&&	\
							((Periph) 	!= (uint32_t) TIM9) 	&&	\
							((Periph) 	!= (uint32_t) TIM10)	&&	\
							((Periph) 	!= (uint32_t) TIM11)	&&	\
							((Periph) 	!= (uint32_t) TIM12)	&&	\
							((Periph) 	!= (uint32_t) TIM13)	&&	\
							((Periph) 	!= (uint32_t) TIM14)			)

#define __IsTimerA16BistOne(Periph)		( 	((Periph) 	== (uint32_t) TIM3) 	||	\
							((Periph) 	== (uint32_t) TIM4) 	||	\
							((Periph) 	== (uint32_t) TIM9) 	||	\
							((Periph) 	== (uint32_t) TIM10)	||	\
							((Periph) 	== (uint32_t) TIM11)	||	\
							((Periph) 	== (uint32_t) TIM12)	||	\
							((Periph) 	== (uint32_t) TIM13)	||	\
							((Periph) 	== (uint32_t) TIM14)	||	\
							((Periph) 	== (uint32_t) TIM6)	||	\
							((Periph) 	== (uint32_t) TIM7)			)

#define __IsTimerA32BitsOne(Periph)		( 	((Periph) 	== (uint32_t) TIM2) 	||	\
							((Periph) 	== (uint32_t) TIM5)			)


#define	__GetPeriod_uS(Periph, Channel)		( PWM[ PWM_Save_GetTimPeriph(Periph) ][ PWM_Save_GetTimChannel(Channel) ].SavePeriode_us   )
#define	__GetRatio_pr100(Periph, Channel)	( PWM[ PWM_Save_GetTimPeriph(Periph) ][ PWM_Save_GetTimChannel(Channel) ].SaveRatio_pr100   )
#define	__GetRunningState(Periph, Channel)	( PWM[ PWM_Save_GetTimPeriph(Periph) ][ PWM_Save_GetTimChannel(Channel) ].RunningState   )
#define __GetConfState(periph, channel)		( PWM[ PWM_Save_GetTimPeriph(periph) ][ PWM_Save_GetTimChannel(channel) ].ConfState )
#define __SetConfState(periph, channel, val)	( PWM[ PWM_Save_GetTimPeriph(periph) ][ PWM_Save_GetTimChannel(channel) ].ConfState = val )
#define	__SaveRatio_pr100(Ratio, Periph, Channel)	( PWM[ PWM_Save_GetTimPeriph(Periph) ][ PWM_Save_GetTimChannel(Channel) ].SaveRatio_pr100   = Ratio 	)
#define	__SetRunningState(bool, Periph, Channel)	( PWM[ PWM_Save_GetTimPeriph(Periph) ][ PWM_Save_GetTimChannel(Channel) ].RunningState 	    = bool	)
#define	__SavePeriod_uS(Periode_us, Periph, Channel)	( PWM[ PWM_Save_GetTimPeriph(Periph) ][ PWM_Save_GetTimChannel(Channel) ].SavePeriode_us    = Periode_us)

#define __SetPWMOffString(str)				__strncat(  str, 	 OFF_STRING, strlen(OFF_STRING))
#define __SetRatioString(str, Ratio)			__strncat  ( str, Ratio, NB_CHAR_POURCENTAGE )

#define __PeriodeToFrequencyString(str, periode) 	__snprintf ( str, 3, "%d", 		(int)(1000000 / periode)	)
#define __RatioToString(str, ratio)		 	__snprintf ( str, NB_CHAR_POURCENTAGE, 	" %d pr100", 	ratio		)




/********************************************************************
 * Private Types
 */
typedef enum {

	PWM_Stopped = 0,
	PWM_Running,
	PWM_OK,

}Liste_Etat_PWM_e;

typedef enum {

	PWM_Unconfigured = 0,
	PWM_Configured,

	err_PWM_AlreadyConfigured,
	err_PWM_CantBeConfigured,

}Liste_EtatConf_PWM_e;


/** Structure d'une PWM */
typedef struct {

	uint32_t 		SavePeriode_us;
	uint8_t 		SaveRatio_pr100;

	Liste_Etat_PWM_e 	RunningState;
	Liste_EtatConf_PWM_e	ConfState;

	PWM_PulseMode_e		PulseMode;
	pFunction		IRQ_Handle;

}PWM_s;

/********************************************************************
 * Private variables
 */
static PWM_s	PWM[nb_TimPeriph][nb_TimChannels];

/********************************************************************
 * Private Fonctions Prototypes
 */
inline static void 		 PWM_initOutputCompare		(Mapping_GPIO_e	IdPinPwm, uint32_t TIM_Periode, uint32_t Ratio_pr100);
inline static void 		 PWM_initTimeBase_RccInit	(uint32_t Periph);
inline static ListePeriphTim_e 	 PWM_Save_GetTimPeriph		(uint32_t Periph);
inline static ListeTimChannels_e PWM_Save_GetTimChannel		(uint32_t Channel);
inline static void 		 PWM_IT_init			(uint32_t Periph, uint32_t Channel);

/********************************************************************
 * Exported Fonctions Definition
 */
/**------------------------------------------------------------------
 *
 * @brief	Activation du signal PWM
 *
 */
uint8_t
Pwm_Activer(
		Mapping_GPIO_e IdPinPwm		/** <[in] ID de la PIN o� g�n�rer le signal PWM */
) {

	Liste_Etat_PWM_e PWM_State;

	//--------- Verif que periph conf
	if( __GetConfState(Mapping_GPIO[IdPinPwm].Periph, Mapping_GPIO[IdPinPwm].Parametre) ==  PWM_Unconfigured)
		return PWM_Unconfigured;

	PWM_State = __GetRunningState(	 Mapping_GPIO[IdPinPwm].Periph,
				 	 Mapping_GPIO[IdPinPwm].Parametre );

	if(PWM_State == PWM_Running)		return PWM_Running;



	__SetRunningState(	PWM_Running,
				 Mapping_GPIO[IdPinPwm].Periph,
				 Mapping_GPIO[IdPinPwm].Parametre );

	TIM_CCxCmd	  (	(TIM_TypeDef*) Mapping_GPIO[IdPinPwm].Periph,
				Mapping_GPIO[IdPinPwm].Parametre,
				TIM_CCx_Enable					);

	TIM_CtrlPWMOutputs(	(TIM_TypeDef*) Mapping_GPIO[IdPinPwm].Periph,
				ENABLE						);

	if(Mapping_GPIO[IdPinPwm].Etat_Interruption == Interrupt_ON)
		TIM_ITConfig((TIM_TypeDef* )Mapping_GPIO[IdPinPwm].Periph,  __GetITChannel(Mapping_GPIO[IdPinPwm].Parametre), ENABLE);



	return PWM_Running;
}


/**------------------------------------------------------------------
 *
 * @brief	Desactivation du signal PWM
 *
 */
uint8_t
Pwm_Desactiver(
		Mapping_GPIO_e IdPinPwm		/** <[in] ID de la PIN o� le signal PWM �tait g�n�r� */
) {

	Liste_Etat_PWM_e PWM_State;

	//--------- Verif que periph conf
	if( __GetConfState(Mapping_GPIO[IdPinPwm].Periph, Mapping_GPIO[IdPinPwm].Parametre) ==  PWM_Unconfigured)
		return PWM_Unconfigured;

	PWM_State = __GetRunningState(	Mapping_GPIO[IdPinPwm].Periph,
					Mapping_GPIO[IdPinPwm].Parametre );

	if(PWM_State == PWM_Stopped)		return PWM_Stopped;



	__SetRunningState(	FALSE,
				 Mapping_GPIO[IdPinPwm].Periph,
				 Mapping_GPIO[IdPinPwm].Parametre );

	TIM_CCxCmd	  (	(TIM_TypeDef*) Mapping_GPIO[IdPinPwm].Periph,
				Mapping_GPIO[IdPinPwm].Parametre,
				TIM_CCx_Disable					);

	TIM_CtrlPWMOutputs(	(TIM_TypeDef*) Mapping_GPIO[IdPinPwm].Periph,
				DISABLE						);

	/*if(Mapping_GPIO[IdPinPwm].Etat_Interruption == Interrupt_ON)
		TIM_ITConfig((TIM_TypeDef* )Mapping_GPIO[IdPinPwm].Periph,  __GetITChannel(Mapping_GPIO[IdPinPwm].Parametre), DISABLE);*/



	return PWM_Stopped;
}

/**------------------------------------------------------------------
 * @brief	Initialisation de la PWM
 *
 */
uint8_t
PWM_Init(
		Mapping_GPIO_e	IdPinPwm,	/**<[in] ID de la Pin ou g�n�rer la PWM*/
		uint32_t 	Periode_us,	/**<[in] Periode de comptage */
		uint32_t	Ratio_pr100	/**<[in] Ratio du signal PWM en % */
) {

	uint32_t 	TIM_Periode 	= 1;	/** Periode de comptage du TimeBas */
	uint32_t 	TIM_Prescaler 	= 1;	/** Prescaler du TimeBase */
	uint32_t	SizeTimerMax	= 0;

	//-------- Test sur l'existence de la Pin dans le BSP
	if (IdPinPwm >= nb_GPIO)
		return Err_PWM_Unavailable;

	//-------- Test sur la periph demande
	if (__IsPeriphTimer(Mapping_GPIO[IdPinPwm].Periph))
		return err_PWM_CantBeConfigured;

	//--------- Verif que pas deja conf
	if( __GetConfState(Mapping_GPIO[IdPinPwm].Periph, Mapping_GPIO[IdPinPwm].Parametre) ==  PWM_Configured)
		return err_PWM_AlreadyConfigured;


	//------- Calcul de la periode et du prescaler du Timer
	TIM_Prescaler = (((SystemCoreClock / 1000000) / 4) - 1);
	TIM_Periode = (Periode_us  * 2) - 1;

	//------- Recuperation de la taille max du timer
		if(__IsTimerA32BitsOne(Mapping_GPIO[IdPinPwm].Periph)) 		SizeTimerMax = SIZEOF_TIMER_32BITS;
	else 	if(__IsTimerA16BistOne(Mapping_GPIO[IdPinPwm].Periph))		SizeTimerMax = SIZEOF_TIMER_16BITS;


	//------- Verification que la taille n'est pas d�pass�e
	while (	TIM_Periode >= SizeTimerMax )	{

		TIM_Prescaler <<= 1;
		TIM_Periode >>= 1;
	}

	TimeBase_init		(Mapping_GPIO[IdPinPwm].Periph,	TIM_Periode, 	TIM_Prescaler	);
	PWM_initOutputCompare	(IdPinPwm, 	TIM_Periode, 	Ratio_pr100	);

	//--------- Configuration interruption
	if( Mapping_GPIO[IdPinPwm].Etat_Interruption == Interrupt_ON ) {

		PWM_IT_init(	Mapping_GPIO[IdPinPwm].Periph,
				Mapping_GPIO[IdPinPwm].Parametre	);
	}

	//--------- Activation du Timer
	TIM_Cmd( (TIM_TypeDef*) Mapping_GPIO[IdPinPwm].Periph, ENABLE );

	if(Mapping_GPIO[IdPinPwm].EtatInit == ETAT_ACTIF) {

		Pwm_Activer(IdPinPwm);

	} else {

		__SetRunningState(	PWM_Stopped,
					Mapping_GPIO[IdPinPwm].Periph,
					Mapping_GPIO[IdPinPwm].Parametre );
	}

	//--------- Sauvegarde de la periode
	__SavePeriod_uS( Periode_us,
			 Mapping_GPIO[IdPinPwm].Periph,
			 Mapping_GPIO[IdPinPwm].Parametre );

	__SetConfState( Mapping_GPIO[IdPinPwm].Periph,
			Mapping_GPIO[IdPinPwm].Parametre,
			PWM_Configured			  );

	return PWM_OK;
}

/**------------------------------------------------------------------
 * @brief	Desinitialisation de la PWM
 *
 */
void
PWM_Desinit(
			Mapping_GPIO_e	IdPinPwm		/**<[in] ID de la Pin ou g�n�rer la PWM*/
) {
	TIM_DeInit( (TIM_TypeDef*) Mapping_GPIO[IdPinPwm].Periph );

	__SavePeriod_uS( 0,
			 Mapping_GPIO[IdPinPwm].Periph,
			 Mapping_GPIO[IdPinPwm].Parametre );

	__SetRunningState( PWM_Stopped,
			   Mapping_GPIO[IdPinPwm].Periph,
			   Mapping_GPIO[IdPinPwm].Parametre );

	__SetConfState( Mapping_GPIO[IdPinPwm].Periph,
			Mapping_GPIO[IdPinPwm].Parametre,
			PWM_Unconfigured			);
}

/**------------------------------------------------------------------
 * @brief	Lecture de la configuration d'une PWM
 *
 * @return	void
 *
 */
void
PWM_GetConfiguration(
			Mapping_GPIO_e	IdPinPwm,	/**<[in] ID de la Pin ou g�n�rer la PWM */
			uint32_t* 	Periode_us,	/**<[out] Periode de la PWM en microsecondes */
			uint8_t*	Ratio_pr100	/**<[out] Ratio de la PWM en pr100 */
) {

	Liste_Etat_PWM_e PWM_Running;

	PWM_Running = __GetRunningState( Mapping_GPIO[IdPinPwm].Periph,
					 Mapping_GPIO[IdPinPwm].Parametre );

	if(PWM_Running == PWM_Stopped) {
		*Periode_us  = 0;
		*Ratio_pr100 = 0;
		return;
	}

	*Periode_us = __GetPeriod_uS( 	Mapping_GPIO[IdPinPwm].Periph,
					Mapping_GPIO[IdPinPwm].Parametre );


	*Ratio_pr100 = __GetRatio_pr100(  Mapping_GPIO[IdPinPwm].Periph,
			  	  	  Mapping_GPIO[IdPinPwm].Parametre );
}


/********************************************************************
 * Private Fonctions Definition
 */

/**------------------------------------------------------------------
 *
 * @brief	Init Clock APB1 ou APB2 en fonction du TIM
 *
 */
inline static void
PWM_initTimeBase_RccInit(
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

/**------------------------------------------------------------------
 *
 * @brief	Lecture du periph TIM
 *
 * @return	Periph
 *
 */
inline static ListePeriphTim_e
PWM_Save_GetTimPeriph(
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

/**------------------------------------------------------------------
 *
 * @brief	Lecture du periph TIM
 *
 * @return	Channel
 *
 */
inline static ListeTimChannels_e
PWM_Save_GetTimChannel(
		uint32_t	Channel
) {
	switch(Channel) {

		case (uint32_t) TIM_Channel_1:	return Periph_TimChannel1;	break;
		case (uint32_t) TIM_Channel_2:	return Periph_TimChannel2;	break;
		case (uint32_t) TIM_Channel_3:	return Periph_TimChannel3;	break;
		case (uint32_t) TIM_Channel_4:	return Periph_TimChannel4;	break;

		default:			return err_NotATimChannel;	break;
	}
}


/**------------------------------------------------------------------
 *
 * @brief	Initialisation de OutputCompare
 *
 * @warning La Pin doit �tre configur�
 *
 */
inline static void
PWM_initOutputCompare(
		Mapping_GPIO_e	IdPinPwm,		/**<[in] ID de la Pin ou g�n�rer la PWM*/
		uint32_t 		TIM_Periode,	/**<[in] Periode de comptage */
		uint32_t		Ratio_pr100	/**<[in] Ratio du signal PWM en % */
) {

	//----------------------------------------------------------------------------
	//------------------------ D�claration et Initialisation ---------------------
	//----------------------------------------------------------------------------
	//---------- D�claration des variables
	uint32_t 		PulseLength_ms;				/** Duree de l'etat haut de la PWM*/

	TIM_OCInitTypeDef 	TIM_OCInit;				/** Structure d'initialisation d'outputcompare */
	pFunction 		pOCxInit;
	pFunction 		pOCxPreloadingConfig;

	TIM_OCStructInit	(&TIM_OCInit);


	//----------------------------------------------------------------------------
	//----------------------- Initialisation OutputCompare -----------------------
	//----------------------------------------------------------------------------
	//---------- Remplissage de la structure d'init TimOutputCompare
	TIM_OCInit.TIM_OutputState 	= TIM_OutputState_Disable;
	TIM_OCInit.TIM_OCPolarity 	= TIM_OCPolarity_High;
	TIM_OCInit.TIM_OCMode 		= TIM_OCMode_PWM1;

	//---------------- Sauvegarde du ratio
	__SaveRatio_pr100( Ratio_pr100,
			   Mapping_GPIO[IdPinPwm].Periph,
			   Mapping_GPIO[IdPinPwm].Parametre );

	//---------- Calcul de la duree de l'etat haut
	if (Ratio_pr100 >= 100)	{
		TIM_OCInit.TIM_Pulse = TIM_Periode;
	} else	{
		PulseLength_ms  = Ratio_pr100 * TIM_Periode;
		PulseLength_ms /= 100;
		TIM_OCInit.TIM_Pulse = PulseLength_ms;
	}

	//----------  Choix du canal a configurer
	switch (Mapping_GPIO[IdPinPwm].Parametre)	{

		case TIM_Channel_1:	pOCxInit 		= (pFunction)TIM_OC1Init;
					pOCxPreloadingConfig	= (pFunction)TIM_OC1PreloadConfig;
					break;

		case TIM_Channel_2:	pOCxInit 		= (pFunction)TIM_OC2Init;
					pOCxPreloadingConfig	= (pFunction)TIM_OC2PreloadConfig;
					break;

		case TIM_Channel_3:	pOCxInit 		= (pFunction)TIM_OC3Init;
					pOCxPreloadingConfig	= (pFunction)TIM_OC3PreloadConfig;
					break;

		case TIM_Channel_4:	pOCxInit 		= (pFunction)TIM_OC4Init;
					pOCxPreloadingConfig	= (pFunction)TIM_OC4PreloadConfig;
					break;

		default:		return;
	}

	pOCxInit		( (TIM_TypeDef*) Mapping_GPIO[IdPinPwm].Periph, &TIM_OCInit		);
	pOCxPreloadingConfig	( (TIM_TypeDef*) Mapping_GPIO[IdPinPwm].Periph, TIM_OCPreload_Enable	);
}


/**------------------------------------------------------------------
 *
 * @brief	Init IT
 *
 */
inline static void
PWM_IT_init(
	uint32_t Periph,
	uint32_t Channel
) {
	NVIC_InitTypeDef 	NVIC_InitStructure;

	switch(Periph) {

		case (uint32_t) TIM2: 	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
					break;

		case (uint32_t) TIM3: 	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
					break;

		default:		return;

	}


	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 12;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 		= 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd 			= ENABLE;
	NVIC_Init( &NVIC_InitStructure );

	TIM_ITConfig((TIM_TypeDef* )Periph, __GetITChannel(Channel), DISABLE);
}

/**------------------------------------------------------------------
 *
 * @brief	ToString Valeur PWM
 *
 */
inline void
PWM_Value_toString(

		toString_Possibilities_e	Field,
		Mapping_GPIO_e 			IDMapping,
		uint8_t*			pString
) {
	uint32_t	Periode_us;
	uint8_t		Ratio_pr100;
	uint8_t		Ratio[NB_CHAR_POURCENTAGE];
	uint8_t		Freq[NB_CHAR_FREQUENCY];

	memset(Ratio, 0, NB_CHAR_POURCENTAGE);
	memset(Freq, 0, NB_CHAR_FREQUENCY);

	switch(Field) {

		//----------------------------------------------------------
		case toString_Getpin:	toString_GetPeriphral	(Mapping_GPIO[IDMapping].GpioPeripheral,pString);
					toString_GetPin		(Mapping_GPIO[IDMapping].GpioPin, 	pString);
					break;

		//----------------------------------------------------------
		case toString_GetValue:	PWM_GetConfiguration(IDMapping, &Periode_us, &Ratio_pr100);

					if(Periode_us != 0) {

						if(Periode_us < 1000) {
							__PeriodeToFrequencyString (Freq, (Periode_us*100));
							__VT100STRING_SET_FOREGROUND_COLOR(pString, Couleur_verte);
							__SetFrequencyString(pString, Freq);
							__SetFrequencyUnit_KHz(pString);
						} else {
							__PeriodeToFrequencyString (Freq, Periode_us);
							__VT100STRING_SET_FOREGROUND_COLOR(pString, Couleur_verte);
							__SetFrequencyString(pString, Freq);
							__SetFrequencyUnit_Hz(pString);
						}
						__RatioToString	  (Ratio,   Ratio_pr100);
						__SetRatioString  (pString, Ratio);
						__VT100STRING_SET_FOREGROUND_COLOR(pString, Couleur_blanc);

					} else {
						__VT100STRING_SET_FOREGROUND_COLOR(pString, Couleur_rouge);
						__SetPWMOffString(pString);
						__VT100STRING_SET_FOREGROUND_COLOR(pString, Couleur_default);
					}break;

		//----------------------------------------------------------
		default:
					break;
	}
}
