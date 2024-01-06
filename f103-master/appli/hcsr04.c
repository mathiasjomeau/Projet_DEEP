#include "hcsr04.h"

#define PERIOD_MEASURE			100

void HCSR04_Init(uint8_t * id, GPIO_TypeDef * TRIG_GPIO, uint16_t TRIG_PIN, GPIO_TypeDef * ECHO_GPIO, uint16_t ECHO_PIN)
{
	HCSR04_add(id, TRIG_GPIO, TRIG_PIN, ECHO_GPIO, ECHO_PIN);
}

bool_e HCSR04_GetDistance(uint8_t id_sensor, uint16_t * distance)
{
	typedef enum
	{
		LAUNCH_MEASURE,
		WAIT_DURING_MEASURE,
		WAIT_BEFORE_NEXT_MEASURE
	}state_e;

	bool_e ret = FALSE;

	static state_e state = LAUNCH_MEASURE;
	static uint32_t tlocal;

	switch(state)
	{
		case LAUNCH_MEASURE:
			HCSR04_run_measure(id_sensor);
			tlocal = HAL_GetTick();
			state = WAIT_DURING_MEASURE;
			break;
		case WAIT_DURING_MEASURE:
			switch(HCSR04_get_value(id_sensor, distance))
			{
				case HAL_BUSY:
					//rien � faire... on attend...
					break;
				case HAL_OK:
					//printf("sensor %d - distance : %s\n", id_sensor, distance);
					state = WAIT_BEFORE_NEXT_MEASURE;
					ret = TRUE;
					break;
				case HAL_ERROR:
					printf("sensor %d - erreur ou mesure non lanc�e\n", id_sensor);
					state = WAIT_BEFORE_NEXT_MEASURE;
					break;

				case HAL_TIMEOUT:
					printf("sensor %d - timeout\n", id_sensor);
					state = WAIT_BEFORE_NEXT_MEASURE;
					break;
			}
			break;
		case WAIT_BEFORE_NEXT_MEASURE:
			if(HAL_GetTick() > tlocal + PERIOD_MEASURE)
				state = LAUNCH_MEASURE;
			break;
		default:
			break;
	}

	return ret;
}
