/***************************************************************************************************
 ***************************************************************************************************
 *
 * @author	Duclos Timothe
 * @date	11/12/2014
 * @file	util_Inversion.c
 * @brief
 *
 ***************************************************************************************************/


/***************************************************************************************************
 * Includes
 */
#include "util_Inversion.h"

/***************************************************************************************************
 * Private defines
 */

/***************************************************************************************************
 * Private macros
 */

/***************************************************************************************************
 * Private Types
 */

/***************************************************************************************************
 * Private variables
 */
/***************************************************************************************************
 * Private Functions prototypes
 */

/***************************************************************************************************
 * Exported Functions definition
 */
/**-------------------------------------------------------------------------------------------------
 * @brief	Inversion d'un tableau de char
 */
void
Inversion_TabChar(
	uint8_t* Tab,		/**<[in] Pointeur sur le tableau a inverser */
	uint32_t Size_octet	/**<{in] Nombre d'octet (=char) composant le tableau */
) {

	uint8_t* tab_end;
	uint8_t* tab_temp = (uint8_t *) malloc((Size_octet+1) * sizeof(uint8_t));

	memset(tab_temp, 0, Size_octet);

	tab_end  = Tab 		- 1;
	Tab 	+= Size_octet	- 1;

	do 	{	*(tab_temp++) = *(Tab--);	}
	while	( (uint32_t) Tab != (uint32_t) tab_end	);

	Tab++;
	tab_temp -= Size_octet;

	__strncpy(Tab, tab_temp, Size_octet);

	free(tab_temp);
}

/**-------------------------------------------------------------------------------------------------
 * @brief	Inversion d'une valeur uint32_t : LSB <-> MSB
 */
void
Inversion_LSBMSB_uint8(
	uint8_t* Value		/**<[in] Pointeur sur la valeur a inverser */
) {

	uint8_t  b_Inversion = 0, MASK = 0x1;
	uint8_t* value_temp = (uint8_t *) malloc(sizeof(uint8_t));
	uint8_t valueInv = 0;

	memset(value_temp, 0, 1);

	MASK <<= 7;

	for(b_Inversion=0; b_Inversion<8; b_Inversion++) {

		*value_temp = ( *Value & (MASK >> b_Inversion) ) >> ((8 - 1) - b_Inversion);
		valueInv |= *value_temp << b_Inversion;
	}

	* Value = valueInv;

	free(value_temp);
}

void
Inversion_LSBMSB(
	void* 		Value,
	uint32_t	Size_octets
) {

	uint32_t  b_Inversion;
	uint32_t* value_temp = (uint32_t *) malloc(sizeof(uint32_t));

	memset(value_temp, 0, 1);

	for(b_Inversion=0; b_Inversion< (8 * Size_octets); b_Inversion++) {

	}

	free(value_temp);
}

/***************************************************************************************************
 * Private Functions definition
 */
