/**
  ******************************************************************************
  * @file    hcsr04.h
  * @author  Mathias Jomeau
  * @date    14-January-2024
  * @brief   Fichier d'en-tête pour le capteur de distance (HC-SR04)
  ******************************************************************************
*/

#ifndef HCSR_04_H_
#define HCSR_04_H_

#include "config.h"
#include "macro_types.h"
#include "HC-SR04/HCSR04.h"

/**
 * @brief Initialise un capteur de distance ultrasonique HCSR04.
 * Elle doit être appelée avant toute utilisation du capteur
 * @param id Pointeur pour stocker l'identifiant unique du capteur.
 * @param TRIG_GPIO GPIO pour la broche TRIG du capteur.
 * @param TRIG_PIN Numéro de broche pour la broche TRIG du capteur.
 * @param ECHO_GPIO GPIO pour la broche ECHO du capteur.
 * @param ECHO_PIN Numéro de broche pour la broche ECHO du capteur.
 */
void HCSR04_Init(uint8_t * id, GPIO_TypeDef * TRIG_GPIO, uint16_t TRIG_PIN, GPIO_TypeDef * ECHO_GPIO, uint16_t ECHO_PIN);

/**
 * @brief Obtient la distance mesurée par un capteur de distance ultrasonique HCSR04.
 * @param id_sensor Identifiant du capteur.
 * @param distance Pointeur pour stocker la distance mesurée.
 * @return TRUE si la mesure de la distance est réussie, FALSE sinon.
 */
bool_e HCSR04_GetDistance(uint8_t id_sensor, uint16_t * distance);

#endif /* HCSR_04_H_ */
