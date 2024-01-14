/**
  ******************************************************************************
  * @file    electrovanne.c
  * @author  Mathias Jomeau
  * @date    14-January-2024
  * @brief   Fonctions pour la gestion des électrovannes
  ******************************************************************************
*/

#include "electrovanne.h"
#include "stm32f1_gpio.h"

#define NUMBER_ELECTROVANNE 2

/**
* @struct electrovanne_e
* @brief Structure définissant une électrovanne
*/
typedef struct {
    GPIO_TypeDef * GPIO; /**< Pointeur vers le GPIO de l'électrovanne. */
    uint16_t PIN; /**< Numéro de broche de l'électrovanne. */
    bool_e state; /**< État actuel de l'électrovanne (activée ou désactivée). */
} electrovanne_t;

electrovanne_t electrovannes[NUMBER_ELECTROVANNE];

/**
 * @brief Ajoute une électrovanne au système.
 * Elle doit être appelée avant toute utilisation d'une electrovanne
 * @param id : Identifiant unique de l'électrovanne.
 * @param GPIO : GPIO associé à l'électrovanne.
 * @param PIN : Numéro de broche associé à l'électrovanne.
 */
void ELECTROVANNE_Add(uint8_t id, GPIO_TypeDef * GPIO, uint16_t PIN)
{
	electrovannes[id].GPIO = GPIO;
	electrovannes[id].PIN = PIN;

	BSP_GPIO_PinCfg(GPIO, PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
}

/**
 * @brief Définit l'état d'une électrovanne.
 * @param id : Identifiant de l'électrovanne.
 * @param state : Nouvel état de l'électrovanne (TRUE pour activé, FALSE pour désactivé).
 */
void ELECTROVANNE_Set(uint8_t id, bool_e state)
{
	electrovannes[id].state = state;
	HAL_GPIO_WritePin(electrovannes[id].GPIO, electrovannes[id].PIN, state);
}

/**
 * @brief Obtient l'état actuel de l'électrovanne spécifiée.
 * @param id : Identifiant de l'électrovanne.
 * @return État actuel de l'électrovanne (TRUE si activé, FALSE si désactivé).
 */
bool_e ELECTROVANNE_GetState(uint8_t id)
{
	return electrovannes[id].state;
}
