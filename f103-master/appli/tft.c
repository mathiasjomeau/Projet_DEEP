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
	uint8_t id;
}DynamicLine_t;

void updateDynamicLine_Text(DynamicLine_t pDynamicLine, char * new_text);
void updateDynamicLine_Foreground(DynamicLine_t pDynamicLine, uint16_t newForeground);
void updateDynamicLine_Background(DynamicLine_t pDynamicLine, uint16_t newBackground);
void displayDynamicLine(DynamicLine_t pDynamicLine);

void displayTitle(void);
void displayInformationsSensors(void);

void updateSelectedMenu(uint8_t ids[], uint8_t id_modeselected);

DynamicLine_t Screens_Addresse [NB_MAX_PARAMETERS];

void displayDynamicLine(DynamicLine_t pDynamicLine)
{
	Screens_Addresse[pDynamicLine.id] = pDynamicLine;

    ILI9341_Puts(pDynamicLine.position_x, pDynamicLine.position_y, pDynamicLine.text,
                     pDynamicLine.font, pDynamicLine.foreground, pDynamicLine.background);
}

void updateDynamicLine_Text(DynamicLine_t pDynamicLine, char * new_text)
{
	sprintf(pDynamicLine.text, "                                   ");
	snprintf(pDynamicLine.text, sizeof(pDynamicLine.text), "%s", new_text);
	Screens_Addresse[pDynamicLine.id] = pDynamicLine;
	displayDynamicLine(pDynamicLine);
}


void updateDynamicLine_Foreground(DynamicLine_t pDynamicLine, uint16_t newForeground)
{
	pDynamicLine.foreground = newForeground;
	Screens_Addresse[pDynamicLine.id] = pDynamicLine;
	displayDynamicLine(pDynamicLine);
}

void updateDynamicLine_Background(DynamicLine_t pDynamicLine, uint16_t newBackground)
{
	pDynamicLine.background = newBackground;
	Screens_Addresse[pDynamicLine.id] = pDynamicLine;
	displayDynamicLine(pDynamicLine);
}

void displayTitle()
{
	ILI9341_Fill(ILI9341_COLOR_WHITE);
	ILI9341_PutBigs(100, 20, "HYDRESEO", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 2, 2);
	ILI9341_DrawLine(0, 50, 400, 50, ILI9341_COLOR_BLACK);
}

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

void TFT_InformationsSensors_Update(uint16_t water_level, bool_e EC_state, bool_e EP_state, float eau_temperature)
{
	char water_level_char [30];
	char EP_state_char [40];
	char EC_state_char [40];
	char eau_temperature_char [30];

	sprintf(water_level_char, "Niveau de la cuve : %d%%", water_level);
	updateDynamicLine_Text(Screens_Addresse[4], water_level_char);

	char * state [2] = {"Fermee", "Ouverte"};

	sprintf(EC_state_char, "Electrovanne Cuve : %s ", state[EC_state]);
	updateDynamicLine_Text(Screens_Addresse[5], EC_state_char);

	sprintf(EP_state_char, "Electrovanne Eau Courante : %s ", state[EP_state]);
	updateDynamicLine_Text(Screens_Addresse[6], EP_state_char);

	sprintf(eau_temperature_char, "Temperature Eau : %.1f C", eau_temperature);
	updateDynamicLine_Text(Screens_Addresse[7], eau_temperature_char);
}

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


void TFT_Init(void)
{
	ILI9341_Init();
	ILI9341_Rotate(ILI9341_Orientation_Landscape_2);
	ILI9341_Fill(ILI9341_COLOR_WHITE);
}

void TFT_Acceuil()
{
	DynamicLine_t mode_actif = {30, 60, "Mode actif :", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 0};
	DynamicLine_t mode_auto = {30,90, " - Mode Auto", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 1};
	DynamicLine_t mode_manuel = {30,110, " - Mode Manuel", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 2};
	DynamicLine_t mode_off = {30,130, " - Parametres", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 3};

	displayTitle();

	displayDynamicLine(mode_actif);
	displayDynamicLine(mode_auto);
	displayDynamicLine(mode_manuel);
	displayDynamicLine(mode_off);

	ILI9341_DrawLine(0, 160, 400, 160, ILI9341_COLOR_BLACK);

	displayInformationsSensors();
}

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

