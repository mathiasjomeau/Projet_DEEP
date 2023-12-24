/*
 * button.h
 *
 *  Created on: 26 juin 2019
 *      Author: Nirgal
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "stm32f1_gpio.h"

typedef enum
{
	BUTTON_EVENT_NONE,
	BUTTON_EVENT_SHORT_PRESS,
	BUTTON_EVENT_LONG_PRESS
}button_event_e;

void BUTTON_init(GPIO_TypeDef * GPIO, uint16_t PIN);

button_event_e BUTTON_state_machine(GPIO_TypeDef * GPIO, uint16_t PIN);


#endif /* BUTTON_H_ */
