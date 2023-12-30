#ifndef HCSR_04_H_
#define HCSR_04_H_

#include "config.h"
#include "macro_types.h"

#include "HC-SR04/HCSR04.h"


void HCSR04_Init(hcsr_04_t * hcsr04);

bool_e HCSR04_GetDistance(uint8_t id_sensor, uint16_t * distance);


#endif /* HCSR_04_H_ */
