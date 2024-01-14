/**
  ******************************************************************************
  * @file    electrovanne.h
  * @author  Mathias Jomeau
  * @date    14-January-2024
  * @brief   Fichier d'en-tête pour les électrovannes
  ******************************************************************************
*/

#ifndef ELECTROVANNE_H_
#define ELECTROVANNE_H_

#include "config.h"
#include "macro_types.h"

/**
 * @brief Ajoute une électrovanne au système.
 * Elle doit être appelée avant toute utilisation d'une electrovanne
 * @param id : Identifiant unique de l'électrovanne.
 * @param GPIO : GPIO associé à l'électrovanne.
 * @param PIN : Numéro de broche associé à l'électrovanne.
 */
void ELECTROVANNE_Add(uint8_t id, GPIO_TypeDef * GPIO, uint16_t PIN);

/**
 * @brief Définit l'état d'une électrovanne.
 * @param id : Identifiant de l'électrovanne.
 * @param state : Nouvel état de l'électrovanne (TRUE pour activé, FALSE pour désactivé).
 */
void ELECTROVANNE_Set(uint8_t id, bool_e state);

/**
 * @brief Obtient l'état actuel de l'électrovanne spécifiée.
 * @param id : Identifiant de l'électrovanne.
 * @return État actuel de l'électrovanne (TRUE si activé, FALSE si désactivé).
 */
bool_e ELECTROVANNE_GetState(uint8_t id);

#endif /* ELECTROVANNE_H_ */
