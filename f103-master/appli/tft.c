#include "tft.h"
#include "stm32f1_ili9341.h"
#include <stdio.h>
#include <stdlib.h>


typedef struct{
	uint16_t position_x;
	uint16_t position_y;
	char text[40];
	FontDef_t * font;
	uint16_t foreground;
	uint16_t background;
	uint8_t screen;
	uint8_t id;
}DynamicLine_t;

void updateDynamicLine_Text(DynamicLine_t pDynamicLine, char * new_text);
void updateDynamicLine_Foreground(DynamicLine_t pDynamicLine, uint16_t newForeground);
void updateDynamicLine_Background(DynamicLine_t pDynamicLine, uint16_t newBackground);
void displayDynamicLine(DynamicLine_t pDynamicLine);

DynamicLine_t Screens_Addresse [NB_SCREENS][NB_MAX_PARAMETERS];

void displayDynamicLine(DynamicLine_t pDynamicLine)
{
	Screens_Addresse[pDynamicLine.screen][pDynamicLine.id] = pDynamicLine;

    ILI9341_Puts(pDynamicLine.position_x, pDynamicLine.position_y, pDynamicLine.text,
                     pDynamicLine.font, pDynamicLine.foreground, pDynamicLine.background);
}

void updateDynamicLine_Text(DynamicLine_t pDynamicLine, char * new_text)
{
	sprintf(pDynamicLine.text, "                                ");
	snprintf(pDynamicLine.text, sizeof(pDynamicLine.text), "%s", new_text);
	Screens_Addresse[pDynamicLine.screen][pDynamicLine.id] = pDynamicLine;
	displayDynamicLine(pDynamicLine);
}


void updateDynamicLine_Foreground(DynamicLine_t pDynamicLine, uint16_t newForeground)
{
	pDynamicLine.foreground = newForeground;
	Screens_Addresse[pDynamicLine.screen][pDynamicLine.id] = pDynamicLine;
	displayDynamicLine(pDynamicLine);
}

void updateDynamicLine_Background(DynamicLine_t pDynamicLine, uint16_t newBackground)
{
	pDynamicLine.background = newBackground;
	Screens_Addresse[pDynamicLine.screen][pDynamicLine.id] = pDynamicLine;
	displayDynamicLine(pDynamicLine);
}


void TFT_Init(void)
{
	ILI9341_Init();
	ILI9341_Rotate(ILI9341_Orientation_Landscape_2);
	ILI9341_Fill(ILI9341_COLOR_WHITE);
}

void TFT_Acceuil()
{
	DynamicLine_t mode_actif = {30, 60, "Mode actif :", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 0, 0};
	DynamicLine_t mode_auto = {30,90, " - Mode Auto", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 0, 1};
	DynamicLine_t mode_manuel = {30,110, " - Mode Manuel", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 0, 2};
	DynamicLine_t mode_off = {30,130, " - Parametres", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 0, 3};
	DynamicLine_t niveau_cuve = {30,170, "Niveau de la cuve :", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 0, 4};
	DynamicLine_t electrovanne_EP = {30,185, "Electrovanne Cuve :", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 0, 5};
	DynamicLine_t electrovanne_EC = {30,200, "Electrovanne Eau Courante :", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 0, 6};
	DynamicLine_t temp_eau = {30,215, "Temperature :", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 0, 7};

	ILI9341_Fill(ILI9341_COLOR_WHITE);

	ILI9341_PutBigs(100, 20, "HYDRESEO", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 2, 2);

	ILI9341_DrawLine(0, 50, 400, 50, ILI9341_COLOR_BLACK);

	displayDynamicLine(mode_actif);
	displayDynamicLine(mode_auto);
	displayDynamicLine(mode_manuel);
	displayDynamicLine(mode_off);

	ILI9341_DrawLine(0, 160, 400, 160, ILI9341_COLOR_BLACK);

	displayDynamicLine(niveau_cuve);
	displayDynamicLine(electrovanne_EP);
	displayDynamicLine(electrovanne_EC);
	displayDynamicLine(temp_eau);
}

