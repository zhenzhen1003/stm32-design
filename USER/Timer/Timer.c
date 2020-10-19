/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#include "Timer.h"                  // Device header
#include "stm32f10x.h"                  // Device header


//���ò���Ƶ�ʺ����ڵĺ���
void Timer_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* PA0   TIM2��������*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;   //PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50Mʱ���ٶ�
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//����RCC
	RCC_ClocksTypeDef RCC_ClockFreq;
	SystemInit();//Դ��system_stm32f10x.c�ļ�,ֻ��Ҫ���ô˺���,������RCC������.
	RCC_GetClocksFreq(&RCC_ClockFreq);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);    //ʹ��TIM2ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);    //ʹ��TIM2ʱ��
	TIM_ITRxExternalClockConfig(TIM2,TIM_TS_ETRF); //timer������ʱ��ԴĬ����RCC����Ҫ����Ϊ�ⲿETR�������
	TIM_ITRxExternalClockConfig(TIM3,TIM_TS_ITR1); //����TIM3������ʱ��Ϊ�ⲿITR1����ʵ����TIM2�������
	//���ö�ʱ��
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;

   //����TIMER2��Ϊ������
  TIM_DeInit(TIM2);

  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;   //��������Ϊ65535
	TIM_TimeBaseStructure.TIM_Prescaler = 0x00;  //����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  //����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //���ϼ���
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); // Time base configuration

	TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);  //�����ⲿ�������������Ƶ�������ش���


	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);//ʹ�ø����¼�Ϊ��������
	//ѡ��TIM2Ϊ����ʱ��
	TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable);
	//�ر�TIM��ʱ��
	
	TIM_DeInit(TIM3);
	//����TIM3��ʱ��
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF; 
	TIM_TimeBaseStructure.TIM_Prescaler = 0; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	//����TIM��ģʽΪ�ⲿ����
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_External1);
	//������ʽΪTIM2��ITR1��
	TIM_SelectInputTrigger(TIM3, TIM_TS_ITR1);
	TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);
	//����TIM3�Ĵ�ģʽ
	TIM_SelectMasterSlaveMode(TIM3, TIM_MasterSlaveMode_Enable);
	
	TIM_SetCounter(TIM2, 0);
	TIM_Cmd(TIM2, ENABLE);
	TIM_SetCounter(TIM3, 0);
	TIM_Cmd(TIM3, ENABLE);
}

//��ʱ��2ͨ��1��ͨ��2���벶������
void TIM2_Cap_Init(u16 arr,u16 psc)
{     
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef  TIM2_ICInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    //ʹ�ܶ�ʱ����GPIO��ʱ�� 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);    //ʹ��TIM2ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIOAʱ��
    //��ʼ��IO�ڣ�ģʽΪ����
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;  //PA0 ���֮ǰ����  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 ����  
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA,GPIO_Pin_0);   //PA0 ����    
	
	  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;  //PA0 ���֮ǰ����  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 ����  
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA,GPIO_Pin_1);   //PA0 ����    
    //��ʼ����ʱ��2 TIM2   
    TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
    TIM_TimeBaseStructure.TIM_Prescaler =psc;   //Ԥ��Ƶ��   
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);   
    //��ʼ��TIM2���벶�����
    TIM2_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01     ѡ������� IC1ӳ�䵽TI1��
    TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;    //�����ز���
    TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
    TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;   //���������Ƶ,����Ƶ 
    TIM2_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
    TIM_ICInit(TIM2, &TIM2_ICInitStructure);    
		
    TIM2_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01     ѡ������� IC1ӳ�䵽TI1��
    TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;    //�����ز���
    TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
    TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;   //���������Ƶ,����Ƶ 
    TIM2_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
    TIM_ICInit(TIM2, &TIM2_ICInitStructure);    
    //�жϷ����ʼ��
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�2��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���  
    TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2,ENABLE);//��������ж� ,����CC1IE�����ж�        
    TIM_Cmd(TIM2,ENABLE );     //ʹ�ܶ�ʱ��2 
}

u8  Capture_State=0;//���벶��״̬
u16 Capture_Value;//���벶��ֵ

//��ʱ��2�жϷ������  
void TIM2_IRQHandler(void)
{ 
     if((Capture_State&0X80)==0)//��δ�ɹ����� 1000 0000   
    {
        if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)       
        {       
            if(Capture_State&0X40)//�Ѿ����񵽸ߵ�ƽ��0100 0000
            {
                if((Capture_State&0X3F)==0X3F)//�ߵ�ƽ̫���� 0011 1111
                {
                    Capture_State|=0X80;//��ǳɹ�������һ�� 1000 0000   
                    Capture_Value=0XFFFF; //1111 1111
                }
                else 
                   Capture_State++;
            }    
        }
    if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)//����1���������¼�
        {   
            if(Capture_State&0X40)//����һ���½��� 0100 0000
            {               
                Capture_State|=0X80;//��ǳɹ�����һ��������1000 0000 
                Capture_Value=TIM_GetCapture1(TIM2);
                TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
            }else //��δ��ʼ,��һ�β���������
            {
                Capture_State=0;//���
                Capture_Value=0;
                TIM_SetCounter(TIM2,0);
                Capture_State|=0X40;//��ǲ�����������0100 0000
                TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling); //CC1P=1 ����Ϊ�½��ز���
            }           
        }                                              
     } 
		if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)
		{
			TIM_Cmd(TIM2,DISABLE );
		}
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC2|TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ 
}
