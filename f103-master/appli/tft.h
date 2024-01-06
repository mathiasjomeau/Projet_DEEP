#ifndef TFT_H_
#define TFT_H_

#include "config.h"

#define NB_SCREENS 4
#define NB_MAX_PARAMETERS 8

void TFT_Init(void);

void TFT_Acceuil(void);
void TFT_Acceuil_Update(uint8_t mode, uint8_t current_mode, uint8_t id_mode, uint16_t water_level, uint8_t EC_state, uint8_t EP_state);
/*
void TFT_Mode_Auto(void);

void TFT_Update_capteurs(uint16_t water_level, uint8_t EC_state, uint8_t EP_state);

void TFT_Select_Mode(uint8_t mode);

void TFT_Change_CurrentMode (uint8_t mode);
*/

#endif /* TFT_H_ */
