/**
  ******************************************************************************
  * @file    button.h
  * @author  Mathias Jomeau
  * @date    14-January-2024
  * @brief   Fichier d'en-tête pour les boutons
  ******************************************************************************
*/

#ifndef BUTTON_H_
#define BUTTON_H_

#include "stm32f1_gpio.h"
#include "config.h"

/**
 * @enum button_event_e
 * @brief Enumération des différents évenements possibles d'un bouton.
 */
typedef enum
{
    BUTTON_EVENT_NONE,
    BUTTON_EVENT_SHORT_PRESS,
    BUTTON_EVENT_LONG_PRESS
} button_event_e;

/**
 * @brief Ajoute un bouton au système.
 * @param id : Identifiant unique du bouton.
 * @param GPIO : GPIO associé au bouton.
 * @param PIN : Numéro de broche associé au bouton.
 */
void BUTTON_add(uint8_t id, GPIO_TypeDef * GPIO, uint16_t PIN);

/**
 * @brief Gère l'état d'un bouton dans la machine à états.
 * @param id Identifiant du bouton à gérer.
 */
void BUTTON_state_machine(uint8_t id);

/**
 * @brief Obtient l'événement du bouton spécifié.
 * @param id Identifiant du bouton.
 * @return Événement du bouton (court, long, ou aucun).
 */
button_event_e BUTTON_getEvent(uint8_t id);

#endif /* BUTTON_H_ */
