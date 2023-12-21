#include "config.h"

void TFT_Init(void);

void TFT_Acceuil();

void TFT_Update_capteurs(uint16_t water_level, uint8_t EC_state, uint8_t EP_state);

void TFT_Mode_State(uint8_t mode);
