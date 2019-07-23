#include "infrared.h"
#include "Wait_Clock.h"
#include "usart1.h"

int Non = 0;
int Add_Data = 0;
int Con_Data = 0;

//初始化红外和设置中断
void Infrared_Init()
{
	GPIO_InitTypeDef	GPIO_InitStruct;
	EXTI_InitTypeDef	EXTI_InitStruct;
	NVIC_InitTypeDef	NVIC_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_8;
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_25MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource8);
	
	EXTI_InitStruct.EXTI_Line	= EXTI_Line8;
	EXTI_InitStruct.EXTI_LineCmd	= ENABLE;
	EXTI_InitStruct.EXTI_Mode	= EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger	= EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel				= EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd			= ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority	= 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority		= 0;
	NVIC_Init(&NVIC_InitStruct);
	
}

//中断函数
void EXTI9_5_IRQHandler()
{
	int count = 0;
	EXTI_ClearITPendingBit(EXTI_Line8);
	
	//printf("%d", Non);调试代码
	
	Non = 0;
	if(Add_Data != 0 &&Con_Data != 0)return;
	if(!ReadGPA8){
		
		//引导码9ms低电平
		while(!ReadGPA8){
			delay_ms(1);
			if(++count > 12){
				Non = -11;
				return;
			}
		}
		
		if(count < 6){
			Non = -12;
			return;
		}
		count = 0;
		
		//引导码4.5ms高电平
		while(ReadGPA8){
			delay_ms(1);
			if(++count > 8){
				Non = -13;
				return;
			}
		}
		if(count < 4){
			Non = -14;
			return;
		}
		count = 0;
		
		//开始接收数据并检验
		GetData();
		if(Non != 0)return;
		
		//检测结束位
		
		//printf("AData:%d  CData:%d\r\n", Add_Data, Con_Data);
		Non = 0;
	}else{
		Non = -10;
	}
}

void GetData()
{
	int count = 0;
	int i = 0;
	if(!ReadGPA8){
		//接收地址码开始
		Add_Data = 0;
		while(i < 8){
			while(!ReadGPA8){
				delay_us(1);
				if(++count > 600){
					Non = -21;
					return;
				}
			}
//			if(count < 400){
//				Non = -22;
//				return;
//			}
			
			count = 0;
			
			
			
			while(ReadGPA8){
				delay_us(1);
				if(++count > 600){
					Add_Data |= (0x1 << (7 - i));
				}
			}
			
//			if(count > 180){
//				Non = -23;
//				return;
//			}
			
			
			count = 0;
			
			i++;
		}
		
		//检验地址码
		i = 0;
		while(i < 8){
			while(!ReadGPA8){
				delay_us(1);
				if(++count > 600){
					Non = -24;
					return;
				}
			}
			if(count < 45){
				Non = -25;
				return;
			}
			
			count = 0;

			while(ReadGPA8){
				delay_us(1);
				if(++count > 600){
					if((Add_Data & (0x1 << (7 - i))) != 0){
						Non = -31;
						return;
					}
				}
			}
			
			if(count < 600 && ((Add_Data & (0x1 << (7 - i))) == 0)){
				Non = -32;
				return;
			}
//			if(count > 180){
//				Non = -26;
//				return;
//			}
			
			count = 0;
			i++;
		}
		
		
		//接收控制码开始
		Con_Data = 0;
		i = 0;
		while(i < 8){
			while(!ReadGPA8){
				delay_us(1);
				if(++count > 600){
					Non = -41;
					return;
				}
			}
//			if(count < 400){
//				Non = -42;
//				return;
//			}
			
			count = 0;

			while(ReadGPA8){
				delay_us(1);
				if(++count > 600){
					Con_Data |= (0x1 << (7 - i));
				}
			}
//			if(count > 180){
//				Non = -43;
//				return;
//			}
			count = 0;
			i++;
		}
		
		//检验控制码
		i = 0;
		while(i < 8){
			while(!ReadGPA8){
				delay_us(1);
				if(++count > 600){
					Non = -44;
					return;
				}
			}
			if(count < 400){
				Non = -45;
				return;
			}
			count = 0;

			while(ReadGPA8){
				delay_us(1);
				if(++count > 600){
					if((Con_Data & (0x1 << (7 - i))) != 0){
						Non = -46;
						return;
					}
				}
			}
			if(count < 600 && ((Con_Data & (0x1 << (7 - i))) == 0)){
				Non = -47;
				return;
			}
//			if(count > 180){
//				Non = -48;
//				return;
//			}
			count = 0;
			i++;
		}
	}
}
