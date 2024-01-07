#ifndef ELECTROVANNE_H_
#define ELECTROVANNE_H_

#include "config.h"
#include "macro_types.h"

#define NUMBER_ELECTROVANNE 2

void ELECTROVANNE_Add(uint8_t id, GPIO_TypeDef * GPIO, uint16_t PIN);

void ELECTROVANNE_Set(uint8_t id, bool_e state);

bool_e ELECTROVANNE_GetState(uint8_t id);

#endif /* ELECTROVANNE_H_ */
