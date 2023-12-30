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

#define NB_BUTTONS 3

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
	for(uint8_t i = 0; i<NB_BUTTONS; i++)
	{
		if(buttons[i].state == BUTTON_STATE_INEXISTANT)
		{
			//*id = i;
			buttons[i].state = INIT;
			buttons[i].GPIO = GPIO;
			buttons[i].PIN  = PIN;
			buttons[i].button_event = BUTTON_EVENT_NONE;

			BSP_GPIO_PinCfg(GPIO, PIN, GPIO_MODE_INPUT,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);

			Systick_add_callback_function(&process_ms);
		}
	}
}

/*void BUTTON_init(button_t * button)
{
	BSP_GPIO_PinCfg(button->GPIO, button->PIN, GPIO_MODE_INPUT,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);

	Systick_add_callback_function(&process_ms);

}*/


/**
	Elle doit �tre appel�e en boucle tr�s r�guli�rement.
	Pr�condition : avoir appel� auparavant BUTTON_init();
	Si un appui vient d'�tre fait, elle renverra BUTTON_EVENT_SHORT_PRESS ou BUTTON_EVENT_LONG_PRESS
*/
void BUTTON_process_main()
{
	static uint8_t test;
	for (uint8_t i ; i<NB_BUTTONS ; i++)
	{
		bool_e current_button;

		if(flag_10ms)	//le cadencement de cette portion de code � 10ms permet d'�liminer l'effet des rebonds sur le signal en provenance du bouton.
		{
			flag_10ms = FALSE;
			current_button = !HAL_GPIO_ReadPin(buttons[i].GPIO, buttons[i].PIN);


			switch(buttons[i].state)
			{
				case BUTTON_STATE_INEXISTANT:
					break;
				case INIT:
					buttons[i].state = WAIT_BUTTON;
					break;
				case WAIT_BUTTON:
					buttons[i].button_event = BUTTON_EVENT_NONE;

					if(current_button)
					{
						if (i==1)
						{
							test++;
						}
						printf("[%d] button pressed\n", i);
						t=LONG_PRESS_DURATION;	//Action r�alis�e sur la transition.
						buttons[i].state = BUTTON_PRESSED;	//Changement d'�tat conditionn� � "if(current_button)"
					}
					break;
				case BUTTON_PRESSED:
					if(t==0)
					{
						buttons[i].button_event = BUTTON_EVENT_LONG_PRESS;
						printf("[%d] long press event\n", i);
						buttons[i].state = WAIT_RELEASE;		//le temps est �coul�, c'�tait un appui long !
					}
					else if(!current_button)
					{
						buttons[i].button_event = BUTTON_EVENT_SHORT_PRESS;
						printf("[%d] short press event\n", i);
						buttons[i].state = WAIT_BUTTON;	//le bouton a �t� rel�ch� avant l'�coulement du temps, c'�tait un appui court !
					}
					break;

				case WAIT_RELEASE:
					if(!current_button)
					{
						printf("[%d] release button after long press\n", i);
						buttons[i].state = WAIT_BUTTON;
					}
					break;
				default:
					buttons[i].state = INIT;
					break;
			}
		}
	}
}

button_event_e BUTTON_getEvent(uint8_t id)
{
	return buttons[id].button_event;
}






