/*
 * button.h
 *
 *  Created on: 26 juin 2019
 *      Author: Nirgal
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "stm32f1_gpio.h"
#include "config.h"

typedef enum
{
	BUTTON_EVENT_NONE,
	BUTTON_EVENT_SHORT_PRESS,
	BUTTON_EVENT_LONG_PRESS
}button_event_e;

void BUTTON_add(uint8_t id, GPIO_TypeDef * GPIO, uint16_t PIN);

void BUTTON_process_main(void);

button_event_e BUTTON_getEvent(uint8_t id);


#endif /* BUTTON_H_ */
