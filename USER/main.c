#include "stm32f4xx.h"
#include "infrared.h"
#include "Wait_Clock.h"
#include "usart1.h"

extern int Add_Data;
extern int Con_Data;

int main(void)
{
	Systick_Init();
	Infrared_Init();
	Usart1_Init(115200);
	
	printf("初始化成功！开始监测红外！\r\n");
	
	while(1){
		if(Con_Data != 0){
			if(Con_Data == 162)printf("1\r\n");
			else if(Con_Data == 98)printf("2\r\n");
			else if(Con_Data == 226)printf("3\r\n");
			else if(Con_Data == 34)printf("4\r\n");
			else if(Con_Data == 2)printf("5\r\n");
			else if(Con_Data == 194)printf("6\r\n");
			else if(Con_Data == 224)printf("7\r\n");
			else if(Con_Data == 168)printf("8\r\n");
			else if(Con_Data == 144)printf("9\r\n");
			else if(Con_Data == 104)printf("*\r\n");
			else if(Con_Data == 152)printf("0\r\n");
			else if(Con_Data == 176)printf("#\r\n");
			else if(Con_Data == 24)printf("U\r\n");
			else if(Con_Data == 16)printf("L\r\n");
			else if(Con_Data == 56)printf("OK\r\n");
			else if(Con_Data == 90)printf("R\r\n");
			else if(Con_Data == 74)printf("D\r\n");
			Con_Data = 0;
		}
		
	}
	
}
