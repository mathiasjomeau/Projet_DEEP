/**
  ******************************************************************************
  * @file    mcp9701.h
  * @author  Mathias Jomeau
  * @date    14-January-2024
  * @brief   Fichier d'en-tête pour le capteur de température (MCP 9701)
  ******************************************************************************
*/

#ifndef MCP9701_H_
#define MCP9701_H_

#include "config.h"
#include "macro_types.h"

/**
 * @brief Initialise le capteur de température MCP9701.
 * Elle doit être appelée avant toute utilisation du capteur
 */
void MCP9701_Init();

/**
 * @brief Obtient la température mesurée par le capteur MCP9701.
 * @param temperature Pointeur pour stocker la température mesurée.
 */
void MCP9701_GetTemperature (float * temperature);

#endif /* MCP9701_H_ */
