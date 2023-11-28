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

#include "composants/tft.h"

static void state_machine(void);

static volatile uint32_t t = 0;
void process_ms(void)
{
	if(t)
		t--;
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

	while(1)	//boucle de t�che de fond
	{
		test_HCSR04();
	}
}

static void state_machine(void)
{
	typedef enum{
		INIT,
		ACCUEIL
	}state_e;

	static state_e state = INIT;


	switch(state)
	{
		case INIT :
			// Ecran TFT
			TFT_Init();

			// HCSRO4
			HCSR04_add(&id_sensor, GPIOB, GPIO_PIN_6, GPIOB, GPIO_PIN_7);

			// Electrovannes
			BSP_GPIO_PinCfg(ELECTROVANNE0_GPIO, ELECTROVANNE0_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
			BSP_GPIO_PinCfg(ELECTROVANNE1_GPIO, ELECTROVANNE1_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);

			HAL_GPIO_WritePin(ELECTROVANNE0_GPIO, ELECTROVANNE0_PIN, 1); // vanne fermée
			HAL_GPIO_WritePin(ELECTROVANNE1_GPIO, ELECTROVANNE1_PIN, 1); // vanne fermée

			// Boutons

			// Changement d'état
			state = ACCUEIL;
			break;

		case ACCUEIL :
			TFT_Acceuil();
	}

}
