/*********************************************************************
 * *******************************************************************
 *
 * @file	util_Console.c
 *
 * @author	Duclos Timothe
 *
 * @date	17/09/2014
 *
 * @brief	Utilitaire d'affichage console
 *
 *********************************************************************/

/********************************************************************
 * Includes
 */
#include "util_Console.h"

/********************************************************************
 * Private defines
 */
#define PERIODE_REFRESH_PING_TERMINAL_ms	100
#define REPONSE_TERMINAL			255
#define VITESSE_USART_DEBUG_bauds		115200

/********************************************************************
 * Private Types
 */

typedef enum {

	Console_Initialisation = 0,
	Console_Ping,
	Console_ReponsePing,
	Console_AttenteNouveauPing,

}Console_Etape_e;

typedef struct {

	Bool_e			TerminalPresent;
	Liste_Usart_Periph_e	Periph_Usart;
	Mapping_GPIO_e		ID_Pin_TX;
	Mapping_GPIO_e		ID_Pin_RX;

}Console_s;

/********************************************************************
 * Private variables
 */
static Console_s Console = {FALSE, Periph_USART6, CONSOLE_TX, PIN_NULL};


/********************************************************************
 * Private Fonction Prototype
 */

inline uint8_t
Console_Init(
	void
);

inline uint8_t
Console_PingTerminal(
	void
);

inline uint8_t
Console_GetResp(
	void
);

inline uint8_t
Console_AttenteNvPing(
		void
);

/********************************************************************
 * Extern Fonction Definition
 */


/*
 * Faire que si le terminal n'est pas la on desinit l'usart (faire attention, peut etre pas une tres bonne idee)
 * Faire attention a que tout les console send n'opere que s'il y a un terminal
 *
 * Faire getter de Presence Terminal
 *
 * Entrer Pin RX et TX dans structure USART
 * Faire fonctionner les drivers USART et PWM avec leur PERIPH_ENUM au lieu des pins (sauf pour l'init...)
 *         ET BIEN FAIRE ATTENTION QUE RIEN NE SE PASSE SI NON-INITIALISER
 *
 */

/**------------------------------------------------------------------
 *
 * @brief	Gestion de la console
 *
 * @return	void
 *
 */
void
Console_main(
	void
) {
	static Console_Etape_e Etape = Console_Initialisation;

	switch(Etape) {

		case Console_Initialisation:	   Etape = Console_Init();		break;
		default:								break;
	}
}

/**------------------------------------------------------------------
 *
 * @brief	Lecture de la presence console
 *
 * @return	TRUE (present) ou FALSE (absent)
 *
 */
Bool_e
Console_GetPresence(
	void
) {
	return Console.TerminalPresent;
}

/********************************************************************
 * Private Fonction Definition
 */

/**------------------------------------------------------------------
 *
 * @brief	Init de la console
 *
 * @return	Etape suivante
 *
 */
inline uint8_t
Console_Init(
	void
) {

	Usart_InitPeriph( Console.ID_Pin_TX,
			  Console.ID_Pin_RX,
			  VITESSE_USART_DEBUG_bauds );

	__VT100CMD_CLEAR_SCREEN;
	__VT100CMD_MOVECURSOR_TOPLEFTUP;
	__VT100CMD_SWITCH_CURSOR_INVISIBLE;
	__VT100CMD_SET_FOREGROUND_COLOR(Couleur_blanc);

	return Console_Ping;
}

/**------------------------------------------------------------------
 *
 * @brief	Pinger le terminal afin de n'envoyer les commandes que
 * 		s'il y en a un
 *
 * @return	void
 *
 */
inline uint8_t
Console_PingTerminal(
	void
) {
	//TSW_Start(Timer_IdentifyTerminal, PERIODE_REFRESH_PING_TERMINAL_ms);
	//USART_SetInterruptState(Console.Periph_Usart, Usart_Pin_Rx, ENABLE);
	__VT100CMD_IDENTIDY_TERMINAL;
	return Console_ReponsePing;
}

/**------------------------------------------------------------------
 *
 * @brief	Scruter la réponse du Terminal et relancer le ping
 *
 * @return	void
 *
 */
inline uint8_t
Console_GetResp(
	void
) {

	uint16_t TermAnswer = 0;

	//USART_Read((uint32_t) USART6, &TermAnswer, 1);

	if( USART_GetFlagStatus(USART6, USART_FLAG_RXNE) == SET) {
		TermAnswer = (uint8_t) USART_ReceiveData(USART6);
	}

	if( USART_GetFlagStatus(USART6, USART_FLAG_RXNE) == SET) {
		TermAnswer = (uint8_t) USART_ReceiveData(USART6);
	}

	if	(TermAnswer == REPONSE_TERMINAL)		Console.TerminalPresent = TRUE;
	else						{	Console.TerminalPresent = FALSE;
								//return Console_ReponsePing;
							}

	//USART_SetInterruptState(Console.Periph_Usart, Usart_Pin_Rx, DISABLE);

	return Console_AttenteNouveauPing;
}

/**------------------------------------------------------------------
 *
 * @brief	Attente de la fin du timer afin de générer un nouveau ping
 *
 * @return	void
 *
 */
inline uint8_t
Console_AttenteNvPing(
		void
) {
	/*if(TSW_GetStatus(Timer_IdentifyTerminal) == STATUS_FINIS) 	return Console_Ping;
	return Console_AttenteNouveauPing;*/
}
