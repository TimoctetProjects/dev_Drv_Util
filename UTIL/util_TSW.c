/***************************************************************************************************
 ***************************************************************************************************
 *
 * @author	Duclos Timothe
 * @date	09/12/2014
 * @file	util_TSW.c
 * @brief	Timer software
 *
 * @note	How to use this utilitie
 *
 * 		First declare a TSW using these following methods:
 * 			- local variable:  	use the macro @ref __TSW_New
 * 						with the name you want
 * 			- global variable:	use the typedef @ref TSW_s
 * 		This is done like this so the TSW is in a reset state
 *
 * 		Then start the Timer using @ref TSW_Start with the value
 * 		expressed in milliseconds
 *
 * 		To stop the Timer use @ref TSW_Stop
 *
 * 		To know when it is finished, use the macro @ref __TSW_IsFinished
 *
 * 		To know about the remaining or elapsing time use these following functions
 * 			- @ref TSW_GetRemainingTime
 * 			- @ref TSW_GetElapsingTime
 *
 * 		You can re-start the timer before its finished if you want (watchdog use)
 *
 ***************************************************************************************************/


/***************************************************************************************************
 * Includes
 */
#include "util_TSW.h"

/***************************************************************************************************
 * Private defines
 */
#define TAILLE_TSW_32_bits	0xFFFFFFFF

/***************************************************************************************************
 * Private macros
 */
#define __TSW_IsIDValid(id)	( (id >= 0) && (id < nb_TIMERS) )

/***************************************************************************************************
 * Private Types
 */

/***************************************************************************************************
 * Private variables
 */
volatile 	uint32_t 	msTicks 	= 0;
static  	TSW_s		FirstTSW	= {.NextTimer = NULL};
static		TSW_s*		CurrentTSW 	= &FirstTSW;

/***************************************************************************************************
 * Private Functions prototypes
 */
static inline void DeleteTSW(TSW_s* DoomedTimer);
static inline void InsertTSW(TSW_s* NewTimer);

/***************************************************************************************************
 * Exported Functions definition
 */
/**-------------------------------------------------------------------------------------------------
 * @brief
 */
void
TSW_Start(
	TSW_s*	 Timer,		/**<[in] */
	uint32_t Value_ms	/**<[in] */
) {

	if(Timer->Status != STATUS_ENCOURS)	InsertTSW(Timer);

	Timer->Etat 		= ETAT_ACTIF;
	Timer->Status 		= STATUS_ENCOURS;
	Timer->Stop_Value_ms 	= msTicks + Value_ms;
	Timer->Start_Value_ms 	= msTicks;
}

/**-------------------------------------------------------------------------------------------------
 * @brief
 */
void
TSW_StructInit(
	TSW_s*	 Timer	/**<[in] */
) {

	Timer->Etat		= ETAT_INACTIF;
	Timer->NextTimer	= NULL;
	Timer->Pause_Value_ms	= 0;
	Timer->PreviousTimer	= NULL;
	Timer->Start_Value_ms	= 0;
	Timer->Status		= STATUS_FINIS;
	Timer->Stop_Value_ms	= 0;
}

/**-------------------------------------------------------------------------------------------------
 * @brief
 */
void
TSW_Reset(
	TSW_s*	 Timer	/**<[in] */
) {

	DeleteTSW(Timer);
	Timer->Etat 		= ETAT_INACTIF;
	Timer->Status		= STATUS_FINIS;
	Timer->Stop_Value_ms 	= 0;
	Timer->NextTimer	= NULL;
	Timer->PreviousTimer	= NULL;
}


/**-------------------------------------------------------------------------------------------------
 * @brief	Suspension du timer
 * @return	Valeur restante
 */
uint32_t
TSW_Suspend(
	TSW_s*	 		Timer,	/**<[in] */
	TSW_ListeChoixValeurs	Choix	/**<[in] */
) {

}

/**-------------------------------------------------------------------------------------------------
 * @brief	Lecture du Timestamp
 * @return	Valeur du timestamp en millisecondes
 */
uint32_t
TSW_GetTimestamp(
		void
) {

	return msTicks;
}

/**-------------------------------------------------------------------------------------------------
 * @brief	Lecture du temps restant
 * @return	Valeur du temps restant en millisecondes
 */
uint32_t
TSW_GetRemainingTime(
		TSW_s* Timer	/**<[in] */
) {

	if(Timer->Status == STATUS_FINIS)	return 0;
	return (Timer->Stop_Value_ms - msTicks);
}

/**-------------------------------------------------------------------------------------------------
 * @brief	Lecture du Temps écoulé
 * @return	Valeur du temps écoule en millisecondes
 */
uint32_t
TSW_GetElapsedTime(
		TSW_s* Timer	/**<[in] */
) {
	return (msTicks - Timer->Start_Value_ms);
}

/**-------------------------------------------------------------------------------------------------
 * @brief 	Interruption Systick genere toutes les @ref CONFIG_SYSCLOCK_1ms
 */
void
SysTick_Handler(
	void
) {

	msTicks++;
	if(msTicks == TAILLE_TSW_32_bits) {
		msTicks=0;
	}

	while(CurrentTSW->NextTimer != NULL) {

		CurrentTSW = CurrentTSW->NextTimer;

		if(CurrentTSW->Stop_Value_ms <= msTicks) {

			CurrentTSW->Status = STATUS_FINIS;

			if((uint32_t)CurrentTSW != (uint32_t) &FirstTSW)
				DeleteTSW(CurrentTSW);
		}


	}	CurrentTSW = &FirstTSW;
}



/***************************************************************************************************
 * Private Functions definition
 */
/**-------------------------------------------------------------------------------------------------
 * @brief	Suppression d'un Timer dans la liste
 */
static inline void
DeleteTSW(
	TSW_s* DoomedTimer	/**<[in] Timer a supprimer de la liste */
) {

	((TSW_s*)DoomedTimer->NextTimer)->PreviousTimer	=	DoomedTimer->PreviousTimer;
	((TSW_s*)DoomedTimer->PreviousTimer)->NextTimer	=	DoomedTimer->NextTimer;
}

/**-------------------------------------------------------------------------------------------------
 * @brief	Ajout d'un Timer dans la liste
 */
static inline void
InsertTSW(
	TSW_s*	NewTimer	/**<[in] Timer a inserer dans la liste */
) {

	if(FirstTSW.NextTimer == NULL) {
		FirstTSW.NextTimer 	= NewTimer;
		NewTimer->NextTimer 	= NULL;
		NewTimer->PreviousTimer = &FirstTSW;
		return;
	}

	((TSW_s*)(FirstTSW.NextTimer))->PreviousTimer	= (void *)NewTimer;

	NewTimer->NextTimer 	= FirstTSW.NextTimer;
	FirstTSW.NextTimer	= NewTimer;
	NewTimer->PreviousTimer	= &FirstTSW;
}
