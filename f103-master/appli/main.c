/**
  ******************************************************************************
  * @file    main.c
  * @author  Mathias Jomeau
  * @date    14-January-2024
  * @brief   Fichier Principal
  ******************************************************************************
*/

#include "stm32f1xx_hal.h"
#include "stm32f1_uart.h"
#include "stm32f1_sys.h"
#include "macro_types.h"
#include "systick.h"

#include "tft.h"
#include "button.h"
#include "electrovanne.h"
#include "hcsr04.h"
#include "mcp9701.h"

#include "test_unitaire.h"
#include "config.h"

#define ID_ELECTROVANNE_CUVE	0
#define ID_ELECTROVANNE_EAU		1
#define ID_BUTTON_H				0
#define ID_BUTTON_B				1
#define ID_BUTTON_E				2

static uint16_t PROFONDEUR_CUVE =  40; // en dm

static void process_ms(void);
static void state_machine(void);

static volatile uint32_t t = 0;
static volatile bool_e flag_5s;

/**
 * @brief Fonction de traitement appelée périodiquement toutes les millisecondes pour gérer une période de 5s
 * @pre Fonction ajouté à la routine d'interruption du périphique SYSTICK
 */

static void process_ms(void)
{
	static uint32_t t5s = 0;
	t5s = (t5s + 1)%5000;
	if(!t5s)
		flag_5s = TRUE;
	if(t)
		t--;
}

/**
 * @brief Fonction au coeur du programme
 */
int main(void)
{
	//Initialisation de la couche logicielle HAL (Hardware Abstraction Layer)
	//Cette ligne doit rester la premi�re �tape de la fonction main().
	HAL_Init();

	//Initialisation de l'UART1
	UART_init(UART1_ID,115200);

	//Indique que les printf sortent vers le p�riph�rique UART2
	SYS_set_std_usart(UART1_ID, UART1_ID, UART1_ID);

	//On ajoute la fonction process_ms à la liste des fonctions appelées automatiquement chaque ms par la routine d'interruption du périphérique SYSTICK
	Systick_add_callback_function(&process_ms);

	while(1)	//boucle de tâche de fond
	{
		// Taches de fond pour les boutons
		BUTTON_state_machine(ID_BUTTON_H);
		BUTTON_state_machine(ID_BUTTON_B);
		BUTTON_state_machine(ID_BUTTON_E);
		// Tache de fond pour le capteur de distance
		HCSR04_process_main();

		state_machine();
	}

}

/**
 * @brief Machine à états principale du programme.
 * @pre Fonction qui doit être appelée le plus souvent possible pour un bon fonctionnement
 */
