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
#include "drv_TimBase.h"
#include "util_Inversion.h"

#define DEBUG_ON

static TSW_s TSW_AffichageRefresh;

/**-------------------------------------------------------------------
 * Main function
 *
 */
int
main(
	void
){
	TSW_Start(&TSW_AffichageRefresh, 50);

	//--------------------------------------------------------------
	//------------ Initialisations
	BSP_Init();
	SysTick_Config(CONFIG_SYSCLOCK_1ms);

	while(1) {

		Console_main();

		#ifdef  DEBUG_ON
			//
			if __TSW_isFinished(TSW_AffichageRefresh) {
				TSW_Start(&TSW_AffichageRefresh, 50);
				Affichage_Main();
			}	setDureeTourBoucle();
		#endif
	}
}
