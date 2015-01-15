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
#include "util_Inversion.h"

#define DEBUG_ON

static TSW_s TSW_AffichageRefresh, TSW_BurstRefresh;



void
ADC_ChannelConfigure(
	void
) {
	//------------------------------------------------------
	//--------- Declaration des variables
	ADC_CommonInitTypeDef 	ADC_CommonInitStruct;
	ADC_InitTypeDef 	xADC_Init;
	NVIC_InitTypeDef 	NVIC_InitStructure;

	//-------- Init structure
	ADC_CommonStructInit	(&ADC_CommonInitStruct);
	ADC_StructInit		(&xADC_Init);

	//-------- Remplissage structure
	ADC_CommonInitStruct.ADC_Mode			= ADC_Mode_Independent;
	ADC_CommonInitStruct.ADC_Prescaler		= ADC_Prescaler_Div2;
	ADC_CommonInitStruct.ADC_DMAAccessMode		= ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStruct.ADC_TwoSamplingDelay	= ADC_TwoSamplingDelay_5Cycles;

	xADC_Init.ADC_Resolution		= ADC_Resolution_12b;
	xADC_Init.ADC_ScanConvMode		= DISABLE;
	xADC_Init.ADC_ContinuousConvMode	= DISABLE;
	xADC_Init.ADC_ExternalTrigConvEdge	= ADC_ExternalTrigConvEdge_None;
	xADC_Init.ADC_ExternalTrigConv		= 0;
	xADC_Init.ADC_DataAlign			= ADC_DataAlign_Right;
	xADC_Init.ADC_NbrOfConversion		= 1;

	//---------- Init commune a tous les ADC
	ADC_CommonInit(&ADC_CommonInitStruct);

	//---------- Activation horloges
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	//--------- Init ADC
	ADC_Init( ADC1, &xADC_Init );

	//--------- Config interruption
	ADC_ITConfig( ADC1, ADC_IT_EOC, ENABLE );

	NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStructure );


	//---------------- Activation peripherique
	ADC_Cmd( ADC1, ENABLE );

	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_3Cycles);
}



static Bool_e Conv = FALSE;
static uint8_t	nbIT = 0;
static Bool_e DonneLue = FALSE;
uint32_t Ligne;
uint32_t ValCam[128];

void
TIM3_IRQHandler(
	void
) {
	TIM_ITConfig(TIM3, TIM_IT_CC1, DISABLE);

	/*if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

	else if(TIM_GetITStatus(TIM3, TIM_IT_CC1) == SET)
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);

	else if(TIM_GetITStatus(TIM3, TIM_IT_CC2) == SET)
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);

	else if(TIM_GetITStatus(TIM3, TIM_IT_CC3) == SET)
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);

	else if(TIM_GetITStatus(TIM3, TIM_IT_CC4) == SET)
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);

	else if(TIM_GetITStatus(TIM3, TIM_IT_COM) == SET)
		TIM_ClearITPendingBit(TIM3, TIM_IT_COM);

	else if(TIM_GetITStatus(TIM3, TIM_IT_Trigger) == SET)
		TIM_ClearITPendingBit(TIM3, TIM_IT_Trigger);

	else if(TIM_GetITStatus(TIM3, TIM_IT_Break) == SET)
		TIM_ClearITPendingBit(TIM3, TIM_IT_Break);*/


	/*Pwm_Desactiver(Broche_SI);

	TIM_ITConfig( ( TIM_TypeDef* )Mapping_GPIO[Broche_SI].Periph,
			__GetITChannel(Mapping_GPIO[Broche_SI].Parametre),
			DISABLE							);*/

	//TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);

	if(!Conv) {
		Pwm_Activer(BROCHE_CLK);
		Conv = TRUE;
	}

	Pwm_Desactiver(Broche_SI);

}

void
TIM2_IRQHandler(
	void
) {


	TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);

	if(Conv) {
		ADC_SoftwareStartConv( ADC1 );
		if(nbIT == 128){
			Pwm_Desactiver(BROCHE_CLK);
			TIM_ITConfig(TIM2,TIM_IT_CC4, DISABLE);
			nbIT = 0;
			Conv = FALSE;
			Ligne++;
		} else	nbIT++;
	}

	/*if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	else if(TIM_GetITStatus(TIM2, TIM_IT_CC1) == SET)
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	else if(TIM_GetITStatus(TIM2, TIM_IT_CC2) == SET)
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	else if(TIM_GetITStatus(TIM2, TIM_IT_CC3) == SET)
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	else if(TIM_GetITStatus(TIM2, TIM_IT_CC4) == SET)
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	if(nbIT == 128) {
		Pwm_Desactiver(BROCHE_CLK);
		TIM_ITConfig( ( TIM_TypeDef* )Mapping_GPIO[BROCHE_CLK].Periph,
				__GetITChannel(Mapping_GPIO[BROCHE_CLK].Parametre),
				DISABLE							);
	}*/

	//TIM_ITConfig(TIM2, TIM_IT_CC4, DISABLE);
}