static void state_machine(void)
{
	/**
	 * @enum state_e
	 * @brief Enumeration des différents états du programme principale
	 */
	typedef enum {
	    INIT, /**< Initialisation des différentes variables, capteurs et actionneurs. */
	    ACCUEIL, /**< État d'affichage de l'accueil sur l'écran TFT. */
	    MODE_AUTO, /**< État du mode automatique du système. */
	    MODE_MANUEL, /**< État du mode manuel du système. */
	    PARAMETRES, /**< État d'affichage des paramètres sur l'écran TFT. */
	    ANNONCE, /**< État d'affichage d'une annonce sur l'écran TFT. */
	    MODIF_CUVE, /**< État de modification de la taille de la cuve. */
	    MODIF_ALERTES /**< État de modification des alertes sur l'écran TFT. */
	} state_e;

	static state_e state = INIT;
	static state_e previous_state = INIT;
	bool_e entrance = (state!=previous_state)?TRUE:FALSE;

	// Récupération des événements des boutons
	button_event_e button_H = BUTTON_getEvent(ID_BUTTON_H);
	button_event_e button_B = BUTTON_getEvent(ID_BUTTON_B);
	button_event_e button_E = BUTTON_getEvent(ID_BUTTON_E);

	// Récupération des états des electrovannes
	bool_e state_electrovanne_cuve = ELECTROVANNE_GetState(ID_ELECTROVANNE_CUVE);
	bool_e state_electrovanne_eau = ELECTROVANNE_GetState(ID_ELECTROVANNE_EAU);

	static uint8_t id_sensor;
	static uint16_t profondeur_pourcentage;
	uint16_t distance;
	static float eau_temperature;

	// Changement de la profondeur si seulement la distance est vérifiée
	// Un facteur x100 est appliquée pour convertir en mm pour être compatible avec la valeur du capteur de distance
	if (HCSR04_GetDistance(id_sensor, &distance))
		profondeur_pourcentage = (uint16_t) (((PROFONDEUR_CUVE*100) - distance) * 100 / (PROFONDEUR_CUVE*100));

	// Acutalisation des valeurs de capteurs et des états des electrovannes toutes les 5s
	if (flag_5s)
	{
		MCP9701_GetTemperature(&eau_temperature);
		TFT_InformationsSensors_Update(profondeur_pourcentage, state_electrovanne_cuve, state_electrovanne_eau, eau_temperature);
		flag_5s = FALSE;
	}

	static bool_e entrance_mode_auto;
	static uint8_t mode_chosing;
	static uint8_t current_mode;
	static bool_e activation_alertes;

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

			// MCP9701
			MCP9701_Init();

			current_mode = 1;
			entrance_mode_auto = FALSE;
			activation_alertes = TRUE;
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
				if (mode_chosing == 0)
				{
					entrance_mode_auto = TRUE;
				}

				state = mode[mode_chosing];
			}

			if (current_mode == MODE_AUTO)
			{
				state = MODE_AUTO;
			}

			break;

		case MODE_AUTO:
			if (entrance_mode_auto)
			{
				if (activation_alertes)
					TFT_Annonce("   Activation", " Mode Automatique");
				entrance_mode_auto = FALSE;
				state = ANNONCE;
			}

			if (profondeur_pourcentage < 50 && profondeur_pourcentage > 10)
			{
				if (activation_alertes)
					TFT_Annonce("    Attention", "     Cuve 50%");
			}
			else if (profondeur_pourcentage < 10 && profondeur_pourcentage > 0)
			{
				if (activation_alertes)
					TFT_Annonce("   Danger Pompe", "     Cuve 10%");

				// Changement des electrovannes par sécurité
				ELECTROVANNE_Set(ID_ELECTROVANNE_CUVE, FALSE);
				ELECTROVANNE_Set(ID_ELECTROVANNE_EAU, TRUE);
			}
			else if (eau_temperature < 5 && eau_temperature > 0)
			{
				if (activation_alertes)
					TFT_Annonce("    Attention", "Temperature < 5 C");
			}
			else if (eau_temperature < 0)
			{
				if (activation_alertes)
					TFT_Annonce("    Attention", "Temperature < 0 C");

				// Changement des electrovannes par sécurité
				ELECTROVANNE_Set(ID_ELECTROVANNE_CUVE, FALSE);
				ELECTROVANNE_Set(ID_ELECTROVANNE_EAU, TRUE);
			}
			else //Si toutes conditions sont optimales, on modifie l'états des electrovannes
			{
				ELECTROVANNE_Set(ID_ELECTROVANNE_CUVE, TRUE);
				ELECTROVANNE_Set(ID_ELECTROVANNE_EAU, FALSE);
			}

			if (activation_alertes)
				state = ANNONCE;
			else
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
			if (entrance)
			{
				mode_chosing = 0;
				TFT_Mode_Parametre();
				TFT_Mode_Parametre_Update(mode_chosing);
				previous_state = PARAMETRES;
			}

			if (button_H == BUTTON_EVENT_SHORT_PRESS)
			{
				mode_chosing = (uint8_t) ((mode_chosing+2) % 3);
				TFT_Mode_Parametre_Update(mode_chosing);
			}

			else if (button_B == BUTTON_EVENT_SHORT_PRESS)
			{
				mode_chosing = (uint8_t) ((mode_chosing+1) % 3);
				TFT_Mode_Parametre_Update(mode_chosing);
			}

			else if (button_E == BUTTON_EVENT_SHORT_PRESS)
			{
				switch (mode_chosing)
				{
					case 0: // Modif Taille Cuve
						state = MODIF_CUVE;
						break;
					case 1: // Modif Activation Alertes
						state = MODIF_ALERTES;
						break;
					case 2: // Retour Acceuil
						state = ACCUEIL;
						break;
				}
			}
			break;

		case ANNONCE:
			if(entrance)
				previous_state = ANNONCE;
			if (button_E == BUTTON_EVENT_SHORT_PRESS) // On attend que l'utilisateur valide l'alerte
				state = ACCUEIL;
			break;

		case MODIF_CUVE:
			if(entrance)
			{
				previous_state = MODIF_CUVE;
				TFT_Parametre_Cuve();
				TFT_Parametre_Cuve_Update(PROFONDEUR_CUVE);
			}

			if (button_H == BUTTON_EVENT_SHORT_PRESS)
			{
				PROFONDEUR_CUVE ++;
				TFT_Parametre_Cuve_Update(PROFONDEUR_CUVE);
			}

			else if (button_B == BUTTON_EVENT_SHORT_PRESS)
			{
				PROFONDEUR_CUVE --;
				TFT_Parametre_Cuve_Update(PROFONDEUR_CUVE);
			}

			else if (button_E == BUTTON_EVENT_SHORT_PRESS)
			{
				state = PARAMETRES;
			}

			break;

		case MODIF_ALERTES:
			if(entrance)
			{
				previous_state = MODIF_ALERTES;
				TFT_Parametre_Alertes();
				TFT_Parametre_Alertes_Update(activation_alertes);
			}

			if (button_H == BUTTON_EVENT_SHORT_PRESS)
			{
				activation_alertes = !activation_alertes;
				TFT_Parametre_Alertes_Update(activation_alertes);
			}

			else if (button_B == BUTTON_EVENT_SHORT_PRESS)
			{
				activation_alertes = !activation_alertes;
				TFT_Parametre_Alertes_Update(activation_alertes);
			}

			else if (button_E == BUTTON_EVENT_SHORT_PRESS)
			{
				state = PARAMETRES;
			}

			break;

		default:
			state = ACCUEIL;
			break;
	}

}


