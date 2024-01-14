/**
  ******************************************************************************
  * @file    button.c
  * @author  Mathias Jomeau
  * @date    14-January-2024
  * @brief   Fonctions pour la gestion des boutons
  ******************************************************************************
*/

#include "button.h"
#include "stm32f1_gpio.h"
#include "macro_types.h"
#include "systick.h"

#define LONG_PRESS_DURATION	1000	//en ms
#define NB_BUTTONS 3

/**
 * @enum button_event_e
 * @brief Enumération des différents états possibles d'un bouton.
 */
typedef enum {
    BUTTON_STATE_INEXISTANT, /**< État inexistant du bouton. */
    INIT, /**< État d'initialisation du bouton. */
    WAIT_BUTTON, /**< État d'attente de l'appui sur le bouton. */
    BUTTON_PRESSED, /**< État de détection d'appui sur le bouton. */
    WAIT_RELEASE /**< État d'attente du relâchement du bouton après un appui. */
} button_state_e;

/**
 * @struct button_t
 * @brief Structure représentant un bouton.
 */
typedef struct {
    GPIO_TypeDef * GPIO; /**< Pointeur vers le GPIO du bouton. */
    uint16_t PIN; /**< Numéro de broche du bouton. */
    button_state_e state; /**< État actuel du bouton. */
    button_event_e button_event; /**< Événement du bouton (court appui, long appui, etc.). */
} button_t;

static button_t buttons[NB_BUTTONS];

static void process_ms(void);

static volatile bool_e flag_10ms;
static volatile uint32_t t = 0;

/**
 * @brief Fonction de traitement appelée périodiquement toutes les millisecondes pour gérer une période de 10ms
 * @pre Fonction ajouté à la routine d'interruption du périphique SYSTICK
 */
static void process_ms(void)
{
	static uint32_t t10ms = 0;
	t10ms = (t10ms + 1)%10;		//incr�mentation de la variable t10ms (modulo 10 !)
	if(!t10ms)
		flag_10ms = TRUE; //toutes les 10ms, on l�ve ce flag.
	if(t)
		t--;
}

/**
 * @brief Ajoute un bouton au système.
 * @param id : Identifiant unique du bouton.
 * @param GPIO : GPIO associé au bouton.
 * @param PIN : Numéro de broche associé au bouton.
 */
void BUTTON_add(uint8_t id, GPIO_TypeDef * GPIO, uint16_t PIN)
{
	if(buttons[id].state == BUTTON_STATE_INEXISTANT)
	{
		buttons[id].state = INIT;
		buttons[id].GPIO = GPIO;
		buttons[id].PIN  = PIN;
		buttons[id].button_event = BUTTON_EVENT_NONE;

		BSP_GPIO_PinCfg(GPIO, PIN, GPIO_MODE_INPUT,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);

		Systick_add_callback_function(&process_ms);
	}
}

/**
 * @brief Gère l'état d'un bouton dans la machine à états.
 * @param id Identifiant du bouton à gérer.
 */
void BUTTON_state_machine(uint8_t id)
{
	if (flag_10ms)
	{
		bool_e current_button;
		flag_10ms = FALSE;

		current_button = !HAL_GPIO_ReadPin(buttons[id].GPIO, buttons[id].PIN);

		switch(buttons[id].state)
		{
			case BUTTON_STATE_INEXISTANT:
				break;
			case INIT:
				buttons[id].state = WAIT_BUTTON;
				break;
			case WAIT_BUTTON:
				if(current_button)
				{
					t=LONG_PRESS_DURATION;
					buttons[id].state = BUTTON_PRESSED;
				}
				break;
			case BUTTON_PRESSED:
				if(t==0)
				{
					buttons[id].button_event = BUTTON_EVENT_LONG_PRESS;
					buttons[id].state = WAIT_RELEASE;						}
				else if(!current_button)
				{
					buttons[id].button_event = BUTTON_EVENT_SHORT_PRESS;
					buttons[id].state = WAIT_BUTTON;
				}
				break;

			case WAIT_RELEASE:
				if(!current_button)
				{
					buttons[id].state = WAIT_BUTTON;
				}
				break;
			default:
				buttons[id].state = INIT;
				break;
		}
	}
}

/**
 * @brief Obtient l'événement du bouton spécifié.
 * @param id Identifiant du bouton.
 * @return Événement du bouton (court, long, ou aucun).
 */
button_event_e BUTTON_getEvent(uint8_t id)
{
	button_event_e ret = buttons[id].button_event;
	buttons[id].button_event = BUTTON_EVENT_NONE;
	return ret;
}
