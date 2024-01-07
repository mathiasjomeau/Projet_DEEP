#include "electrovanne.h"

#include "stm32f1_gpio.h"

typedef struct{
		GPIO_TypeDef * GPIO;
		uint16_t PIN;
		bool_e state;
}electrovanne_t;

electrovanne_t electrovannes[NUMBER_ELECTROVANNE];

void ELECTROVANNE_Add(uint8_t id, GPIO_TypeDef * GPIO, uint16_t PIN)
{
	electrovannes[id].GPIO = GPIO;
	electrovannes[id].PIN = PIN;

	BSP_GPIO_PinCfg(GPIO, PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
}

void ELECTROVANNE_Set(uint8_t id, bool_e state)
{
	electrovannes[id].state = state;
	HAL_GPIO_WritePin(electrovannes[id].GPIO, electrovannes[id].PIN, state);
}

bool_e ELECTROVANNE_GetState(uint8_t id)
{
	return electrovannes[id].state;
}
