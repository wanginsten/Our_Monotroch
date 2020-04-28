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

void JY901_Data(uint8_t ucDATA)	//ÿ�յ�һ�����ݣ�����һ���������
{
	static unsigned char ucRxBuffer[250];
	static unsigned char ucRxCnt = 0;	
	
	
	ucRxBuffer[ucRxCnt++]=ucDATA;	//���յ������ݴ��뻺������
	if (ucRxBuffer[0]!=0x55) //����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) {return;}//���ݲ���11�����򷵻�
	else
	{
		switch(ucRxBuffer[1])//�ж��������������ݣ�Ȼ���俽������Ӧ�Ľṹ���У���Щ���ݰ���Ҫͨ����λ���򿪶�Ӧ������󣬲��ܽ��յ�������ݰ�������
		{
			case 0x50:	memcpy(&stcTime,&ucRxBuffer[2],8);break;//memcpyΪ�������Դ����ڴ濽��������������"string.h"�������ջ��������ַ����������ݽṹ�����棬�Ӷ�ʵ�����ݵĽ�����
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
		ucRxCnt=0;//��ջ�����
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
	
	JY901_User.Roll_angle=stcAngle.Angle[0]/K_angle;//ʵ�鷢��˵�����е�ϵ�������⣬������������1000
	JY901_User.Pitch_angle=stcAngle.Angle[1]/K_angle;
	JY901_User.Yaw_angle=stcAngle.Angle[2]/K_angle;
	
	//JY901_User.T=stcAngle.T/100;
	
}

float Average_Filtering(Average_Filtering_typedef *x,float get)	//��ֵ�˲�
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
