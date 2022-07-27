 #include "serial.h" 
    char RxBuf1[256]={0}; //�Զ���Ľ��ջ���
    static uint8_t Num_U1RxByte = 0;    //USART1 ���յ������ֽ���
    static uint8_t U1RxF = 0; //������ָ��USART1 ���յ������ֻ�����δ�����ݵĵ�һ��
    static uint8_t U1RxL = 0;//������ָ��USART1 ���յ������ֻ�����δ�����ݵ����һ��
    char Rx1; //һ����ʱ���ձ���
    //���ｫ��������FIFO������������Ϊ8λ�޷���������Χ���ǻ���Ĵ�С��
    //��������ûص�����Ŀ�ʼ���������л���
    
    
    char RxBuf2[256]={0};
    static uint8_t Num_U2RxByte = 0;    
    static uint8_t U2RxF = 0;
    static uint8_t U2RxL = 0;
    char Rx2;

/*
    *@brief: ��USART1�����ж���һ���ֽ�
 */ 
char USART1_ReadByte(void){
    if(Num_U1RxByte == 0) return '\0';  
    Num_U1RxByte--;
    return RxBuf1[U1RxF++];
}
/*
    *@brief: USART1����һ���ֽڣ�ֱ�������꣬����10s�㳬ʱ������1
 */ 
char USART1_SendByte(char data){
    uint16_t i = 0;
    Serial1.Send(&data,1);
    for(;i<1000;i++){
        if(Serial1.GetTxCount()>=1)return 0;
        osDelay(10);
    }
    return 1;
}
char USART2_ReadByte(void){
    if(Num_U2RxByte == 0) return '\0';  
    Num_U2RxByte--;
    return RxBuf2[U2RxF++];
}
char USART2_SendByte(char data){
    uint16_t i = 0;
    Serial2.Send(&data,1);
    for(;i<1000;i++){
        if(Serial2.GetTxCount()>=1)return 0;
        osDelay(10);
    }
    return 1;
}
/*
    *@brief: ��ȡUSART1����RxBuf1�л�����ֽ���
 */
uint8_t USART1_RecNum(void){
        return Num_U1RxByte;
}
uint8_t USART2_RecNum(void){
        return Num_U2RxByte;
}

/*
    *@brief: ����USART1����ʹ������Ϊbaud
 */
void Serial1_Conf(uint32_t baud){
        /*Initialize the USART driver */
    Serial1.Initialize(USART1_callback);
    /*Power up the USART peripheral */
    Serial1.PowerControl(ARM_POWER_FULL);
    /*Configure the USART to baud Bits/sec */
    Serial1.Control(ARM_USART_MODE_ASYNCHRONOUS |
                      ARM_USART_DATA_BITS_8 |
                      ARM_USART_PARITY_NONE |
                      ARM_USART_STOP_BITS_1 |
                      ARM_USART_FLOW_CONTROL_NONE, baud);
     
    /* Enable Receiver and Transmitter lines */
    Serial1.Control (ARM_USART_CONTROL_TX, 1);
    Serial1.Control (ARM_USART_CONTROL_RX, 1);
        Serial1.Receive(&Rx1,1);
}
void Serial2_Conf(uint32_t baud){
        /*Initialize the USART driver */
    Serial2.Initialize(USART2_callback);
    /*Power up the USART peripheral */
    Serial2.PowerControl(ARM_POWER_FULL);
    /*Configure the USART to baud Bits/sec */
    Serial2.Control(ARM_USART_MODE_ASYNCHRONOUS |
                      ARM_USART_DATA_BITS_8 |
                      ARM_USART_PARITY_NONE |
                      ARM_USART_STOP_BITS_1 |
                      ARM_USART_FLOW_CONTROL_NONE, baud);
     
    /* Enable Receiver and Transmitter lines */
    Serial2.Control (ARM_USART_CONTROL_TX, 1);
    Serial2.Control (ARM_USART_CONTROL_RX, 1);
        Serial2.Receive(&Rx2,1);
}
/*
    *@brief: USART1�������һ���ַ�����ֱ���������
 */
int Serial1_Print(char str[]){
    int length = 0;
    uint16_t i = 0;
    while(str[length++]);
    length--;
    Serial1.Send(str,length);
    for(;i<1000;i++){
        if(Serial1.GetTxCount()>=length)return length;
        osDelay(30);
    }
    return -1;
}
int Serial2_Print(char str[]){
    int length = 0;
    uint16_t i = 0;
    while(str[length++]);
    length--;
    Serial2.Send(str,length);
    for(;i<1000;i++){
        if(Serial2.GetTxCount()>=length)return length;
        osDelay(30);
    }
    return -1;
}
/*
    *@brief: USART2��ȡnum�����ݣ������buffer��
 */
uint8_t USART2_Read(char buffer[],uint8_t num){
    uint8_t count = 0;
    for(;count<num;count++){
        buffer[count] = USART2_ReadByte();
    }
    buffer[count] = '\0';
    return count;
}
/*
*@brief: �ӻ����еȴ���ȡһ��������
*@note: for line ,condition is "\r\n" 
*/
uint8_t USART2_ReadLine(char buffer[]){
    uint8_t p=0;
    while(1){
        if(USART2_RecNum()){
            buffer[p] = USART2_ReadByte();
            if(buffer[p] == '\n'){
                buffer[--p] = 0;
                break;
            }
            else{
                p++;
            }
        }
    }
    return p;
}
/*
    *@brief: RTOS API��USART1���¼��ص�����
    *@function�����������������ת�Ƶ�������
*/
void USART1_callback(uint32_t event){
    /*
  uint32_t mask;
  mask = ARM_USART_EVENT_RECEIVE_COMPLETE  |
         ARM_USART_EVENT_TRANSFER_COMPLETE |
         ARM_USART_EVENT_SEND_COMPLETE     |
         ARM_USART_EVENT_TX_COMPLETE       ;
  if (event & mask) {
  }
    if (event & mask) {
  // Success: Wakeup Thread 
  //osSignalSet(tid2, 0x01);
  }
  if (event & ARM_USART_EVENT_RX_TIMEOUT) {
//    __breakpoint(0); //  Error: Call debugger or replace with custom error handling 
    }
  if (event & (ARM_USART_EVENT_RX_OVERFLOW | ARM_USART_EVENT_TX_UNDERFLOW)) {
//    __breakpoint(0);  // Error: Call debugger or replace with custom error handling 
  }
    */

    if (event & ARM_USART_EVENT_RECEIVE_COMPLETE) {
        RxBuf1[U1RxL++] = Rx1;
        Serial1.Receive(&Rx1,1);
        if(Num_U1RxByte < 255)Num_U1RxByte++;
        else{
            U1RxF++; //��buffer���˶��������յ���1������
        }
  }
    /*
    if (event & ARM_USART_EVENT_TRANSFER_COMPLETE) {
        U1Tx_Flag = true;
  }*/
}

void USART2_callback(uint32_t event){

    if (event & ARM_USART_EVENT_RECEIVE_COMPLETE) {
        RxBuf2[U2RxL++] = Rx2;
        Serial2.Receive(&Rx2,1);
        if(Num_U2RxByte < 255)Num_U2RxByte++;
        else{
            U2RxF++; 
        }
  }
    /*
    if (event & ARM_USART_EVENT_TRANSFER_COMPLETE) {
        U2Tx_Flag = true;
  }*/
        
}

