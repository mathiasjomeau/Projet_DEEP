#include "stm32f1_ili9341.h"

void TFT_Init(void)
{
	ILI9341_Init();
	ILI9341_Rotate(ILI9341_Orientation_Landscape_2);
	ILI9341_Fill(ILI9341_COLOR_WHITE);

}

void TFT_Acceuil(void)
{
	ILI9341_PutBigs(30, 30, "HYDRESEO", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 2, 2)
}




