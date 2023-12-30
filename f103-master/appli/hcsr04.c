#include "hcsr04.h"


void HCSR04_Init(hcsr_04_t * hcsr04)
{
	HCSR04_add(&hcsr04->id_sensor, hcsr04->GPIO_TRIG, hcsr04->PIN_TRIG, hcsr04->GPIO_ECHO, hcsr04->PIN_ECHO);
}

bool_e HCSR04_GetDistance(uint8_t id_sensor, uint16_t * distance)
{
	static uint32_t tlocal;
	uint16_t PERIOD_MEASURE = 100;

	typedef enum{
			LAUNCH_MEASURE,
			WAIT_DURING_MEASURE,
			WAIT_BEFORE_NEXT_MEASURE
	}state_hcsr04;
	static state_hcsr04 state_capteur = LAUNCH_MEASURE;
	bool_e ret = FALSE;

	switch(state_capteur){
		case LAUNCH_MEASURE:
			HCSR04_run_measure(id_sensor);
			tlocal = HAL_GetTick();
			state_capteur = WAIT_DURING_MEASURE;
			break;
		case WAIT_DURING_MEASURE:
			switch(HCSR04_get_value(id_sensor, distance))
			{
				case HAL_BUSY:
					//rien � faire... on attend...
					break;
				case HAL_OK:
					ret = TRUE;
					state_capteur = WAIT_BEFORE_NEXT_MEASURE;
					break;
				case HAL_ERROR:
					//printf("sensor %d - erreur ou mesure non lanc�e\n", hcsr04.id_sensor);
					state_capteur = WAIT_BEFORE_NEXT_MEASURE;
					break;

				case HAL_TIMEOUT:
					//printf("sensor %d - timeout\n", hcsr04.id_sensor);
					state_capteur = WAIT_BEFORE_NEXT_MEASURE;
					break;
			}
			break;

		case WAIT_BEFORE_NEXT_MEASURE:
			if(HAL_GetTick() > tlocal + PERIOD_MEASURE)
				state_capteur = LAUNCH_MEASURE;
			break;

		default:
			break;
	}

	return ret;
}
