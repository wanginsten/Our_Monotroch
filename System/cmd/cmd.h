#ifndef __CMD_H
#define __CMD_H
#include "System.h"
#include "cmd.h"

#define CMD_GET_FLAG  0	///cmd���������״̬
#define CMD_STE_NUM 1		///�������ݵ�״̬

#define GYRO_REPORT 1
#define SET_NUM			2

void cmd_Init(void);
void cmd_control(uint8_t Rx);
void cmd_doing(void);

#endif
