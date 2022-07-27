#ifndef __SERIAL_H
#define __SERIAL_H
#include "GPIO_STM32F10x.h"
#include "Driver_USART.h"
#include <cmsis_os.h>

extern ARM_DRIVER_USART Driver_USART1;//导入RTOS API的USART1 
extern ARM_DRIVER_USART Driver_USART2;
#define Serial1 Driver_USART1 //转换为更加熟悉的Serial1
#define Serial2 Driver_USART2

char USART1_ReadByte(void);
char USART1_SendByte(char data);
char USART2_ReadByte(void);
char USART2_SendByte(char data);
uint8_t USART2_Read(char buffer[],uint8_t num);
uint8_t USART2_ReadLine(char buffer[]);
uint8_t USART1_RecNum(void);
uint8_t USART2_RecNum(void);
void USART1_callback(uint32_t event);
void USART2_callback(uint32_t event);
void Serial2_Conf(uint32_t baud);
void Serial1_Conf(uint32_t baud);
int Serial1_Print(char str[]);
int Serial2_Print(char str[]);
#endif   /*__SERIAL_H*/
