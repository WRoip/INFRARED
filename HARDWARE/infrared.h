#ifndef _INFRARED_H_
#define _INFRARED_H_

/************************* 头文件 **************************/
#include "stm32f4xx.h"

/************************ 变量声明 *************************/

/************************* 宏定义 **************************/
#define ReadGPA8 (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == 1)

/************************ 函数声明 *************************/
 /*
  * 功  能：Infrared初始化
  * 参  数：void
  * 返回值：void
  *
 */
void Infrared_Init(void);

void GetData(void);

#endif
