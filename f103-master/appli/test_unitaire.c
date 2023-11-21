#include "test_unitaire.h"

#include "stm32f1_gpio.h"

#include "stm32f1_ili9341.h" // Import de la librairie de l'ecran TFT


void test_EcranTFT(void){
	// Initionalisation de l'écran TFT
	ILI9341_Init();
	ILI9341_Rotate(ILI9341_Orientation_Landscape_2);
	ILI9341_Fill(ILI9341_COLOR_WHITE);

	// Affichage d'un rectangle bleu
	ILI9341_DrawFilledRectangle(0, 0, 150, 100, ILI9341_COLOR_BLUE);

}

void test_Electrovanne(void){
	//Initialisation du port de la vanne en sortie Push-Pull
	BSP_GPIO_PinCfg(ELECTROVANNE1_GPIO, ELECTROVANNE1_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);

	HAL_GPIO_WritePin(ELECTROVANNE1_GPIO, ELECTROVANNE1_PIN, 1); // vanne fermée
	HAL_GPIO_WritePin(ELECTROVANNE1_GPIO, ELECTROVANNE1_PIN, 0); // vanne ouverte
	HAL_GPIO_WritePin(ELECTROVANNE1_GPIO, ELECTROVANNE1_PIN, 1);
	HAL_GPIO_WritePin(ELECTROVANNE1_GPIO, ELECTROVANNE1_PIN, 0);

}

