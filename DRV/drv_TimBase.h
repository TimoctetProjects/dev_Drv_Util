/*********************************************************************
 * *******************************************************************
 *
 * @file	drv_TimBase.h
 *
 * @author	Duclos Timothe
 *
 * @date	21/11/2014
 *
 * @brief	Driver gestion TimBase
 *
 *********************************************************************/

#ifndef DRV_TIMBASE_H
#define DRV_TIMBASE_H

/********************************************************************
 * Includes
 */
#include "BSP_carte.h"

/********************************************************************
 * Exported defines
 */

/********************************************************************
 * Exported variables
 */

/********************************************************************
 * Exported types
 */
typedef enum {

	Periph_Tim1 = 0,
	Periph_Tim2,
	Periph_Tim3,
	Periph_Tim4,
	Periph_Tim5,
	Periph_Tim6,
	Periph_Tim7,
	Periph_Tim8,
	Periph_Tim9,
	Periph_Tim10,
	Periph_Tim11,
	Periph_Tim12,
	Periph_Tim13,
	Periph_Tim14,

	nb_Periph,

	err_NotATimPeriph

}ListePeriphTim_e;

typedef enum {

	Periph_TimChannel1 = 0,
	Periph_TimChannel2,
	Periph_TimChannel3,
	Periph_TimChannel4,

	nb_TimChannels,

	err_NotATimChannel

}ListeTimChannels_e;

/********************************************************************
 * Exported Function
 */
void TimeBase_init(
	Mapping_GPIO_e	IdPinPwm,		/**<[in] ID de la Pin ou générer la PWM*/
	uint32_t 	TIM_Periode,		/**<[in] Periode de comptage */
	uint32_t 	TIM_Prescaler 		/**<[in] Prescaler du TimeBase */
);

#endif
