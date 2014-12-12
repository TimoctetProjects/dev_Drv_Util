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


/**-------------------------------------------------------------------
 * Main function
 *
 */
int
main(
	void
){
	uint8_t test;
	uint8_t test2 = 0xF0;

	TSW_New(TSW_AffichageRefresh);

	//--------------------------------------------------------------
	//------------ Initialisations
	BSP_Init();
	SysTick_Config(CONFIG_SYSCLOCK_1ms);

	Inversion_LSBMSB_uint8(&test2);
	__NOP;

	while(1) {


		#ifdef  APP_AFFDEBUG_H
			Console_main();
			//TSW_VALIDATION();
			if( __TSW_isFinished(TSW_AffichageRefresh) == TRUE ) {
				TSW_Start(&TSW_AffichageRefresh, 50);
				Affichage_Main();
			}	setDureeTourBoucle();
		#endif
	}
}
