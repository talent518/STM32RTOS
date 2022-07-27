 /*由于RTOS中没有外部中断的API，因此这里直接使用标准库的函数来配置*/
#include "boardkey.h"

extern void KEY_Event(uint8_t num);

int32_t KEY_Initialize (void) {
        GPIO_InitTypeDef GPIO_InitStructure; 
        EXTI_InitTypeDef EXTI_InitStructure;
        NVIC_InitTypeDef NVIC_InitStructure;    
    
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);//打开GPIO AFIO的时钟
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
      GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource4); //选择EXTI信号源 
        EXTI_ClearITPendingBit(EXTI_Line4);                                                                       
    EXTI_InitStructure.EXTI_Line = EXTI_Line4;               //中断线选择
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;      //EXTI为中断模式
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                //使能中断
    EXTI_Init(&EXTI_InitStructure); 
 
      GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5); //选择EXTI信号源
        EXTI_ClearITPendingBit(EXTI_Line5);
      EXTI_InitStructure.EXTI_Line = EXTI_Line5;               //中断线选择

    EXTI_Init(&EXTI_InitStructure); 
        
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);           //配置NVIC优先级分组为1
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;        //中断源stm32f10x.h”中
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级：1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        //子优先级：1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //使能中断通道
    NVIC_Init(&NVIC_InitStructure);
        
        NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn; 
        NVIC_Init(&NVIC_InitStructure);
  return 0;
}
//中断服务函数的名称要特别小心，因为你随便写错一点，编译不会产生报错，但是会导致程序无法进入中断服务程序，甚至卡死。不确定的话可以到启动文件startup_stm32f10x_md.s中查找。
void EXTI4_IRQHandler(void)  //按键1，PC4中断服务函数                     
{
  if(EXTI_GetITStatus(EXTI_Line4)!= RESET)  
  {  
    EXTI_ClearITPendingBit(EXTI_Line4);
      KEY_Event(4);
  }   
}

void EXTI9_5_IRQHandler(void)                       
{
  if(EXTI_GetITStatus(EXTI_Line5)!= RESET)  
  {  
    EXTI_ClearITPendingBit(EXTI_Line5);
      KEY_Event(5);
  }   
}
