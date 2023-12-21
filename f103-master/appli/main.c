/**
  ******************************************************************************
  * @file    main.c
  * @author  Nirgal
  * @date    03-July-2019
  * @brief   Default main function.
  ******************************************************************************
*/
#include "stm32f1xx_hal.h"
#include "stm32f1_uart.h"
#include "stm32f1_sys.h"
#include "macro_types.h"
#include "systick.h"

#include "test_unitaire.h"

#include "stm32f1_gpio.h"
#include "stm32f1_ili9341.h" // Import de la librairie de l'ecran TFT
#include "stm32f1_adc.h"
#include "HC-SR04/HCSR04.h"

#include "tft.h"

typedef struct{
		char name[20];
		GPIO_TypeDef * GPIO;
		uint16_t PIN;
		u_int8_t state;
}electrovanne_t;

bool_e getWater_level(uint8_t id_sensor, uint16_t * distance);
void setElectrovanne(electrovanne_t ev);
static void state_machine(void);

static volatile uint32_t t = 0;

void process_ms(void)
{
	if(t)
		t--;
}

bool_e getWater_level(uint8_t id_sensor, uint16_t * distance)
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
					printf("sensor %d - erreur ou mesure non lanc�e\n", id_sensor);
					state_capteur = WAIT_BEFORE_NEXT_MEASURE;
					break;

				case HAL_TIMEOUT:
					printf("sensor %d - timeout\n", id_sensor);
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

void setElectrovanne(electrovanne_t ev)
{
	HAL_GPIO_WritePin(ev.GPIO, ev.PIN, ev.state);
}

int main(void)
{
	//Initialisation de la couche logicielle HAL (Hardware Abstraction Layer)
	//Cette ligne doit rester la premi�re �tape de la fonction main().
	HAL_Init();

	//Initialisation de l'UART2 � la vitesse de 115200 bauds/secondes (92kbits/s) PA2 : Tx  | PA3 : Rx.
		//Attention, les pins PA2 et PA3 ne sont pas reli�es jusqu'au connecteur de la Nucleo.
		//Ces broches sont redirig�es vers la sonde de d�bogage, la liaison UART �tant ensuite encapsul�e sur l'USB vers le PC de d�veloppement.
	UART_init(UART2_ID,115200);

	//"Indique que les printf sortent vers le p�riph�rique UART2."
	SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);


	//On ajoute la fonction process_ms � la liste des fonctions appel�es automatiquement chaque ms par la routine d'interruption du p�riph�rique SYSTICK
	Systick_add_callback_function(&process_ms);


	TFT_Init();

	while(1)	//boucle de t�che de fond
	{
		HCSR04_process_main();
		state_machine();
	}
}

static void state_machine(void)
{
	typedef enum{
		INIT,
		ACCUEIL,
		ACTUALISATION,
		MODE_AUTO,
		MODE_MANUEL,
		MODE_OFF
	}state_e;


	static state_e state = INIT;

	switch(state)
	{
		case INIT :

			// Ecran TFT
			TFT_Init();


			static uint16_t water_level;
			static electrovanne_t electrovanne_EC = {"Electrovanne EC", ELECTROVANNE0_GPIO, ELECTROVANNE0_PIN, 0};
			static electrovanne_t electrovanne_EP = {"Electrovanne EP", ELECTROVANNE1_GPIO, ELECTROVANNE1_PIN, 1};

			//static char current_mode[] = "";
			//static float temperature = 0.0;

			// HCSRO4
			static uint8_t id_sensor;
			HCSR04_add(&id_sensor, GPIOB, GPIO_PIN_6, GPIOB, GPIO_PIN_7);

			// Electrovannes
			BSP_GPIO_PinCfg(electrovanne_EC.GPIO, electrovanne_EC.PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
			BSP_GPIO_PinCfg(electrovanne_EP.GPIO, electrovanne_EP.PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
			setElectrovanne(electrovanne_EC); // Vanne ouverte
			setElectrovanne(electrovanne_EP); // Vanne fermée

			// Boutons


			// Changement d'état
			state = ACCUEIL;
			break;

		case ACCUEIL :
			if(getWater_level(id_sensor, &water_level))
			{
				printf("distance : %d\n", water_level);
				TFT_Acceuil(water_level);
			}
			break;

		case ACTUALISATION :

			break;

		case MODE_AUTO:
			break;
		case MODE_MANUEL:
			break;
		case MODE_OFF:
			break;
	}

}
