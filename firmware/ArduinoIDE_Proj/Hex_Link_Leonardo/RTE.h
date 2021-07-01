#ifndef RTE_H
#define RTE_H
#include "Universal_Headers.h"
#include "ASW.h"
extern void Task_Init(void);

extern void OsTask_200ms(void *pvParameters);
extern void OsTask_100ms(void *pvParameters);
extern void OsTask_50ms(void *pvParameters);
extern void OsTask_20ms(void *pvParameters);

#endif
