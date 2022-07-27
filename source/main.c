 /*----------------------------------------------------------------------------
    
    Designers Guide to the Cortex-M Family
    CMSIS RTOS Threads Example

*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
  Include the microcontroller header for register defenitions and CMSIS core functions
    Include the CMSIS RTOS header for the RTOS API
 *---------------------------------------------------------------------------*/

//#include "STM32F10x.h"
#include <cmsis_os2.h>
#include <cmsis_os.h>
#include "boardled.h"
#include "boardkey.h"
#include "serial.h"

static uint64_t thread1_stk[128];
static uint64_t thread1_cdm[128];
void KEY_Event(uint8_t num){
static uint8_t falg01 = 1;
    static uint8_t falg02 = 1;
    if(num == 4){
        if(falg01){
            LED_On(0);
            falg01 = 0;
        }
        else{
            LED_Off(0);
            falg01 = 1;
        }
    }
    else if(num == 5)
        {
        if(falg02){
            LED_On(1);
            falg02 = 0;
        }
        else{
            LED_Off(1);
            falg02 = 1;
        }
    }
}

void thread1 (void *argument) 
{
    KEY_Initialize ();
    LED_Initialize ();
    osSignalWait(0x01, osWaitForever);
}

void thread2 (void *argument) 
{
    //osSignalWait(0x01, osWaitForever);
  char temp1;
    char temp2;

    Serial1_Conf(9600);
    Serial2_Conf(9600);

    Serial1_Print("Usart1 Initialize Finished!\r\n");
    osDelay(100);

    while(1){
        if(USART1_RecNum()){
        USART2_SendByte(USART1_ReadByte());     
        }
        if(USART2_RecNum()){
      USART1_SendByte(USART2_ReadByte());   
        }
    }
}

static osThreadAttr_t thread2_attr;

int main (void) 
{
    thread2_attr.stack_mem  = thread1_stk;
  thread2_attr.stack_size = sizeof(thread1_stk);
    //thread1_attr.priority = osPriorityHigh;
    thread2_attr.cb_mem = thread1_cdm;
    thread2_attr.cb_size = sizeof(thread1_cdm);
    SystemCoreClockUpdate();
    osKernelInitialize ();                    // initialize CMSIS-RTOS
    osThreadNew(thread1,NULL,NULL);
    osThreadNew(thread2,NULL,&thread2_attr);
    osKernelStart();                         // start thread execution 
    while(1);
}
