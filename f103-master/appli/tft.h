/**
  ******************************************************************************
  * @file    tft.h
  * @author  Mathias Jomeau
  * @date    14-January-2024
  * @brief   Fichier d'en-tête pour l'écran TFT
  ******************************************************************************
*/

#ifndef TFT_H_
#define TFT_H_

#include "config.h"

/**
 * @brief Initialise l'écran TFT.
 * Elle doit être appelée avant toute utilisation de l'écran.
 */
void TFT_Init(void);

/**
 * @brief Met à jour les informations des capteurs affichées sur l'écran TFT.
 * @param water_level Niveau d'eau en pourcentage.
 * @param EC_state État de l'électrovanne de la cuve.
 * @param EP_state État de l'électrovanne d'eau.
 * @param eau_temperature Température de l'eau.
 */
void TFT_InformationsSensors_Update(uint16_t water_level, bool_e EC_state, bool_e EP_state, float eau_temperature);

/**
 * @brief Affiche l'écran d'accueil sur l'écran TFT.
 */
void TFT_Acceuil(void);

/**
 * @brief Met à jour l'écran d'accueil
 * @param mode Mode à afficher.
 * @param current_mode Mode actuel.
 * @param id_mode Identifiant du mode à surligner dans la sélection des menus
 */
void TFT_Acceuil_Update(uint8_t mode, uint8_t current_mode, uint8_t id_mode);

/**
 * @brief Affiche l'écran du mode manuel sur l'écran TFT.
 */
void TFT_Mode_Manuel(void);

/**
 * @brief Met à jour l'écran du mode manuel
 * @param id_mode Identifiant du mode à surligner dans la sélection des menus
 */
void TFT_Mode_Manual_Update(uint8_t id_mode);

/**
 * @brief Affiche l'écran des paramètres sur l'écran TFT.
 */
void TFT_Mode_Parametre(void);

/**
 * @brief Met à jour l'écran du mode paramètres
 * @param id_mode Identifiant du mode à surligner dans la sélection des menus
 */
void TFT_Mode_Parametre_Update(uint8_t id_mode);

/**
 * @brief Affiche l'écran de modification de la cuve sur l'écran TFT.
 */
void TFT_Parametre_Cuve(void);

/**
 * @brief Met à jour l'écran de modification de la cuve avec la taille spécifiée.
 * @param taille_cuve Taille de la cuve en dm
 */
void TFT_Parametre_Cuve_Update(uint16_t taille_cuve);

/**
 * @brief Affiche l'écran de modification des alertes sur l'écran TFT.
 */
void TFT_Parametre_Alertes(void);

/**
 * @brief Met à jour l'écran de modification des alertes avec l'état d'activation spécifié.
 * @param activation État d'activation des alertes.
 */
void TFT_Parametre_Alertes_Update(bool_e activation);

/**
 * @brief Affiche un écran d'annonce sur l'écran TFT.
 * @param ligne1 Texte de la première ligne d'annonce.
 * @param ligne2 Texte de la deuxième ligne d'annonce.
 */
void TFT_Annonce (char * ligne1, char * ligne2);

#endif /* TFT_H_ */
