#include "fctn_HCSR04.h"
#include "config.h"
#include "HC-SR04/HCSR04.h"

/// Machine à états demo state-machine de la librairie
uint16_t HCSR04_demo_state_machine(void)
	{
		typedef enum
		{
			INIT,
			FAIL,
			LAUNCH_MEASURE,
			RUN,
			WAIT_DURING_MEASURE,
			WAIT_BEFORE_NEXT_MEASURE
		}state_e;

		static state_e state = INIT;
		static uint32_t tlocal;
		static uint8_t id_sensor;
		uint16_t distance;

		//ne pas oublier d'appeler en t�che de fond cette fonction.
		HCSR04_process_main();


		switch(state)
		{
			case INIT:
				if(HCSR04_add(&id_sensor, GPIOB, GPIO_PIN_6, GPIOB, GPIO_PIN_7) != HAL_OK)
				{
					printf("HCSR04 non ajout� - erreur g�nante\n");
					state = FAIL;
				}
				else
				{
					printf("HCSR04 ajout�\n");
					state = LAUNCH_MEASURE;
				}
				break;
			case LAUNCH_MEASURE:
				HCSR04_run_measure(id_sensor);
				tlocal = HAL_GetTick();
				state = WAIT_DURING_MEASURE;
				break;
			case WAIT_DURING_MEASURE:
				switch(HCSR04_get_value(id_sensor, &distance))
				{
					case HAL_BUSY:
						//rien � faire... on attend...
						break;
					case HAL_OK:
						printf("sensor %d - distance : %d\n", id_sensor, distance);
						state = WAIT_BEFORE_NEXT_MEASURE;
						return distance;
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
	}


uint16_t getDistance(void){
	HCSR04_demo_state_machine();
}

