/**
  ******************************************************************************
  * @file    mcp9701.c
  * @author  Mathias Jomeau
  * @date    14-January-2024
  * @brief   Fonctions pour la gestion du capteur de température (MCP 9701)
  ******************************************************************************
*/

#include "mcp9701.h"
#include "systick.h"
#include "stm32f1_adc.h"

adc_id_e ID_temperature = ADC_4;

static volatile uint16_t t = 0;

void ADC_process_1ms(void);
float Conversion_TensionToTemp(float tension);

/**
 * @brief Fonction de traitement périodique pour l'ADC.
 * Elle est appelée toutes les 1 ms par la routine d'interruption du SYSTICK.
 * @pre Fonction ajouté à la routine d'interruption du périphique SYSTICK
 */
void ADC_process_1ms(void)
{
	if(t)
		t--;
}

/**
 * @brief Conversion de la tension en température pour le capteur MCP9701.
 * @param tension Tension mesurée en millivolts.
 * @return temperature en degrés Celsius .
 */
float Conversion_TensionToTemp(float tension)
{
    // Caractéristiques du MCP9701
    const float tension_zero_degres = 400.0;  // 500 mV à 0°C
    const float pente_mV_par_degre = 19.5;    // 10 mV/°C

    // Calcul de la température en degrés Celsius
    float temperature = (tension - tension_zero_degres) / pente_mV_par_degre;

    return temperature;
}

/**
 * @brief Initialise le capteur de température MCP9701.
 * Elle doit être appelée avant toute utilisation du capteur
 */
void MCP9701_Init()
{
	// Simplification de l'initialisation en appelant la fonction ADC_Init
	ADC_init();
	Systick_add_callback_function(ADC_process_1ms); // Ajout à la routine d'interruption
}

/**
 * @brief Obtient la température mesurée par le capteur MCP9701.
 * @param temperature Pointeur pour stocker la température mesurée.
 */
void MCP9701_GetTemperature (float * temperature)
{
	int16_t value;
	int16_t millivolt;
	float temp;

	// Obtention de la température toutes les 400ms
	if(!t)
	{
		t = 400;
		value = ADC_getValue(ID_temperature);
		millivolt = (int16_t)((((int32_t)value)*3300)/4096);
		temp = Conversion_TensionToTemp ((float) millivolt);
		*temperature = temp;
	}
}