void TFT_Acceuil_Update(uint8_t mode, uint8_t current_mode, uint8_t id_mode,
		uint16_t water_level, uint8_t EC_state, uint8_t EP_state)
{
	switch (mode)
	{
	char water_level_char [30];
	char EP_state_char [40];
	char EC_state_char [40];
		case 1: // Update Current Mode
			switch (current_mode)
			{
				case 0:
					updateDynamicLine_Text(Screens_Addresse[0][0], "Mode actif : Mode Auto  " );
					break;
				case 1:
					updateDynamicLine_Text(Screens_Addresse[0][0], "Mode actif : Mode Manuel" );
					break;
			}
			break;

		case 2: // Update Selected mode
			updateDynamicLine_Foreground(Screens_Addresse[0][1], ILI9341_COLOR_BLACK);
			updateDynamicLine_Background(Screens_Addresse[0][1], ILI9341_COLOR_WHITE);
			updateDynamicLine_Foreground(Screens_Addresse[0][2], ILI9341_COLOR_BLACK);
			updateDynamicLine_Background(Screens_Addresse[0][2], ILI9341_COLOR_WHITE);
			updateDynamicLine_Foreground(Screens_Addresse[0][3], ILI9341_COLOR_BLACK);
			updateDynamicLine_Background(Screens_Addresse[0][3], ILI9341_COLOR_WHITE);

			updateDynamicLine_Foreground(Screens_Addresse[0][id_mode+1], ILI9341_COLOR_WHITE);
			updateDynamicLine_Background(Screens_Addresse[0][id_mode+1], ILI9341_COLOR_GRAY);

			break;

		case 3: // Update Capteurs
			sprintf(water_level_char, "Niveau de la cuve : %d%%", water_level);
			updateDynamicLine_Text(Screens_Addresse[0][4], water_level_char);

			char * state [2] = {"Ouverte", "Fermee"};

			sprintf(EP_state_char, "Electrovanne Cuve : %s", state[EP_state]);
			updateDynamicLine_Text(Screens_Addresse[0][5], EP_state_char);

			sprintf(EC_state_char, "Electrovanne Eau Courante : %s", state[EC_state]);
			updateDynamicLine_Text(Screens_Addresse[0][6], EC_state_char);

			break;
	}




	/*char EC_state_char [30];
	snprintf(EC_state_char, sizeof(EC_state_char), "Electrovanne Cuve : %d", EC_state);
	updateDynamicLine_Text(Screens_Addresse[0][5], EC_state_char);

	char EP_state_char [30];
	snprintf(EP_state_char, sizeof(EP_state_char), "Electrovanne Eau Courante : %d", EP_state);
	updateDynamicLine_Text(Screens_Addresse[0][6], EP_state_char);*/

}
/*

void TFT_Mode_Auto()
{
	ILI9341_Fill(ILI9341_COLOR_WHITE);

	ILI9341_PutBigs(100, 20, "HYDRESEO", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 2, 2);

	ILI9341_DrawLine(0, 50, 400, 50, ILI9341_COLOR_BLACK);

	ILI9341_Puts(135,60, "MODE", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_Puts(100,85, "AUTOMATIQUE", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

	ILI9341_Puts(30,120, " - Retour", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

	ILI9341_DrawLine(0, 160, 400, 160, ILI9341_COLOR_BLACK);

	ILI9341_Puts(30,170, "Niveau de la cuve :", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_Puts(30,185, "Electrovanne Cuve :", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_Puts(30,200, "Electrovanne Eau Courante :", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_Puts(30,215, "Temperature de l'eau :", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

}

void TFT_Mode_Manuel()
{
	ILI9341_Fill(ILI9341_COLOR_WHITE);

	ILI9341_PutBigs(100, 20, "HYDRESEO", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 2, 2);

	ILI9341_DrawLine(0, 50, 400, 50, ILI9341_COLOR_BLACK);

	ILI9341_Puts(100,60, "MODE MANUEL", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);


	ILI9341_Puts(30,90, " - Vanne EP", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_Puts(30,110, " - Vanne EC", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_Puts(30,130, " - Retour", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

	ILI9341_DrawLine(0, 160, 400, 160, ILI9341_COLOR_BLACK);

	ILI9341_Puts(30,170, "Niveau de la cuve :", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_Puts(30,185, "Electrovanne Cuve :", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_Puts(30,200, "Electrovanne Eau Courante :", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_Puts(30,215, "Temperature de l'eau :", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

}

void TFT_Update_capteurs(uint16_t water_level, uint8_t EC_state, uint8_t EP_state){

	char *etats[] = {"Ouverte", "Fermee"};

	//Reint des valeurs pour les capteurs pour éviter tout soucis d'affichage
	ILI9341_Puts(30,170, "                                              ", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_Puts(30,185, "                                              ", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_Puts(30,200, "                                              ", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_Puts(30,215, "                                              ", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);


	// Conversion des données en char
	char water_level_char[30];
	char EC_state_char[50];
	char EP_state_char[50];

	snprintf(water_level_char, sizeof(water_level_char), "Niveau de la cuve : %d", water_level);
	snprintf(EC_state_char, sizeof(EC_state_char), "Electrovanne Eau courante : %s", etats[EC_state]);
	snprintf(EP_state_char, sizeof(EP_state_char), "Electrovanne Eau pluie : %s", etats[EP_state]);

	ILI9341_Puts(30,170, water_level_char, &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_Puts(30,185, EP_state_char, &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_Puts(30,200, EC_state_char, &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_Puts(30,215, "Temperature de l'eau : 20 C", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
}

void TFT_Select_Mode(uint8_t mode){
	// Réinit des couleurs
	ILI9341_Puts(30,90, " - Mode Auto", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_Puts(30,110, " - Mode Manuel", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_Puts(30,130, " - Mode Off", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

	switch (mode)
	{
		// Mode Auto
		case 0:
			ILI9341_Puts(30,90, " - Mode Auto", &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_GRAY);
			break;

		// Mode Manuel
		case 1:
			ILI9341_Puts(30,110, " - Mode Manuel", &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_GRAY);
			break;

		// Mode Off
		case 2:
			ILI9341_Puts(30,130, " - Mode Off", &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_GRAY);
			break;
	}
}

void TFT_Change_CurrentMode (uint8_t mode)
{
	// Réinit du texte
	ILI9341_Puts(30,60, "Mode actif :               ", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

	switch (mode)
	{
		// Mode Auto
		case 0:
			ILI9341_Puts(30,60, "Mode actif : Mode Auto", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
			break;

		// Mode Manuel
		case 1:
			ILI9341_Puts(30,60, "Mode actif : Mode Manuel", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
			break;

		// Mode Off
		case 2:
			ILI9341_Puts(30,60, "Mode actif : Mode Off", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
			break;
	}
}
 */


