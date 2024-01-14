#ifndef TFT_H_
#define TFT_H_

#include "config.h"

#define NB_MAX_PARAMETERS 30

void TFT_Init(void);

void TFT_InformationsSensors_Update(uint16_t water_level, bool_e EC_state, bool_e EP_state, float eau_temperature);

void TFT_Acceuil(void);
void TFT_Acceuil_Update(uint8_t mode, uint8_t current_mode, uint8_t id_mode);

void TFT_Mode_Manuel(void);
void TFT_Mode_Manual_Update(uint8_t id_mode);

void TFT_Mode_Parametre(void);
void TFT_Mode_Parametre_Update(uint8_t id_mode);
void TFT_Parametre_Cuve(void);
void TFT_Parametre_Cuve_Update(uint16_t taille_cuve);
void TFT_Parametre_Alertes(void);
void TFT_Parametre_Alertes_Update(bool_e activation);

void TFT_Annonce (char * ligne1, char * ligne2);
/*
void TFT_Mode_Auto(void);

void TFT_Update_capteurs(uint16_t water_level, uint8_t EC_state, uint8_t EP_state);

void TFT_Select_Mode(uint8_t mode);

void TFT_Change_CurrentMode (uint8_t mode);
*/

#endif /* TFT_H_ */
