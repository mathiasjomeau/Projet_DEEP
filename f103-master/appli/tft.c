/**
  ******************************************************************************
  * @file    tft.c
  * @author  Mathias Jomeau
  * @date    14-January-2024
  * @brief   Fonctions pour la gestion de l'écran TFT
  ******************************************************************************
*/

#include "tft.h"
#include "stm32f1_ili9341.h"
#include <stdio.h>
#include <stdlib.h>

#define NB_MAX_PARAMETERS 30

/**
 * @struct DynamicLine_t
 * @brief Structure pour définir une ligne dynamique
 * @pre Utiliser uniquement pour des lignes où ses paramètres peuvent changer
 */
typedef struct {
    uint16_t position_x; /**< Position X de la ligne sur l'écran. */
    uint16_t position_y; /**< Position Y de la ligne sur l'écran. */
    char text[40]; /**< Texte affiché sur la ligne. */
    FontDef_t *font; /**< Police de caractères utilisée pour le texte. */
    uint16_t foreground; /**< Couleur du texte. */
    uint16_t background; /**< Couleur de fond de la ligne. */
    uint8_t id; /**< Identifiant de la ligne. */
} DynamicLine_t;

void updateDynamicLine_Text(DynamicLine_t pDynamicLine, char * new_text);
void updateDynamicLine_Foreground(DynamicLine_t pDynamicLine, uint16_t newForeground);
void updateDynamicLine_Background(DynamicLine_t pDynamicLine, uint16_t newBackground);
void displayDynamicLine(DynamicLine_t pDynamicLine);
void displayTitle(void);
void displayInformationsSensors(void);
void updateSelectedMenu(uint8_t ids[], uint8_t id_modeselected);

DynamicLine_t Screens_Addresse [NB_MAX_PARAMETERS];

/**
 * @brief Affiche une ligne dynamique.
 * @param pDynamicLine Ligne dynamique à afficher.
 */
void displayDynamicLine(DynamicLine_t pDynamicLine)
{
	Screens_Addresse[pDynamicLine.id] = pDynamicLine;

    ILI9341_Puts(pDynamicLine.position_x, pDynamicLine.position_y, pDynamicLine.text,
                     pDynamicLine.font, pDynamicLine.foreground, pDynamicLine.background);
}

/**
 * @brief Met à jour le texte d'une ligne dynamique.
 * @param pDynamicLine Ligne dynamique à mettre à jour.
 * @param new_text Nouveau texte.
 */
void updateDynamicLine_Text(DynamicLine_t pDynamicLine, char * new_text)
{
	sprintf(pDynamicLine.text, "                                   ");
	snprintf(pDynamicLine.text, sizeof(pDynamicLine.text), "%s", new_text);
	Screens_Addresse[pDynamicLine.id] = pDynamicLine;
	displayDynamicLine(pDynamicLine);
}

/**
 * @brief Met à jour la couleur du texte d'une ligne dynamique.
 * @param pDynamicLine Ligne dynamique à mettre à jour.
 * @param newForeground Nouvelle couleur du texte.
 */
void updateDynamicLine_Foreground(DynamicLine_t pDynamicLine, uint16_t newForeground)
{
	pDynamicLine.foreground = newForeground;
	Screens_Addresse[pDynamicLine.id] = pDynamicLine;
	displayDynamicLine(pDynamicLine);
}

/**
 * @brief Met à jour la couleur de fond d'une ligne dynamique.
 * @param pDynamicLine Ligne dynamique à mettre à jour.
 * @param newBackground Nouvelle couleur de fond.
 */
void updateDynamicLine_Background(DynamicLine_t pDynamicLine, uint16_t newBackground)
{
	pDynamicLine.background = newBackground;
	Screens_Addresse[pDynamicLine.id] = pDynamicLine;
	displayDynamicLine(pDynamicLine);
}

/**
 * @brief Affiche le titre sur l'écran TFT.
 */
void displayTitle()
{
	ILI9341_PutBigs(100, 20, "HYDRESEO", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 2, 2);
	ILI9341_DrawLine(0, 50, 400, 50, ILI9341_COLOR_BLACK);
}


/**
 * @brief Affiche les informations des capteurs sur l'écran TFT.
 */
void displayInformationsSensors()
{
	DynamicLine_t niveau_cuve = {30,170, "Niveau de la cuve :", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 4};
	DynamicLine_t electrovanne_EP = {30,185, "Electrovanne Cuve :", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 5};
	DynamicLine_t electrovanne_EC = {30,200, "Electrovanne Eau :", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 6};
	DynamicLine_t temp_eau = {30,215, "Temperature Eau:", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 7};

	displayDynamicLine(niveau_cuve);
	displayDynamicLine(electrovanne_EP);
	displayDynamicLine(electrovanne_EC);
	displayDynamicLine(temp_eau);
}

