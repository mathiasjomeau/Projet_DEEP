/**
  ******************************************************************************
  * @file    test_unitaire.h
  * @author  Mathias Jomeau
  * @date    14-January-2024
  * @brief   Fichier d'en-tête pour les tests unitaires
  ******************************************************************************
*/

#ifndef TEST_UNITAIRE_H_
#define TEST_UNITAIRE_H_

#include "config.h"

/**
 * @brief Fonction de test pour l'écran TFT.
 */
void TEST_EcranTFT(void);

/**
 * @brief Fonction de test pour les électrovannes.
 */
void TEST_Electrovanne(void);

/**
 * @brief Fonction de test pour le capteur de température MCP9701.
 */
void TEST_MCP9701(void);

/**
 * @brief Fonction de test pour le capteur de distance HCSR04.
 */
void TEST_HCSR04(void);

#endif /* TEST_UNITAIRE_H_ */
