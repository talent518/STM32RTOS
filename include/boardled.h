#ifndef __BOARDLED_H
#define __BOARDLED_H
#include "GPIO_STM32F10x.h"

int32_t LED_Initialize (void);
int32_t LED_Uninitialize (void);
int32_t LED_On (uint32_t num);
int32_t LED_Off (uint32_t num);
int32_t LED_SetOut (uint32_t val);
uint32_t LED_GetCount (void);

void thread_keyrun (void *argument);
int32_t KEY_Initialize (void);

#endif   /*__BOARDLED_H*/
