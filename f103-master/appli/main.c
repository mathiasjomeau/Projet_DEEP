/**
  ******************************************************************************
  * @file    main.c
  * @author  Nirgal
  * @date    03-July-2019
  * @brief   Default main function.
  ******************************************************************************
*/
#include "stm32f1xx_hal.h"
#include "stm32f1_uart.h"
#include "stm32f1_sys.h"
#include "macro_types.h"
#include "systick.h"

#include "test_unitaire.h"

#include "stm32f1_gpio.h"
#include "stm32f1_ili9341.h" // Import de la librairie de l'ecran TFT
#include "stm32f1_adc.h"
#include "HC-SR04/HCSR04.h"
#include "tft.h"
#include "button.h"
#include "electrovanne.h"
#include "hcsr04.h"

#include "config.h"

#define ID_ELECTROVANNE_CUVE	0
#define ID_ELECTROVANNE_EAU		1
#define ID_BUTTON_H				0
#define ID_BUTTON_B				1
#define ID_BUTTON_E				2

// Cuve Sphérique
static uint16_t PROFONDEUR_CUVE =  4000; // en mm

static void process_ms(void);
static void state_machine(void);

static volatile uint32_t t = 0;
static volatile bool_e flag_5s;

static void process_ms(void)
{
	static uint32_t t5s = 0;
	t5s = (t5s + 1)%5000;
	if(!t5s)
		flag_5s = TRUE;
	if(t)
		t--;
}

int main(void)
{
	//Initialisation de la couche logicielle HAL (Hardware Abstraction Layer)
	//Cette ligne doit rester la premi�re �tape de la fonction main().
	HAL_Init();

	//Initialisation de l'UART2 � la vitesse de 115200 bauds/secondes (92kbits/s) PA2 : Tx  | PA3 : Rx.
		//Attention, les pins PA2 et PA3 ne sont pas reli�es jusqu'au connecteur de la Nucleo.
		//Ces broches sont redirig�es vers la sonde de d�bogage, la liaison UART �tant ensuite encapsul�e sur l'USB vers le PC de d�veloppement.
	//UART_init(UART2_ID,115200);
	UART_init(UART1_ID,115200);

	//"Indique que les printf sortent vers le p�riph�rique UART2."
	//SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);
	SYS_set_std_usart(UART1_ID, UART1_ID, UART1_ID);


	//On ajoute la fonction process_ms � la liste des fonctions appel�es automatiquement chaque ms par la routine d'interruption du p�riph�rique SYSTICK
	Systick_add_callback_function(&process_ms);

	while(1)	//boucle de t�che de fond
	{
		BUTTON_state_machine(ID_BUTTON_H);
		BUTTON_state_machine(ID_BUTTON_B);
		BUTTON_state_machine(ID_BUTTON_E);
		HCSR04_process_main();
		state_machine();
	}
}

