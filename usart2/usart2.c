#include "usart2.h"
/*
 u8 mode_data[8];
 u8 six_data_stop[3]={0X59,0X59,0X59};  //ֹͣ��������
 u8 six_data_start[3]={0X58,0X58,0X58};  //������������
 */
int32_t distance_left;
int32_t degree_left;

int32_t distance_right;
int32_t degree_right;

uint8_t Uart2_head1;
uint8_t Uart2_head2;

uint8_t sta1;
uint8_t sta2;

uint8_t left_or_right;
 
uint8_t Uart2_Buffer[128];        //���ջ����� 

uint8_t Uart2_Rx; 



void uart2_init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);    
	
	//USART2_TX   GPIOA.2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.2
   
  //USART2_RX	  GPIOA.3��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.3 

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART2, &USART_InitStructure); //��ʼ������2
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���2

}

void USART2_IRQHandler(void)  
{  
  if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET)  
  {   
		
		Uart2_Buffer[Uart2_Rx] = USART_ReceiveData(USART2); 
		Uart2_Rx++;		
		//USART_SendData(USART1,USART_ReceiveData(USART2));
  }
	
	if(Uart2_Buffer[Uart2_Rx-1] == 0x66)   //�ж�֡ͷ1
		Uart2_head1 = Uart2_Rx-1;
	
	else if(Uart2_Rx-1 == Uart2_head1+1) //�ж�֡ͷ1���ݺ��Ƿ�Ϊ֡ͷ2
	{
		if(Uart2_Buffer[Uart2_Rx-1] == 0x00)
		{sta1 = 1;left_or_right=1;}
		else if(Uart2_Buffer[Uart2_Rx-1] == 0x01)
		{sta1 = 1;left_or_right=2;}
		else{
			Uart2_Rx=0;
			sta1=0;
			sta2=0;
			Uart2_head1=0;
			Uart2_head2=0;
			left_or_right=0;}
	}
	
	else if(Uart2_Rx-1 == Uart2_head1+5)
	{
		if(Uart2_Buffer[Uart2_Rx-1] == 0x70)
		{Uart2_head2=Uart2_Rx-1;}
		else {		
			Uart2_Rx=0;
			sta1=0;
			sta2=0;
			Uart2_head1=0;
			Uart2_head2=0;
			left_or_right=0;}
	}
	
	else if(Uart2_Rx-1 == Uart2_head2+1) //�ж�֡ͷ1���ݺ��Ƿ�Ϊ֡ͷ2
	{
		if(Uart2_Buffer[Uart2_Rx-1] == 0x76)
		{sta2 = 1;}
		else{
			Uart2_Rx=0;
			sta1=0;
			sta2=0;
			Uart2_head1=0;
			Uart2_head2=0;
			left_or_right=0;}
	}
	
	
	
	if(sta1&&sta2)
	{
		if(left_or_right==1)
		{
		distance_left=Uart2_Buffer[Uart2_head1+2];
		degree_left=(Uart2_Buffer[Uart2_head1+3]<<8)|Uart2_Buffer[Uart2_head1+4];
		
		Uart2_Rx=0;
		sta1=0;
		sta2=0;
		Uart2_head1=0;
		Uart2_head2=0;
		left_or_right=0;
		}
		else if(left_or_right==2)
		{
		distance_right=Uart2_Buffer[Uart2_head1+2];
		degree_right=(Uart2_Buffer[Uart2_head1+3]<<8)|Uart2_Buffer[Uart2_head1+4];
		
		Uart2_Rx=0;
		sta1=0;
		sta2=0;
		Uart2_head1=0;
		Uart2_head2=0;
		left_or_right=0;
		}
	}
}
