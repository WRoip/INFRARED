#ifndef _INFRARED_H_
#define _INFRARED_H_

/************************* ͷ�ļ� **************************/
#include "stm32f4xx.h"

/************************ �������� *************************/

/************************* �궨�� **************************/
#define ReadGPA8 (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == 1)

/************************ �������� *************************/
 /*
  * ��  �ܣ�Infrared��ʼ��
  * ��  ����void
  * ����ֵ��void
  *
 */
void Infrared_Init(void);

void GetData(void);

#endif