uint8_t nbLigne;
void
ADC_IRQHandler(
	void
) {
	/*if(Ligne == 10) {
		Ligne = 0;

		if(nbLigne == 2)
			nbLigne = 0;
		else	nbLigne++;
	}*/

	if(ADC_GetITStatus(ADC1, ADC_IT_OVR)) {
		ADC_ClearITPendingBit(ADC1, ADC_IT_OVR);

	} else if(ADC_GetITStatus(ADC1, ADC_IT_EOC)) {
		ValCam[nbIT-1] = ADC_GetConversionValue(ADC1);
		ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
	}
}


typedef enum {
	Recherche_Sommet1,
	Recherche_Minimum,
	Recherche_Sommet2,
}Etapes_Analyse_e;


enum {
	IDSommet1,
	IDMinimum,
	IDSommet2,
};

uint8_t Indice[3];
uint8_t Analyse;
void
Camera_main() {

	Etapes_Analyse_e Etapes_Analyse = Recherche_Sommet1;
	uint8_t b_Indice = 0;
	uint32_t Valeur_moy = 0;

	if(Ligne) {

		Ligne = 0;
		for(b_Indice=0; b_Indice<128; b_Indice = b_Indice+3) {

			Valeur_moy = ValCam[b_Indice] + ValCam[b_Indice+1] + ValCam[b_Indice+2];
			Valeur_moy /= 3;

			switch(Etapes_Analyse) {

				//----------------------------------------------------
				case Recherche_Sommet1:
					if(Valeur_moy > 2200) {
						Indice[IDSommet1] = b_Indice;
						Etapes_Analyse = Recherche_Minimum;
					} break;

				//----------------------------------------------------
				case Recherche_Minimum:
					if(Valeur_moy < 2000) {
						Indice[IDMinimum] = b_Indice;
						Etapes_Analyse = Recherche_Sommet2;
					} break;

				//----------------------------------------------------
				case Recherche_Sommet2:
					if(Valeur_moy > 2200) {
						Indice[IDSommet2] = b_Indice;
						Analyse = 1;
					} break;

				//----------------------------------------------------
				default:
					break;
			}
		}
	}
}

#define DELTA_SOMET1_min	3
#define DELTA_SOMET1_max	20

#define DELTA_MIN_min		45
#define DELTA_MIN_max		75

#define DELTA_SOMET2_min	90
#define DELTA_SOMET2_max	120

typedef enum {

	Tout_Droit = 0,
	TourneDroite45,
	TourneDroite90,
	TourneGauche45,
	TourneGauche90,
	Mauvaise,

}Ordres_e;


Ordres_e OrdreLigne[10];
uint8_t IDLigne;

void
Camera_Analyse(){

	if(Analyse) {

		Analyse = 0;

		if(Indice[IDSommet1] > DELTA_SOMET1_min && Indice[IDSommet1] > DELTA_SOMET1_max ) {

			if( Indice[IDMinimum] > DELTA_MIN_min && Indice[IDMinimum] > DELTA_MIN_max  ) {

				if(Indice[IDSommet2] > DELTA_SOMET2_min && Indice[IDSommet2] > DELTA_SOMET2_max ) {

					OrdreLigne[IDLigne] = Tout_Droit;
				}

			}

		}



		else if(Indice[IDSommet1] > DELTA_SOMET1_min && Indice[IDSommet1] > DELTA_SOMET1_max ) {

			if( Indice[IDMinimum] > DELTA_MIN_min && Indice[IDMinimum] > DELTA_MIN_max  ) {

				if(Indice[IDSommet2] > DELTA_SOMET2_min && Indice[IDSommet2] > DELTA_SOMET2_max ) {

					OrdreLigne[IDLigne] = Tout_Droit;
				}

			}

		}


		else {

			OrdreLigne[IDLigne] = Mauvaise;
		}


		IDLigne++;

		if(IDLigne == 10) IDLigne = 0;
	}
}

/**-------------------------------------------------------------------
 * Main function
 *
 */
int
main(
	void
){
	uint8_t oldTSW = 0;

	//--------------------------------------------------------------
	//------------ Initialisations
	BSP_Init();
	SysTick_Config(CONFIG_SYSCLOCK_1ms);
	TSW_Start(&TSW_AffichageRefresh, 50);
	ADC_ChannelConfigure();

	PWM_Init(Broche_SI, 4000, 80);
	PWM_Init(BROCHE_CLK, 100, 50);
	__NOP;
	//Pwm_Activer(Broche_SI);
	//Pwm_Activer(BROCHE_CLK);

	while(1) {

		Console_main();

		if __TSW_isFinished(TSW_BurstRefresh)	 {
			Pwm_Activer(Broche_SI);
			TSW_Start(&TSW_BurstRefresh, 20);
							 }

		Camera_main();
		Camera_Analyse();

		#ifdef  DEBUG_ON
			if __TSW_isFinished(TSW_AffichageRefresh) {
				TSW_Start(&TSW_AffichageRefresh, 50);
				Affichage_Main();
			}	setDureeTourBoucle();
		#endif
	}
}
