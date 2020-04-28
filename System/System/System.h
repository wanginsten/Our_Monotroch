#ifndef __SYSTEM_H
#define __SYSTEM_H
#include "stm32f4xx_hal.h"
#include "System.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "JY901.h"
#include "calculate.h"
#include "vague_pid.h"
#include "cmd.h"

void System_Init(void);
void Time_Space_Rolling(void);
void SystemClock_Config(void);

#endif
