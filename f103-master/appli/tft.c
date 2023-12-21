#include "stm32f1_ili9341.h"
#include "tft.h"

#include <stdio.h>



void TFT_Init(void)
{
	ILI9341_Init();
	ILI9341_Rotate(ILI9341_Orientation_Landscape_2);
	ILI9341_Fill(ILI9341_COLOR_WHITE);

}

void TFT_Acceuil(uint16_t water_level)
{
	// Conversion des données en char
	char water_level_char[30];
	//sprintf(water_level_char, "%d", water_level);
	snprintf(water_level_char, sizeof(water_level_char), "Niveau de la cuve : %d", water_level);
	// SOUCIS : ECRITURE SUR 2/3/4 CHIFFRES SE CHAUVAUCHENT, SOIT METTRE DES 0 DEVANT POUR COMPLETER LES 4 CHIFFRE
	// SOIT METTRE DES ESPACES

	// Informations système
	ILI9341_PutBigs(100, 20, "HYDRESEO", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 2, 2);

	ILI9341_DrawLine(0, 50, 400, 50, ILI9341_COLOR_BLACK);

	ILI9341_Puts(30,60, "Mode actif : Mode Auto", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_Puts(30,90, " - Mode Auto", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_Puts(30,110, " - Mode Manuel", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_Puts(30,130, " - Mode Off", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

	ILI9341_DrawLine(0, 160, 400, 160, ILI9341_COLOR_BLACK);

	ILI9341_Puts(30,170, water_level_char, &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_Puts(30,185, "Electrovanne Cuve : Ouverte", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_Puts(30,200, "Electrovanne Eau Courante : Ouverte", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

	ILI9341_Puts(30,215, "Temperature de l'eau : 20 C", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);



}