void TFT_Mode_Manuel()
{
	DynamicLine_t electrovanne_cuve = {30, 90, " - Electrovanne Cuve", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 10};
	DynamicLine_t electrovanne_eau_courante = {30,110, " - Electrovanne Eau", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 11};
	DynamicLine_t retour = {30,130, " - Retour", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 12};

	displayTitle();

	ILI9341_Puts(100, 60, "Mode Manuel", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

	displayDynamicLine(electrovanne_cuve);
	displayDynamicLine(electrovanne_eau_courante);
	displayDynamicLine(retour);

	ILI9341_DrawLine(0, 160, 400, 160, ILI9341_COLOR_BLACK);

	displayInformationsSensors();
}

void TFT_Mode_Manual_Update(uint8_t id_mode)
{
	uint8_t ids_mode [4] = {4, 10, 11, 12};
	id_mode = (uint8_t) (id_mode + 10);
	updateSelectedMenu(ids_mode, id_mode);
}

void TFT_Mode_Parametre()
{
	DynamicLine_t modif_taille_cuve = {30, 90, " - Taille Cuve", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 13};
	DynamicLine_t activation_annonces = {30,110, " - Activation Alertes", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 14};
	DynamicLine_t retour = {30,130, " - Retour", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 15};

	displayTitle();

	ILI9341_Puts(100, 60, "Pamametres", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

	displayDynamicLine(modif_taille_cuve);
	displayDynamicLine(activation_annonces);
	displayDynamicLine(retour);

	ILI9341_DrawLine(0, 160, 400, 160, ILI9341_COLOR_BLACK);

	displayInformationsSensors();
}

void TFT_Mode_Parametre_Update(uint8_t id_mode)
{
	uint8_t ids_mode [4] = {4, 13, 14, 15};
	id_mode = (uint8_t) (id_mode + 13);
	updateSelectedMenu(ids_mode, id_mode);
}

void TFT_Parametre_Cuve()
{
	DynamicLine_t taille_cuve = {130, 110, "", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 16};

	displayTitle();

	ILI9341_DrawFilledRectangle(20, 60, 300, 150, ILI9341_COLOR_WHITE);
	ILI9341_DrawRectangle(20, 60, 300, 150, ILI9341_COLOR_BLACK);

	ILI9341_Puts(60, 80, "Taille de la cuve : ", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

	displayDynamicLine(taille_cuve);

	ILI9341_Puts(260, 130, "OK", &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_GRAY);

	ILI9341_DrawLine(0, 160, 400, 160, ILI9341_COLOR_BLACK);

	displayInformationsSensors();
}

void TFT_Parametre_Cuve_Update(uint16_t taille_cuve)
{
	char taille_cuve_char [30];
	sprintf(taille_cuve_char, "%d dm", taille_cuve);
	updateDynamicLine_Text(Screens_Addresse[16], taille_cuve_char);
}

void TFT_Parametre_Alertes()
{
	DynamicLine_t activation_true = {135, 115, "", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 17};

	displayTitle();

	ILI9341_DrawFilledRectangle(20, 60, 300, 150, ILI9341_COLOR_WHITE);
	ILI9341_DrawRectangle(20, 60, 300, 150, ILI9341_COLOR_BLACK);

	ILI9341_Puts(60, 80, "Activation Alertes : ", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

	displayDynamicLine(activation_true);

	ILI9341_Puts(260, 130, "OK", &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_GRAY);

	ILI9341_DrawLine(0, 160, 400, 160, ILI9341_COLOR_BLACK);

	displayInformationsSensors();
}

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

void TFT_Annonce (char * ligne1, char * ligne2)
{
	ILI9341_DrawFilledRectangle(20, 60, 300, 150, ILI9341_COLOR_WHITE);
	ILI9341_DrawRectangle(20, 60, 300, 150, ILI9341_COLOR_BLACK);
	ILI9341_PutBigs(40, 75, ligne1, &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 2, 2);
	ILI9341_PutBigs(40, 105, ligne2, &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 2, 2);

	ILI9341_Puts(145, 130, "OK", &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_GRAY);
}


