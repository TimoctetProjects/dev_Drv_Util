/*********************************************************************
 * *******************************************************************
 *
 * @file	util_FIFO.h
 *
 * @author	Duclos Timothe
 *
 * @date	16/10/2014
 *
 * @brief	Utilitaire de gestion des FiFos
 *
 *********************************************************************/

/** Prevent recursive inclusion */
#ifndef UTIL_FIFO_H
#define UTIL_FIFO_H

/********************************************************************
 * Includes
 */
#include "bsp/bsp_carte.h"

/********************************************************************
 * Exported defines
 */


/********************************************************************
 * Exported variables
 */

/********************************************************************
 * Exported types
 */

/** Etat d'une FIFO */
typedef enum {

	FiFoState_Empty = 0,
	FiFo_LastElement,		// On vient de lire le dernier element
	FiFo_DataDispo,			// Donne disponible
	FiFoState_Full,

}FiFo_State_e;

/** Structure d'une FIFO */
typedef struct {

	uint8_t*		pBuffer;	/** Buffer de la FIFO */

	volatile uint32_t	pIndexIn;	/** Pointeur d'entree de la FiFo */
	volatile uint32_t	pIndexOut;	/** Pointeur de sortie de la FiFo */

	FiFo_State_e		State;		/** Etat de la FiFo */

	uint32_t		FifoSize;	/** Taille de la Fifo */
	uint32_t		DataSize;	/** Taille de la donnee */

	uint32_t		IndexLenght;	/** Longueur dans le tableau */

}FiFo_s;

/********************************************************************
 * Exported Fonctions
 */


/**-----------------------------------------------------------------
 *
 * @brief	Initialiser une Fifo
 *
 * @return	Statut de l'initialisation
 *
 */
Status_e
FiFo_init(

		FiFo_s* pNew_FIFO,	/**<[in] Pointeur vers la FIFO. */
		uint32_t FiFo_Size,	/**<[in] Taille de la FIFO. */
		uint32_t Data_Size  	/**<[in] Nombre de paquets de 8 bits, exprime en octet. */
);


/**-----------------------------------------------------------------
 *
 * @brief	Depilation d'un message dans une fifo
 *
 * @return	Statut de la depilation
 *
 */
FiFo_State_e
FiFo_Pull(
		FiFo_s* pFIFO, 	/**<[in] Fifo a initialiser */
		void*	pMsg	/**<[in] Message vide. */
);

/**-----------------------------------------------------------------
 *
 * @brief	Empilation d'un message dans une fifo
 *
 * @return	Statut de l'empilation
 *
 */
Status_e
FiFo_Push(
		FiFo_s* pFiFo,		/**<[in] Fifo a vider */
		...			/**<[in] Donnee a ajouter. */
);

/**-----------------------------------------------------------------
 *
 * @brief	RaZ d'une Fifo
 *
 * @return	void
 *
 */
void
FiFo_Clear(
		FiFo_s* pFifo		/**<[in] Fifo a vider */
);

/**-----------------------------------------------------------------
 *
 * @brief	Lire l'etat d'une FiFo
 *
 * @return	void
 *
 */
FiFo_State_e
FiFo_GetState(
		FiFo_s* pFifo		/**<[in] FiFo dont on veut lire l'etat */
);




#endif //UTIL_FIFO_H
