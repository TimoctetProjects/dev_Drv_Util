/***************************************************************************************************
 *
 * @file	util_Console.c
 * @author	Duclos Timothe
 * @date	17/09/2014
 * @brief	Utilitaire d'affichage console
 *
 ***************************************************************************************************/

/***************************************************************************************************
 * Includes
 */
#include "util_Console.h"

/***************************************************************************************************
 * Private defines
 */
#define PERIODE_REFRESH_PING_TERMINAL_ms	100
#define PERIODE_PING_TIMEOUT_ms			1
#define REPONSE_TERMINAL			27
#define VITESSE_USART_DEBUG_bauds		115200


/***************************************************************************************************
 * Private defines
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

/***************************************************************************************************
 * Private variables
 */
static Console_s Console = {FALSE, Periph_USART6, CONSOLE_TX, CONSOLE_RX};
static TSW_s Timer_IdentifyTerminal, Timer_PingTimeout;

/***************************************************************************************************
 * Private Fonction Prototype
 */
inline uint8_t Console_Init		(void);
inline uint8_t Console_PingTerminal	(void);
inline uint8_t Console_GetResp		(void);
inline uint8_t Console_AttenteNvPing	(void);


/***************************************************************************************************
 * Extern Fonction Definition
 */
/**-------------------------------------------------------------------------------------------------
 * @brief	Gestion de la console
 * @return	void
 */
void
Console_main(
	void
) {
	static Console_Etape_e Etape = Console_Initialisation;

	switch(Etape) {

		case Console_Initialisation:	   Etape = Console_Init();		break;
		case Console_Ping:		   Etape = Console_PingTerminal();	break;
		case Console_ReponsePing:	   Etape = Console_GetResp();		break;
		case Console_AttenteNouveauPing:   Etape = Console_AttenteNvPing();	break;
		default:								break;
	}
}

/**-------------------------------------------------------------------------------------------------
 * @brief	Lecture de la presence console
 * @return	TRUE (present) ou FALSE (absent)
 */
Bool_e
Console_GetPresence(
	void
) {
	return Console.TerminalPresent;
}


/***************************************************************************************************
 * Private Fonction Definition
 */
/**-------------------------------------------------------------------------------------------------
 * @brief	Init de la console
 * @return	Etape suivante
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

/**-------------------------------------------------------------------------------------------------
 * @brief	Pinger le terminal afin de n'envoyer les commandes que
 * 		s'il y en a un
 * @return	void
 */
inline uint8_t
Console_PingTerminal(
	void
) {
	TSW_Start(&Timer_IdentifyTerminal, 	PERIODE_REFRESH_PING_TERMINAL_ms);
	TSW_Start(&Timer_PingTimeout, 		PERIODE_PING_TIMEOUT_ms);
	__VT100CMD_IDENTIDY_TERMINAL;
	return Console_ReponsePing;
}

/**-------------------------------------------------------------------------------------------------
 * @brief	Scruter la réponse du Terminal et relancer le ping
 * @return	void
 */
inline uint8_t
Console_GetResp(
	void
) {

	uint8_t TermAnswer = 0;

	if(USART_Read((uint32_t) USART6, &TermAnswer, 1) != 0) {

		if	(TermAnswer == REPONSE_TERMINAL)	Console.TerminalPresent = TRUE;
		else						Console.TerminalPresent = FALSE;


		return Console_AttenteNouveauPing;
	}

	if(__TSW_isFinished(Timer_PingTimeout))		return Console_AttenteNouveauPing;

	Console.TerminalPresent = FALSE;
	return Console_ReponsePing;
}

/**-------------------------------------------------------------------------------------------------
 * @brief	Attente de la fin du timer afin de générer un nouveau ping
 * @return	void
 */
inline uint8_t
Console_AttenteNvPing(
		void
) {
	if __TSW_isFinished(Timer_IdentifyTerminal) 	return Console_Ping;
	return Console_AttenteNouveauPing;
}
