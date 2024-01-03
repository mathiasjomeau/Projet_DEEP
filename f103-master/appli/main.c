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

// Cuve Sphérique
static uint16_t PROFONDEUR_CUVE =  4000; // en mm

static void state_machine(void);

static volatile uint32_t t = 0;

void process_ms(void)
{
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
		//HCSR04_process_main();
		BUTTON_process_main();
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
		MODE_OFF
	}state_e;

	static state_e state = INIT;
	static state_e previous_state = INIT;
	bool_e entrance = (state!=previous_state)?TRUE:FALSE;

	button_event_e button_H_event;
	button_event_e button_B_event;
	button_event_e button_E_event;
	button_H_event = BUTTON_getEvent(0);
	button_B_event = BUTTON_getEvent(1);
	button_E_event = BUTTON_getEvent(2);

	static uint16_t profondeur_EP;
	uint16_t distance;
	if (HCSR04_GetDistance(0, &distance))
	{
		profondeur_EP = (uint16_t) (PROFONDEUR_CUVE - distance);
	}

	switch(state)
	{
		case INIT :
			// Ecran TFT
			TFT_Init();

			//Définitions
			static hcsr_04_t hcsr04_EP = {"HCSR04 Cuve", 1, GPIOB, GPIO_PIN_6, GPIOB, GPIO_PIN_7, 0};
			static electrovanne_t electrovanne_EC = {"Electrovanne EC", ELECTROVANNE0_GPIO, ELECTROVANNE0_PIN, 0};
			static electrovanne_t electrovanne_EP = {"Electrovanne EP", ELECTROVANNE1_GPIO, ELECTROVANNE1_PIN, 1};

			static state_e mode[3] = {MODE_AUTO, MODE_MANUEL, MODE_OFF};
			static uint8_t mode_chosing = 0;
			static uint8_t current_mode = 2;
			//static float temperature = 0.0;

			// HCSRO4
			//HCSR04_Init(&hcsr04_EP);

			// Electrovannes
			ELECTROVANNE_Init(&electrovanne_EC);
			ELECTROVANNE_Init(&electrovanne_EP);

			// Boutons
			BUTTON_add(0, BUTTON_U_GPIO, BUTTON_U_PIN);
			//BUTTON_add(1, BUTTON_D_GPIO, BUTTON_D_PIN);
			//BUTTON_add(2, BUTTON_R_GPIO, BUTTON_R_PIN);

			previous_state = state;
			state = MODE_OFF;

			break;

		case ACCUEIL:
			/*
			if (entrance)
			{
				TFT_Acceuil();
				TFT_Update_capteurs(hcsr04_EP.value, electrovanne_EC.state, electrovanne_EP.state);
				TFT_Change_CurrentMode(current_mode);
				TFT_Select_Mode(mode_chosing);
				previous_state = ACCUEIL;
			}
			*/

			/*if (button_H_event == BUTTON_EVENT_SHORT_PRESS)
			{
				mode_chosing = (uint8_t) ((mode_chosing+2) % 3);
				TFT_Select_Mode(mode_chosing);
				printf("button_H \n");
			}
			if (button_B_event == BUTTON_EVENT_SHORT_PRESS)
			{
				mode_chosing = (uint8_t) ((mode_chosing+1) % 3);
				TFT_Select_Mode(mode_chosing);
				printf("button_B \n");
			}

			if (button_E_event == BUTTON_EVENT_SHORT_PRESS)
			{
				current_mode = mode_chosing;
				TFT_Change_CurrentMode(current_mode);
				printf("button_E \n");
				//state = mode[mode_chosing];
			}
			*/

			break;

		case MODE_AUTO:
			if(entrance)
			{
				//TFT_Mode_Auto();
				previous_state = MODE_AUTO;
				//TFT_Update_capteurs(hcsr04_EP.value, electrovanne_EC.state, electrovanne_EP.state);
			}
			// Sécu de 20cm, on coupe la vanne de l'eau de pluie
			if (profondeur_EP < 200)
			{
				electrovanne_EP.state = 1;
				electrovanne_EC.state = 0;
				ELECTROVANNE_Set(&electrovanne_EP);
				ELECTROVANNE_Set(&electrovanne_EC);
			}
			// condition à rajouter pour la température
			else
			{
				electrovanne_EP.state = 0;
				electrovanne_EC.state = 1;
				ELECTROVANNE_Set(&electrovanne_EP);
				ELECTROVANNE_Set(&electrovanne_EC);
			}


			break;

		case MODE_MANUEL:
			break;
		case MODE_OFF:
			if (entrance)
			{
				TFT_Acceuil();
				TFT_Acceuil_Update(1, 0, 0, 0);
				previous_state = MODE_OFF;
			}
			break;
	}

}