/**
 * @brief Met à jour les informations des capteurs affichées sur l'écran TFT.
 * @param water_level Niveau d'eau en pourcentage.
 * @param EC_state État de l'électrovanne de la cuve.
 * @param EP_state État de l'électrovanne d'eau.
 * @param eau_temperature Température de l'eau.
 */
void TFT_InformationsSensors_Update(uint16_t water_level, bool_e EC_state, bool_e EP_state, float eau_temperature)
{
	char water_level_char [30];
	char EP_state_char [40];
	char EC_state_char [40];
	char eau_temperature_char [30];

	sprintf(water_level_char, "Niveau de la cuve : %d%%", water_level);
	updateDynamicLine_Text(Screens_Addresse[4], water_level_char);

	char * state [2] = {"Fermee", "Ouverte"}; // Etats des Electrovannes possible

	sprintf(EC_state_char, "Electrovanne Cuve : %s ", state[EC_state]);
	updateDynamicLine_Text(Screens_Addresse[5], EC_state_char);

	sprintf(EP_state_char, "Electrovanne Eau Courante : %s ", state[EP_state]);
	updateDynamicLine_Text(Screens_Addresse[6], EP_state_char);

	sprintf(eau_temperature_char, "Temperature Eau : %.1f C", eau_temperature);
	updateDynamicLine_Text(Screens_Addresse[7], eau_temperature_char);
}

/**
 * @brief Met à jour le menu sélectionné sur l'écran TFT.
 * @param ids Tableau d'identifiants des lignes dynamiques.
 * @param id_modeselected Identifiant du mode sélectionné.
 */
void updateSelectedMenu(uint8_t ids[], uint8_t id_modeselected)
{
	for (uint8_t i = 1 ; i < ids[0] ; i++)
	{
		updateDynamicLine_Foreground(Screens_Addresse[ids[i]], ILI9341_COLOR_BLACK);
		updateDynamicLine_Background(Screens_Addresse[ids[i]], ILI9341_COLOR_WHITE);
	}
	updateDynamicLine_Foreground(Screens_Addresse[id_modeselected], ILI9341_COLOR_WHITE);
	updateDynamicLine_Background(Screens_Addresse[id_modeselected], ILI9341_COLOR_GRAY);
}

/**
 * @brief Initialise l'écran TFT.
 * Elle doit être appelée avant toute utilisation de l'écran.
 */
void TFT_Init(void)
{
	ILI9341_Init();
	ILI9341_Rotate(ILI9341_Orientation_Landscape_2);
	ILI9341_Fill(ILI9341_COLOR_WHITE);
}

/**
 * @brief Affiche l'écran d'accueil sur l'écran TFT.
 */
void TFT_Acceuil()
{
	DynamicLine_t mode_actif = {30, 60, "Mode actif :", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 0};
	DynamicLine_t mode_auto = {30,90, " - Mode Auto", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 1};
	DynamicLine_t mode_manuel = {30,110, " - Mode Manuel", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 2};
	DynamicLine_t mode_off = {30,130, " - Parametres", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 3};

	ILI9341_DrawFilledRectangle(0, 55, 350, 155, ILI9341_COLOR_WHITE);

	displayTitle();

	displayDynamicLine(mode_actif);
	displayDynamicLine(mode_auto);
	displayDynamicLine(mode_manuel);
	displayDynamicLine(mode_off);

	ILI9341_DrawLine(0, 160, 400, 160, ILI9341_COLOR_BLACK);

	displayInformationsSensors();
}

/**
 * @brief Met à jour l'écran d'accueil
 * @param mode Mode à afficher.
 * @param current_mode Mode actuel.
 * @param id_mode Identifiant du mode à surligner dans la sélection des menus
 */
void TFT_Acceuil_Update(uint8_t mode, uint8_t current_mode, uint8_t id_mode)
{
	uint8_t ids_mode [4] = {4, 1, 2, 3};
	switch (mode)
	{
		case 1: // Update Current Mode
			switch (current_mode)
			{
				case 0:
					updateDynamicLine_Text(Screens_Addresse[0], "Mode actif : Mode Auto  " );
					break;
				case 1:
					updateDynamicLine_Text(Screens_Addresse[0], "Mode actif : Mode Manuel" );
					break;
			}
			break;

		case 2: // Update Selected mode
			id_mode = (uint8_t) (id_mode +1);
			updateSelectedMenu(ids_mode, id_mode);

			break;
	}
}

/**
 * @brief Affiche l'écran du mode manuel sur l'écran TFT.
 */
