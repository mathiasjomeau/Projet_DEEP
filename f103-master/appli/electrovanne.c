#include "electrovanne.h"

#include "stm32f1_gpio.h"


void ELECTROVANNE_Init(electrovanne_t * electrovanne)
{
	BSP_GPIO_PinCfg(electrovanne->GPIO, electrovanne->PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
	ELECTROVANNE_Set(electrovanne);

}

void ELECTROVANNE_Set(electrovanne_t * electrovanne)
{
	HAL_GPIO_WritePin(electrovanne->GPIO, electrovanne->PIN, electrovanne->state);
}
