/*由于RTOS中没有外部中断的API，因此这里直接使用标准库的函数来配置*/
#ifndef __BOARDKEY_H
#define __BOARDKEY_H
#include "GPIO_STM32F10x.h"
#include "stm32f10x_exti.h"
#include <cmsis_os.h>

int32_t KEY_Initialize (void);

#endif   /*__BOARDKEY_H*/
