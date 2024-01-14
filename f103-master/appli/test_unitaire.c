/**
  ******************************************************************************
  * @file    test_unitaire.c
  * @author  Mathias Jomeau
  * @date    14-January-2024
  * @brief   Fonctions pour effectuer des tests sur les différents composants
  ******************************************************************************
*/

#include "test_unitaire.h"
#include "stm32f1xx_hal.h"

#include "tft.h"
#include "button.h"
#include "electrovanne.h"
#include "hcsr04.h"
#include "mcp9701.h"

/**
 * @brief Fonction de test pour l'écran TFT.
 */
void TEST_EcranTFT(void)
{
	printf("Test de l'ecran TFT \n");
	printf("TEST : L'ecran va se reinitialiser en blanc et afficher des directions x,y \n");
	ILI9341_demo();
	printf("Ecran TFT Teste \n");
}

/**
 * @brief Fonction de test pour les électrovannes.
 */
void TEST_Electrovanne(void)
{
	printf("Test des Electrovannes\n");
	printf("TEST : Les electrovannes seront fermees puis ouvertes pendant 2s et enfin se refermeront au bout de 2s \n");
	ELECTROVANNE_Add(0, ELECTROVANNE0_GPIO, ELECTROVANNE0_PIN);
	ELECTROVANNE_Add(1, ELECTROVANNE1_GPIO, ELECTROVANNE1_PIN);

	ELECTROVANNE_Set(0, FALSE);
	ELECTROVANNE_Set(1, FALSE);
	printf("Electrovannes fermees\n");

	HAL_Delay(2000);

	ELECTROVANNE_Set(0, TRUE);
	ELECTROVANNE_Set(1, TRUE);
	printf("Electrovannes ouvertes \n");

	HAL_Delay(2000);

	ELECTROVANNE_Set(0, FALSE);
	ELECTROVANNE_Set(1, FALSE);
	printf("Electrovannes fermees \n");

	printf("Electrovannes Testees \n");
}

/**
 * @brief Fonction de test pour le capteur de température MCP9701.
 */
void TEST_MCP9701(void)
{
	printf("Test du capteur de temperature\n");
	printf("TEST : Le capteur va renvoyer la temperature et sera affiche dans le serial serie \n");

	float temperature;
	MCP9701_Init();
	MCP9701_GetTemperature(&temperature);
	printf("Temperature : %.1f C", temperature);

	printf("Capteur de Temperature Teste \n");
}

/**
 * @brief Fonction de test pour le capteur de distance HCSR04.
 */
void TEST_HCSR04(void)
{
	printf("Test du capteur de distance\n");
	printf("TEST : Le capteur va renvoyer la distance et sera afficher dans le serial serie \n");

	HCSR04_demo_state_machine();

	printf("Capteur de Distance Teste \n");
}