static void state_machine(void)
{
	typedef enum{
		INIT,
		ACCUEIL,
		MODE_AUTO,
		MODE_MANUEL,
		PARAMETRES,
	}state_e;

	static state_e state = INIT;
	static state_e previous_state = INIT;
	bool_e entrance = (state!=previous_state)?TRUE:FALSE;

	button_event_e button_H = BUTTON_getEvent(ID_BUTTON_H);
	button_event_e button_B = BUTTON_getEvent(ID_BUTTON_B);
	button_event_e button_E = BUTTON_getEvent(ID_BUTTON_E);

	bool_e state_electrovanne_cuve = ELECTROVANNE_GetState(ID_ELECTROVANNE_CUVE);
	bool_e state_electrovanne_eau = ELECTROVANNE_GetState(ID_ELECTROVANNE_EAU);

	static uint8_t id_sensor;
	static uint16_t profondeur_pourcentage;
	uint16_t distance;
	if (HCSR04_GetDistance(id_sensor, &distance))
		profondeur_pourcentage = (uint16_t) ((PROFONDEUR_CUVE - distance) * 100 / PROFONDEUR_CUVE);

	if (flag_5s)
	{
		TFT_InformationsSensors_Update(profondeur_pourcentage, state_electrovanne_cuve, state_electrovanne_eau);
		flag_5s = FALSE;
	}

	static uint8_t mode_chosing;
	static uint8_t current_mode;

	switch(state)
	{
		case INIT :
			// Ecran TFT
			TFT_Init();

			// HCSRO4
			HCSR04_Init(&id_sensor, GPIOB, GPIO_PIN_6, GPIOB, GPIO_PIN_7);

			// Electrovannes
			ELECTROVANNE_Add(ID_ELECTROVANNE_CUVE, ELECTROVANNE0_GPIO, ELECTROVANNE0_PIN);
			ELECTROVANNE_Add(ID_ELECTROVANNE_EAU, ELECTROVANNE1_GPIO, ELECTROVANNE1_PIN);
			ELECTROVANNE_Set(ID_ELECTROVANNE_CUVE, FALSE);
			ELECTROVANNE_Set(ID_ELECTROVANNE_EAU, TRUE);

			// Boutons
			BUTTON_add(0, BUTTON_U_GPIO, BUTTON_U_PIN);
			BUTTON_add(1, BUTTON_D_GPIO, BUTTON_D_PIN);
			BUTTON_add(2, BUTTON_R_GPIO, BUTTON_R_PIN);

			current_mode = 1;

			previous_state = state;
			state = ACCUEIL;

			break;

		case ACCUEIL:

			if (entrance)
			{
				mode_chosing = 0;
				TFT_Acceuil();
				TFT_Acceuil_Update(1, current_mode, mode_chosing);
				TFT_Acceuil_Update(2, current_mode, mode_chosing);
				previous_state = ACCUEIL;
			}

			static state_e mode[3] = {MODE_AUTO, MODE_MANUEL, PARAMETRES};

			if (button_H == BUTTON_EVENT_SHORT_PRESS)
			{
				mode_chosing = (uint8_t) ((mode_chosing+2) % 3);
				TFT_Acceuil_Update(2, current_mode, mode_chosing);
			}

			else if (button_B == BUTTON_EVENT_SHORT_PRESS)
			{
				mode_chosing = (uint8_t) ((mode_chosing+1) % 3);
				TFT_Acceuil_Update(2, current_mode, mode_chosing);
			}

			else if (button_E == BUTTON_EVENT_SHORT_PRESS)
			{
				if (mode_chosing == 0 || mode_chosing == 1)
				{
					current_mode = mode_chosing;
					TFT_Acceuil_Update(1, current_mode, mode_chosing);
				}
				state = mode[mode_chosing];
			}

			break;

		case MODE_AUTO:

			printf("coucou mode auto");

			state = ACCUEIL;

			break ;

		case MODE_MANUEL:

			if (entrance)
			{
				mode_chosing = 0;
				TFT_Mode_Manuel();
				TFT_Mode_Manual_Update(mode_chosing);
				previous_state = MODE_MANUEL;
			}

			if (button_H == BUTTON_EVENT_SHORT_PRESS)
			{
				mode_chosing = (uint8_t) ((mode_chosing+2) % 3);
				TFT_Mode_Manual_Update(mode_chosing);
			}

			else if (button_B == BUTTON_EVENT_SHORT_PRESS)
			{
				mode_chosing = (uint8_t) ((mode_chosing+1) % 3);
				TFT_Mode_Manual_Update(mode_chosing);
			}

			else if (button_E == BUTTON_EVENT_SHORT_PRESS)
			{
				switch (mode_chosing)
				{
					case 0: // Modif Electovanne Cuve
						ELECTROVANNE_Set(ID_ELECTROVANNE_CUVE, !state_electrovanne_cuve);
						break;
					case 1: // Modif Electovanne Eau
						ELECTROVANNE_Set(ID_ELECTROVANNE_EAU, !state_electrovanne_eau);
						break;
					case 2: // Retour Acceuil
						state = ACCUEIL;
						break;

				}
			}

			break;

		case PARAMETRES:

			printf("coucou mode parametre");
			// modifier la taille de la cuve
			// modifier les paramètres du mode auto
			// activer ou desac des alertes

			state = ACCUEIL;
			break;
	}

}


