 #include "serial.h" 
    char RxBuf1[256]={0}; //自定义的接收缓存
    static uint8_t Num_U1RxByte = 0;    //USART1 接收的数据字节数
    static uint8_t U1RxF = 0; //索引，指向USART1 接收的数据字缓存中未读数据的第一个
    static uint8_t U1RxL = 0;//索引，指向USART1 接收的数据字缓存中未读数据的最后一个
    char Rx1; //一个临时接收变量
    //这里将缓存用作FIFO，将索引定义为8位无符号数，范围正是缓存的大小，
    //溢出后正好回到缓存的开始，遍历所有缓存
    
    
    char RxBuf2[256]={0};
    static uint8_t Num_U2RxByte = 0;    
    static uint8_t U2RxF = 0;
    static uint8_t U2RxL = 0;
    char Rx2;

/*
    *@brief: 从USART1缓存中读走一个字节
 */ 
char USART1_ReadByte(void){
    if(Num_U1RxByte == 0) return '\0';  
    Num_U1RxByte--;
    return RxBuf1[U1RxF++];
}
/*
    *@brief: USART1发送一个字节，直至发送完，超过10s算超时，返回1
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
    *@brief: 获取USART1缓存RxBuf1中缓存的字节数
 */
uint8_t USART1_RecNum(void){
        return Num_U1RxByte;
}
uint8_t USART2_RecNum(void){
        return Num_U2RxByte;
}

/*
    *@brief: 配置USART1，且使波特率为baud
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
    *@brief: USART1发送输出一个字符串，直至发送完毕
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
    *@brief: USART2读取num个数据，保存的buffer中
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
*@brief: 从缓存中等待读取一整行数据
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
    *@brief: RTOS API中USART1的事件回调函数
    *@function：在这里仅将将数据转移到缓存中
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
            U1RxF++; //当buffer满了丢弃最先收到的1个数据
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

