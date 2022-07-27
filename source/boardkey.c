 /*����RTOS��û���ⲿ�жϵ�API���������ֱ��ʹ�ñ�׼��ĺ���������*/
#include "boardkey.h"

extern void KEY_Event(uint8_t num);

int32_t KEY_Initialize (void) {
        GPIO_InitTypeDef GPIO_InitStructure; 
        EXTI_InitTypeDef EXTI_InitStructure;
        NVIC_InitTypeDef NVIC_InitStructure;    
    
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);//��GPIO AFIO��ʱ��
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
      GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource4); //ѡ��EXTI�ź�Դ 
        EXTI_ClearITPendingBit(EXTI_Line4);                                                                       
    EXTI_InitStructure.EXTI_Line = EXTI_Line4;               //�ж���ѡ��
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;      //EXTIΪ�ж�ģʽ
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //�½��ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                //ʹ���ж�
    EXTI_Init(&EXTI_InitStructure); 
 
      GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5); //ѡ��EXTI�ź�Դ
        EXTI_ClearITPendingBit(EXTI_Line5);
      EXTI_InitStructure.EXTI_Line = EXTI_Line5;               //�ж���ѡ��

    EXTI_Init(&EXTI_InitStructure); 
        
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);           //����NVIC���ȼ�����Ϊ1
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;        //�ж�Դstm32f10x.h����
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ���1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        //�����ȼ���1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //ʹ���ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);
        
        NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn; 
        NVIC_Init(&NVIC_InitStructure);
  return 0;
}
//�жϷ�����������Ҫ�ر�С�ģ���Ϊ�����д��һ�㣬���벻������������ǻᵼ�³����޷������жϷ������������������ȷ���Ļ����Ե������ļ�startup_stm32f10x_md.s�в��ҡ�
void EXTI4_IRQHandler(void)  //����1��PC4�жϷ�����                     
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
