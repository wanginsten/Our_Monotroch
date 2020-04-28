#include "System.h"
#include "string.h"
#include "cmd.h"
#include "JY901.h"

extern UART_HandleTypeDef huart1;

uint8_t cmd_get[10];//从串口获得的值
uint8_t cmd_value_NOW[10];//现在的命令
uint8_t cmd_value_LAST[10];//上一次的命令
uint8_t STOP[10]	=			{'S','T','O','P','*','*','*','*','*','*'};
uint8_t GYRO[10]	=			{'G','Y','R','O','*','*','*','*','*','*'};
uint8_t SET_NUMMBER[10]={'S','E','T','N','U','M','*','*','*','*'};
uint8_t overflow[10]=		{'O','V','E','R','F','L','O','W','*','*'};
int k,cmd_flag;
int one_time_flag;//让提示只显示一次;1不能显示下一次;0能显示下一次
uint8_t cmd_num;//在cmd获得的数字

/*
	判断两个uint8_t [10]的'*'之前的值是否相等
	相等return 1;
	不相等return 0;
*/
int JUDGEMENT(uint8_t a[10],uint8_t b[10])
{
	int j=0,judgement=0,level=0;
	while(j<10)
	{
		if(a[j]==b[j])judgement++;
		level++;
		if((a[j]==b[j])&&a[j]=='*')break;
		j++;
	}
	if(judgement==level)return 1;
	else return 0;
}
/*
	把后一项的'*'前的部分复制到前一项
*/
void COPY_behind_to_before(uint8_t a[10],uint8_t b[10])
{
	int j;
	for(j=0;j<10;j++)
	{
		a[j]=b[j];
		if(b[j]=='*')break;
	}
}
/*
	把接收到的数字保存到cmd_num
	还没完成
*/
void get_the_num(void)
{
	if(cmd_flag==SET_NUM)
	{
		int kk=0;
		int n=0;
		uint8_t a[10];
		cmd_num=0;
		while(cmd_value_NOW[kk]!='*')
		{
			switch(cmd_value_NOW[kk])
			{
				case '0':a[kk]=0;break;
				case '1':a[kk]=1;break;
				case '2':a[kk]=2;break;
				case '3':a[kk]=3;break;
				case '4':a[kk]=4;break;
				case '5':a[kk]=5;break;
				case '6':a[kk]=6;break;
				case '7':a[kk]=7;break;
				case '8':a[kk]=8;break;
				case '9':a[kk]=9;break;
				default: return;
			}
			kk++;
		}
		a[kk]='*';
		while(a[n]!='*')
		{
			cmd_num=cmd_num*10+a[n];
			n++;
		}
	}
}

void cmd_Init(void)
{
	k=0;
	cmd_flag=0;
	
	one_time_flag=1;
}

void cmd_control(uint8_t Rx)
{
		//把接收到的内容放到'*'之前
		int j;
		if(Rx=='*')
		{
			cmd_get[k]='*';
			k=0;
			
			for(j=0;j<10;j++)
				cmd_value_NOW[j]=cmd_get[j];
		}
		else
		{
			cmd_get[k]=Rx;
			k++;
			if(k>10)
			{
				HAL_UART_Transmit(&huart1,overflow,sizeof(overflow),100);
				COPY_behind_to_before(cmd_value_NOW,cmd_value_LAST);
			}
		}
		
		
		
		//根据命令改变cmd_flag，解读命令
		if(JUDGEMENT(cmd_value_NOW,cmd_value_LAST)==0)
		{
			if(JUDGEMENT(cmd_value_NOW,STOP))
			{
				cmd_flag=0;
				one_time_flag=0;
			}
			else if(JUDGEMENT(cmd_value_NOW,GYRO))
			{
				cmd_flag=GYRO_REPORT;
				one_time_flag=0;
			}
			else if(JUDGEMENT(cmd_value_NOW,SET_NUMMBER))
			{
				cmd_flag=SET_NUM;
				one_time_flag=0;
			}
				
				
			//再初始化
			COPY_behind_to_before(cmd_value_LAST,cmd_value_NOW);
		}
	
}

void cmd_doing(void)
{
	switch(cmd_flag)
	{
		case 0:
			if(one_time_flag==0)
			{
				HAL_UART_Transmit(&huart1,STOP,sizeof(STOP),100);
				one_time_flag=1;
			
			cmd_Init();
			}
			break;
		case GYRO_REPORT:
			if(one_time_flag==0)
			{
				HAL_UART_Transmit(&huart1,GYRO,sizeof(GYRO),100);
			}
			gyro_reporter();//这个函数内部还没有写
			break;
		case SET_NUM:
			if(one_time_flag==0)
			{
				HAL_UART_Transmit(&huart1,SET_NUMMBER,sizeof(SET_NUMMBER),100);
			}
			get_the_num();
	}
}

