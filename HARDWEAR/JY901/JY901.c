#include "stm32f4xx_hal.h"
#include "JY901.h"
#include "string.h"
#include "System.h"

struct STime		stcTime;
struct SAcc 		stcAcc;
struct SGyro 		stcGyro;
struct SAngle 	stcAngle;
struct SMag 		stcMag;
struct SDStatus stcDStatus;
struct SPress 	stcPress;
struct SLonLat 	stcLonLat;
struct SGPSV 		stcGPSV;
struct SQ       stcQ;

Average_Filtering_typedef Roll_w_Average_Filter;
Average_Filtering_typedef Pitch_w_Average_Filter;
Average_Filtering_typedef Yaw_w_Average_Filter;

JY901_User_typedef JY901_User;

extern uint8_t JY901_Rx[22];

void JY901_Data(uint8_t ucDATA)	//每收到一个数据，调用一次这个函数
{
	static unsigned char ucRxBuffer[250];
	static unsigned char ucRxCnt = 0;	
	
	
	ucRxBuffer[ucRxCnt++]=ucDATA;	//将收到的数据存入缓冲区中
	if (ucRxBuffer[0]!=0x55) //数据头不对，则重新开始寻找0x55数据头
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) {return;}//数据不满11个，则返回
	else
	{
		switch(ucRxBuffer[1])//判断数据是哪种数据，然后将其拷贝到对应的结构体中，有些数据包需要通过上位机打开对应的输出后，才能接收到这个数据包的数据
		{
			case 0x50:	memcpy(&stcTime,&ucRxBuffer[2],8);break;//memcpy为编译器自带的内存拷贝函数，需引用"string.h"，将接收缓冲区的字符拷贝到数据结构体里面，从而实现数据的解析。
			case 0x51:	memcpy(&stcAcc,&ucRxBuffer[2],8);break;
			case 0x52:	memcpy(&stcGyro,&ucRxBuffer[2],8);break;
			case 0x53:	memcpy(&stcAngle,&ucRxBuffer[2],8);break;
			case 0x54:	memcpy(&stcMag,&ucRxBuffer[2],8);break;
			case 0x55:	memcpy(&stcDStatus,&ucRxBuffer[2],8);break;
			case 0x56:	memcpy(&stcPress,&ucRxBuffer[2],8);break;
			case 0x57:	memcpy(&stcLonLat,&ucRxBuffer[2],8);break;
			case 0x58:	memcpy(&stcGPSV,&ucRxBuffer[2],8);break;
			case 0x59:	memcpy(&stcQ,&ucRxBuffer[2],8);break;
		}
		ucRxCnt=0;//清空缓存区
	}
}

void In_Rx_Callback(void)
{
	int k;
	for(k=0;k<22;k++)
	{
		JY901_Data(JY901_Rx[k]);
	}
	
	JY901_User.Roll_w=Average_Filtering(&Roll_w_Average_Filter,(double)(stcGyro.w[0]*101725)*(1e-8));
	JY901_User.Pitch_w=Average_Filtering(&Pitch_w_Average_Filter,(double)(stcGyro.w[1]*101725)*(1e-8));
	JY901_User.Yaw_w=Average_Filtering(&Yaw_w_Average_Filter,(double)(stcGyro.w[2]*101725)*(1e-8));
	
	JY901_User.Roll_angle=stcAngle.Angle[0]/K_angle;//实验发现说明书中的系数有问题，这里随便调成了1000
	JY901_User.Pitch_angle=stcAngle.Angle[1]/K_angle;
	JY901_User.Yaw_angle=stcAngle.Angle[2]/K_angle;
	
	//JY901_User.T=stcAngle.T/100;
	
}

float Average_Filtering(Average_Filtering_typedef *x,float get)	//均值滤波
{
	float temp=0;
	x->a[x->k]=get;
	x->k++;
	if(x->k>=8)x->k=0;
	for(int m=0;m<8;m++)
	{
		temp+=x->a[m];
	}
	temp*=0.125f;
	return temp;
}
void gyro_reporter(void)
{
	
}
