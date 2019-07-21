#include "Wait_Clock.h"

u8 	my_us = 0;
u16 my_ms = 0;

//��ʼ���δ�ʱ��
void Systick_Init(void)
{
	//�õ���Systickʱ�� 21MHZ  �����ʱ��ÿ����һ�� 1/21us   
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); 
	my_us = 21;        //21
	my_ms = 21000;
}

//΢����ʱ,nus���ֵ:798915
void delay_us(u32 nus)   
{
	u32 temp = 0;
	//���Զ���װ�س�ֵ�Ĵ���д����ʱnus  SysTick->LOAD���ֵ0xFFFFFF
	SysTick->LOAD = nus*my_us;  //10*21
	SysTick->VAL  = 0x00;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //��������
	do
	{
		//�����ƼĴ���
		temp = SysTick->CTRL;
	}while(!(temp & (1<<16)));   //�ж��Ƿ������0
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //�رռ���
	SysTick->VAL  = 0x00;
}

//������ʱ,nms���ֵ��nms���ֵ798.915
void delay_ms(u32 nms)
{
	u32 temp = 0;
	//���Զ���װ�س�ֵ�Ĵ���д����ʱnms  SysTick->LOAD���ֵ0xFFFFFF
	SysTick->LOAD = nms*my_ms;
	SysTick->VAL  = 0x00;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //��������
	do
	{
		//�����ƼĴ���
		temp = SysTick->CTRL;
	}while(!(temp & (1<<16)));
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //�رռ���
	SysTick->VAL  = 0x00;

}

void delay_s(u32 ns)
{
	for(; ns>0; ns--)
	{
		delay_ms(500);
		delay_ms(500);
	}
}
