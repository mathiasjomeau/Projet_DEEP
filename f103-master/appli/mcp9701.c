#include "mcp9701.h"
#include "systick.h"
#include "stm32f1_adc.h"

adc_id_e ID_temperature = ADC_4;

static volatile uint16_t t = 0;

void ADC_process_1ms(void);
float Conversion_TensionToTemp(float tension);

void ADC_process_1ms(void)
{
	if(t)
		t--;
}

float Conversion_TensionToTemp(float tension)
{
    // Caractéristiques du MCP9701
    const float tension_zero_degres = 400.0;  // 500 mV à 0°C
    const float pente_mV_par_degre = 19.5;    // 10 mV/°C

    // Calcul de la température en degrés Celsius
    float temperature = (tension - tension_zero_degres) / pente_mV_par_degre;

    return temperature;
}

void MCP9701_Init()
{
	ADC_init();
	Systick_add_callback_function(ADC_process_1ms);
}

void MCP9701_GetTemperature (float * temperature)
{
	int16_t value;
	int16_t millivolt;
	float temp;

	if(!t)
	{
		t = 400;
		value = ADC_getValue(ID_temperature);
		millivolt = (int16_t)((((int32_t)value)*3300)/4096);
		//printf("Temp (mV) : %1d.%03dV \n ",millivolt/1000, millivolt%1000);
		temp = Conversion_TensionToTemp ((float) millivolt);
		*temperature = temp;
		printf("Temp (degre) : %.1f\n", temp);
	}
}
