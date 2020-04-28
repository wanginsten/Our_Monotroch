#ifndef __TIM_H
#define __TIM_H
#include "System.h"
#include "tim.h"

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
void MX_TIM2_Init(void);
void MX_TIM3_Init(void);
void MX_TIM4_Init(void);


#endif