void TFT_Mode_Manuel()
{
	ILI9341_DrawFilledRectangle(0, 55, 350, 155, ILI9341_COLOR_WHITE);

	DynamicLine_t electrovanne_cuve = {30, 90, " - Electrovanne Cuve", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 10};
	DynamicLine_t electrovanne_eau_courante = {30,110, " - Electrovanne Eau", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 11};
	DynamicLine_t retour = {30,130, " - Retour", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 12};

	ILI9341_Puts(100, 60, "Mode Manuel", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

	displayDynamicLine(electrovanne_cuve);
	displayDynamicLine(electrovanne_eau_courante);
	displayDynamicLine(retour);
}

/**
 * @brief Met à jour l'écran du mode manuel
 * @param id_mode Identifiant du mode à surligner dans la sélection des menus
 */
void TFT_Mode_Manual_Update(uint8_t id_mode)
{
	uint8_t ids_mode [4] = {4, 10, 11, 12};
	id_mode = (uint8_t) (id_mode + 10);
	updateSelectedMenu(ids_mode, id_mode);
}

/**
 * @brief Affiche l'écran des paramètres sur l'écran TFT.
 */
void TFT_Mode_Parametre()
{
	ILI9341_DrawFilledRectangle(0, 55, 350, 155, ILI9341_COLOR_WHITE);

	DynamicLine_t modif_taille_cuve = {30, 90, " - Taille Cuve", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 13};
	DynamicLine_t activation_annonces = {30,110, " - Activation Alertes", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 14};
	DynamicLine_t retour = {30,130, " - Retour", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 15};

	ILI9341_Puts(100, 60, "Pamametres", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

	displayDynamicLine(modif_taille_cuve);
	displayDynamicLine(activation_annonces);
	displayDynamicLine(retour);
}

/**
 * @brief Met à jour l'écran du mode paramètres
 * @param id_mode Identifiant du mode à surligner dans la sélection des menus
 */
void TFT_Mode_Parametre_Update(uint8_t id_mode)
{
	uint8_t ids_mode [4] = {4, 13, 14, 15};
	id_mode = (uint8_t) (id_mode + 13);
	updateSelectedMenu(ids_mode, id_mode);
}

/**
 * @brief Affiche l'écran de modification de la cuve sur l'écran TFT.
 */
void TFT_Parametre_Cuve()
{
	DynamicLine_t taille_cuve = {130, 110, "", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 16};

	ILI9341_DrawFilledRectangle(20, 60, 300, 150, ILI9341_COLOR_WHITE);
	ILI9341_DrawRectangle(20, 60, 300, 150, ILI9341_COLOR_BLACK);

	ILI9341_Puts(60, 80, "Taille de la cuve : ", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

	displayDynamicLine(taille_cuve);

	ILI9341_Puts(260, 130, "OK", &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_GRAY);
}

/**
 * @brief Met à jour l'écran de modification de la cuve avec la taille spécifiée.
 * @param taille_cuve Taille de la cuve en dm
 */
void TFT_Parametre_Cuve_Update(uint16_t taille_cuve)
{
	char taille_cuve_char [30];
	sprintf(taille_cuve_char, "%d dm", taille_cuve);
	updateDynamicLine_Text(Screens_Addresse[16], taille_cuve_char);
}

/**
 * @brief Affiche l'écran de modification des alertes sur l'écran TFT.
 */
void TFT_Parametre_Alertes()
{
	DynamicLine_t activation_true = {135, 115, "", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 17};

	ILI9341_DrawFilledRectangle(20, 60, 300, 150, ILI9341_COLOR_WHITE);
	ILI9341_DrawRectangle(20, 60, 300, 150, ILI9341_COLOR_BLACK);

	ILI9341_Puts(60, 80, "Activation Alertes : ", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

	displayDynamicLine(activation_true);

	ILI9341_Puts(260, 130, "OK", &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_GRAY);

}

/**
 * @brief Met à jour l'écran de modification des alertes avec l'état d'activation spécifié.
 * @param activation État d'activation des alertes.
 */
void TFT_Parametre_Alertes_Update(bool_e activation)
{
	switch (activation)
	{
	case TRUE:
		updateDynamicLine_Text(Screens_Addresse[17], "Oui");
		break;
	case FALSE:
		updateDynamicLine_Text(Screens_Addresse[17], "Non");
		break;
	}
}

/**
 * @brief Affiche un écran d'annonce sur l'écran TFT.
 * @param ligne1 Texte de la première ligne d'annonce.
 * @param ligne2 Texte de la deuxième ligne d'annonce.
 */
void TFT_Annonce (char * ligne1, char * ligne2)
{
	ILI9341_DrawFilledRectangle(20, 60, 300, 150, ILI9341_COLOR_WHITE);
	ILI9341_DrawRectangle(20, 60, 300, 150, ILI9341_COLOR_BLACK);
	ILI9341_PutBigs(40, 75, ligne1, &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 2, 2);
	ILI9341_PutBigs(40, 105, ligne2, &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 2, 2);

	ILI9341_Puts(145, 130, "OK", &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_GRAY);
}


