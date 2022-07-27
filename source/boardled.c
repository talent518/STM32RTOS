#include "boardled.h"
#include <cmsis_os.h>

/*There is 2 LED , LDE1 is ctrolled by PB0, LDE2 ctrolled by PB1*/
const GPIO_PIN_ID Pin_LED[] = { //引脚描述结构体
    {GPIOB,  0},
    {GPIOB,  1},
};
#define LED_COUNT (sizeof(Pin_LED)/sizeof(GPIO_PIN_ID))

int32_t LED_Initialize (void) { //使用RTOS API配置GPIO
  uint32_t n;
  /* Configure pins: Push-pull Output Mode (50 MHz) with Pull-down resistors */
  for (n = 0; n < LED_COUNT; n++) {
    GPIO_PortClock   (Pin_LED[n].port, true);
    GPIO_PinWrite    (Pin_LED[n].port, Pin_LED[n].num, 0);
    GPIO_PinConfigure(Pin_LED[n].port, Pin_LED[n].num,
                      GPIO_OUT_PUSH_PULL,
                      GPIO_MODE_OUT2MHZ);
  }
  return 0;
}

int32_t LED_Uninitialize (void) {
  uint32_t n;

  /* Configure pins: Input mode, without Pull-up/down resistors */
  for (n = 0; n < LED_COUNT; n++) {
    GPIO_PinConfigure(Pin_LED[n].port, Pin_LED[n].num,
                      GPIO_IN_FLOATING,
                      GPIO_MODE_INPUT);
  }
  return 0;
}

int32_t LED_On (uint32_t num) {   //开LED灯，如果超出操作范围，返回错误值-1
  int32_t retCode = 0;

  if (num < LED_COUNT) {
    GPIO_PinWrite(Pin_LED[num].port, Pin_LED[num].num, 1);
  }
  else {
    retCode = -1;
  }
  return retCode;
}

int32_t LED_Off (uint32_t num) {
  int32_t retCode = 0;

  if (num < LED_COUNT) {
    GPIO_PinWrite(Pin_LED[num].port, Pin_LED[num].num, 0);
  }
  else {
    retCode = -1;
  }

  return retCode;
}

int32_t LED_SetOut (uint32_t val) {
  uint32_t n;

  for (n = 0; n < LED_COUNT; n++) {
    if (val & (1<<n)) {
      LED_On (n);
    } else {
      LED_Off(n);
    }
  }

  return 0;
}

uint32_t LED_GetCount (void) {

  return LED_COUNT;
}
