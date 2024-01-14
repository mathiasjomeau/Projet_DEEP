/**
  ******************************************************************************
  * @file    hcsr04.c
  * @author  Mathias Jomeau
  * @date    14-January-2024
  * @brief   Fonctions pour la gestion du capteur de distance (HC-SR04)
  ******************************************************************************
*/

#include "hcsr04.h"

#define PERIOD_MEASURE			100

/**
 * @brief Initialise un capteur de distance ultrasonique HCSR04.
 * Elle doit être appelée avant toute utilisation du capteur
 * @param id Pointeur pour stocker l'identifiant unique du capteur.
 * @param TRIG_GPIO GPIO pour la broche TRIG du capteur.
 * @param TRIG_PIN Numéro de broche pour la broche TRIG du capteur.
 * @param ECHO_GPIO GPIO pour la broche ECHO du capteur.
 * @param ECHO_PIN Numéro de broche pour la broche ECHO du capteur.
 */
void HCSR04_Init(uint8_t * id, GPIO_TypeDef * TRIG_GPIO, uint16_t TRIG_PIN, GPIO_TypeDef * ECHO_GPIO, uint16_t ECHO_PIN)
{
    // Simplification de l'initialisation en appelant la fonction HCSR04_add
	HCSR04_add(id, TRIG_GPIO, TRIG_PIN, ECHO_GPIO, ECHO_PIN);
}

/**
 * @brief Obtient la distance mesurée par un capteur de distance ultrasonique HCSR04.
 * @param id_sensor Identifiant du capteur.
 * @param distance Pointeur pour stocker la distance mesurée.
 * @return TRUE si la mesure de la distance est réussie, FALSE sinon.
 */
bool_e HCSR04_GetDistance(uint8_t id_sensor, uint16_t * distance)
{
	/**
	 * @enum state_e
	 * @brief Enumération des différents états pour la mesure du capteur de distance.
	 */
	typedef enum {
	    LAUNCH_MEASURE, /**< Lancement de la mesure. */
	    WAIT_DURING_MEASURE, /**< Attente pendant la mesure. */
	    WAIT_BEFORE_NEXT_MEASURE /**< Attente avant la prochaine mesure. */
	} state_e;

	bool_e ret = FALSE;

	static state_e state = LAUNCH_MEASURE;
	static uint32_t tlocal;

	switch(state)
	{
		case LAUNCH_MEASURE: // Lance la mesure
			HCSR04_run_measure(id_sensor);
			tlocal = HAL_GetTick();
			state = WAIT_DURING_MEASURE;
			break;
		case WAIT_DURING_MEASURE:
			switch(HCSR04_get_value(id_sensor, distance))
			{
				case HAL_BUSY: // La mesure est en cours, attendre
					break;
				case HAL_OK: // La mesure est terminée avec succès
					state = WAIT_BEFORE_NEXT_MEASURE;
					ret = TRUE;
					break;
				case HAL_ERROR: // Erreur ou dépassement de temps, passer à l'étape suivante
					state = WAIT_BEFORE_NEXT_MEASURE;
					break;

				case HAL_TIMEOUT:
					state = WAIT_BEFORE_NEXT_MEASURE;
					break;
			}
			break;
		case WAIT_BEFORE_NEXT_MEASURE: // Attend la période spécifiée avant la prochaine mesure
			if(HAL_GetTick() > tlocal + PERIOD_MEASURE)
				state = LAUNCH_MEASURE;
			break;
		default:
			break;
	}

	return ret;
}
