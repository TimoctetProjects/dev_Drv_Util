/*********************************************************************
 * *******************************************************************
 *
 * @author	Duclos Timothe
 *
 * @date	09/12/2014
 *
 * @brief	Main
 *
 *********************************************************************
 *********************************************************************/


/********************************************************************
 * Includes
 */
#include "app_AffDebug.h"
#include "util_Inversion.h"

//#define DEBUG_ON

/**-------------------------------------------------------------------
 * Main function
 *
 */
int
main(
	void
){

	TSW_New(TSW_AffichageRefresh);

	//--------------------------------------------------------------
	//------------ Initialisations
	BSP_Init();
	SysTick_Config(CONFIG_SYSCLOCK_1ms);

	if(Inversion_Validation() == STATUS_OK)
		__NOP;

	while(1) {

		Console_main();

		#ifdef  DEBUG_ON
			//
			if( __TSW_isFinished(TSW_AffichageRefresh) == TRUE ) {
				TSW_Start(&TSW_AffichageRefresh, 50);
				Affichage_Main();
			}	setDureeTourBoucle();
		#endif
	}
}
