#include "tft.h"

#include "stm32f1_ili9341.h"
#include <stdio.h>



void TFT_Init(void)
{
	ILI9341_Init();
	ILI9341_Rotate(ILI9341_Orientation_Landscape_2);
	ILI9341_Fill(ILI9341_COLOR_WHITE);
}

void TFT_Acceuil()
{
	ILI9341_PutBigs(100, 20, "HYDRESEO", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 2, 2);

	ILI9341_DrawLine(0, 50, 400, 50, ILI9341_COLOR_BLACK);

	ILI9341_Puts(30,60, "Mode actif :", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_Puts(30,90, " - Mode Auto", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_Puts(30,110, " - Mode Manuel", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_Puts(30,130, " - Mode Off", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

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

	//sprintf(water_level_char, "%d", water_level);
	snprintf(water_level_char, sizeof(water_level_char), "Niveau de la cuve : %d", water_level);
	// SOUCIS : ECRITURE SUR 2/3/4 CHIFFRES SE CHAUVAUCHENT, SOIT METTRE DES 0 DEVANT POUR COMPLETER LES 4 CHIFFRES
	// SOIT METTRE DES ESPACES
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



