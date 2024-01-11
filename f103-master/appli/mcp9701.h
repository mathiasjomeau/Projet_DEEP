#ifndef MCP9701_H_
#define MCP9701_H_

#include "config.h"
#include "macro_types.h"

void MCP9701_Init();

void MCP9701_GetTemperature (float * temperature);

#endif /* MCP9701_H_ */
