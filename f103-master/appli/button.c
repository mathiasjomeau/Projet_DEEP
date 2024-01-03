/*
 * button.c
 *
 *  Created on: 26 juin 2019
 *      Author: Nirgal
 */
#include "button.h"
#include "stm32f1_gpio.h"
#include "macro_types.h"
#include "systick.h"

#define LONG_PRESS_DURATION	1000	//unit� : [1ms] => 1 seconde.

typedef enum
{
	BUTTON_STATE_INEXISTANT,
	INIT,
	WAIT_BUTTON,
	BUTTON_PRESSED,
	WAIT_RELEASE
}button_state_e;

typedef struct{
		GPIO_TypeDef * GPIO;
		uint16_t PIN;
		button_state_e state;
		button_event_e button_event;
}button_t;

static button_t buttons[NB_BUTTONS];

static void process_ms(void);


static volatile bool_e flag_10ms;
static volatile uint32_t t = 0;

static void process_ms(void)
{
	static uint32_t t10ms = 0;
	t10ms = (t10ms + 1)%10;		//incr�mentation de la variable t10ms (modulo 10 !)
	if(!t10ms)
		flag_10ms = TRUE; //toutes les 10ms, on l�ve ce flag.
	if(t)
		t--;
}

void BUTTON_add(uint8_t id, GPIO_TypeDef * GPIO, uint16_t PIN)
{
	//for(uint8_t i = 0; i<NB_BUTTONS; i++)
	//{
		if(buttons[id].state == BUTTON_STATE_INEXISTANT)
		{
			//*id = i;
			buttons[id].state = INIT;
			buttons[id].GPIO = GPIO;
			buttons[id].PIN  = PIN;
			buttons[id].button_event = BUTTON_EVENT_NONE;

			BSP_GPIO_PinCfg(GPIO, PIN, GPIO_MODE_INPUT,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);

			Systick_add_callback_function(&process_ms);
		}
	//}
}

/**
	Elle doit �tre appel�e en boucle tr�s r�guli�rement.
	Pr�condition : avoir appel� auparavant BUTTON_init();
	Si un appui vient d'�tre fait, elle renverra BUTTON_EVENT_SHORT_PRESS ou BUTTON_EVENT_LONG_PRESS
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
					buttons[id].button_event = BUTTON_EVENT_NONE;
					break;
				case WAIT_BUTTON:
					if(current_button)
					{
						printf("[%d] button pressed\n", id);
						t=LONG_PRESS_DURATION;
						buttons[id].state = BUTTON_PRESSED;
					}
					break;
				case BUTTON_PRESSED:
					if(t==0)
					{
						buttons[id].button_event = BUTTON_EVENT_LONG_PRESS;
						printf("[%d] long press event\n", id);
						buttons[id].state = WAIT_RELEASE;						}
					else if(!current_button)
					{
						buttons[id].button_event = BUTTON_EVENT_SHORT_PRESS;
						printf("[%d] short press event\n", id);
						buttons[id].state = WAIT_BUTTON;
					}
					break;

				case WAIT_RELEASE:
					if(!current_button)
					{
						printf("[%d] release button after long press\n", id);
						buttons[id].state = WAIT_BUTTON;
					}
					break;
				default:
					buttons[id].state = INIT;
					break;
			}
	}
}

button_event_e BUTTON_getEvent(uint8_t id)
{
	return buttons[id].button_event;
}






