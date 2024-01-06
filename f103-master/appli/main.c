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

	/*static uint8_t id_sensor;
	uint16_t distance;

	HCSR04_Init(&id_sensor, GPIOB, GPIO_PIN_6, GPIOB, GPIO_PIN_7);*/
	while(1)	//boucle de t�che de fond
	{
		BUTTON_state_machine(0);
		BUTTON_state_machine(1);
		BUTTON_state_machine(2);
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

	static uint8_t id_sensor;
	static uint16_t profondeur_pourcentage;
	static uint16_t previous_profondeur_pourcentage = 0;
	uint16_t distance;

	if (HCSR04_GetDistance(id_sensor, &distance))
	{
		profondeur_pourcentage = (uint16_t) ((PROFONDEUR_CUVE - distance) * 100 / PROFONDEUR_CUVE);
	}

	switch(state)
	{
		case INIT :
			// Ecran TFT
			TFT_Init();

			//Définitions
			//static hcsr_04_t hcsr04_EP = {"HCSR04 Cuve", 1, GPIOB, GPIO_PIN_6, GPIOB, GPIO_PIN_7, 0};
			static electrovanne_t electrovanne_EC = {"Electrovanne EC", ELECTROVANNE0_GPIO, ELECTROVANNE0_PIN, 0};
			static electrovanne_t electrovanne_EP = {"Electrovanne EP", ELECTROVANNE1_GPIO, ELECTROVANNE1_PIN, 1};

			static state_e mode[3] = {MODE_AUTO, MODE_MANUEL, PARAMETRES};
			static uint8_t mode_chosing = 0;
			static uint8_t current_mode = 1;
			//static float temperature = 0.0;

			// HCSRO4
			HCSR04_Init(&id_sensor, GPIOB, GPIO_PIN_6, GPIOB, GPIO_PIN_7);

			// Electrovannes
			ELECTROVANNE_Init(&electrovanne_EC);
			ELECTROVANNE_Init(&electrovanne_EP);

			// Boutons
			BUTTON_add(0, BUTTON_U_GPIO, BUTTON_U_PIN);
			BUTTON_add(1, BUTTON_D_GPIO, BUTTON_D_PIN);
			BUTTON_add(2, BUTTON_R_GPIO, BUTTON_R_PIN);

			previous_state = state;
			state = ACCUEIL;

			break;

		case ACCUEIL:

			if (entrance)
			{
				TFT_Acceuil();
				TFT_Acceuil_Update(1, current_mode, mode_chosing, profondeur_pourcentage, electrovanne_EC.state, electrovanne_EP.state);
				TFT_Acceuil_Update(2, current_mode, mode_chosing, profondeur_pourcentage, electrovanne_EC.state, electrovanne_EP.state);
				TFT_Acceuil_Update(3, current_mode, mode_chosing, profondeur_pourcentage, electrovanne_EC.state, electrovanne_EP.state);
				previous_state = ACCUEIL;
			}

			if (BUTTON_getEvent(0) == BUTTON_EVENT_SHORT_PRESS)
			{
				mode_chosing = (uint8_t) ((mode_chosing+2) % 3);
				TFT_Acceuil_Update(2, current_mode, mode_chosing, profondeur_pourcentage, electrovanne_EC.state, electrovanne_EP.state);
			}

			else if (BUTTON_getEvent(1) == BUTTON_EVENT_SHORT_PRESS)
			{
				mode_chosing = (uint8_t) ((mode_chosing+1) % 3);
				TFT_Acceuil_Update(2, current_mode, mode_chosing, profondeur_pourcentage, electrovanne_EC.state, electrovanne_EP.state);
			}

			else if (BUTTON_getEvent(2) == BUTTON_EVENT_SHORT_PRESS)
			{
				if (mode_chosing == 0 || mode_chosing == 1)
				{
					current_mode = mode_chosing;
					TFT_Acceuil_Update(1, current_mode, mode_chosing, profondeur_pourcentage, electrovanne_EC.state, electrovanne_EP.state);
				}
				state = mode[mode_chosing];
			}

			if (profondeur_pourcentage != previous_profondeur_pourcentage)
			{
				TFT_Acceuil_Update(3, current_mode, mode_chosing, profondeur_pourcentage, electrovanne_EC.state, electrovanne_EP.state);
				previous_profondeur_pourcentage = profondeur_pourcentage;
			}


			break;

		case MODE_AUTO:

			printf("coucou mode auto");

			state = ACCUEIL;
			/*if(entrance)
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
			*/
			break ;

		case MODE_MANUEL:

			printf("coucou mode manuel");

			state = ACCUEIL;
			break;
		case PARAMETRES:

			printf("coucou mode parametre");

			state = ACCUEIL;
			break;
	}

}


