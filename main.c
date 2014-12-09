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
#include "drv_PWM.h"

/**-------------------------------------------------------------------
 * Main function
 *
 */
int
main(
	void
) {
	__TSW_New(TSW_AffichageRefresh);

	//--------------------------------------------------------------
	//------------ Initialisations
	BSP_Init();
	SysTick_Config(CONFIG_SYSCLOCK_1ms);

	while(1) {

		#ifdef  APP_AFFDEBUG_H
			Console_main();
			if( __TSW_IsFinished(TSW_AffichageRefresh) ) {
				TSW_Start(&TSW_AffichageRefresh, 50);
				Affichage_Main();
			}	setDureeTourBoucle();
		#endif
	}
}
