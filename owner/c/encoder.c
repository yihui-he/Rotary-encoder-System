#include "all.h"

struct Encoder_Stat Encoders[6]=
{
	{0,0,0,-0.319001,-0.318843,1,0.0,0,0},
	{0,0,0,-0.320864,-0.320624,-1,0.0,PI/2,0},
};
fp64 N[8]={0};//用来标定码盘时记录用的

int32_t GET_ENCODER(int8_t i)
{
	int32_t temp;
	assert_param((i>0)&&(i<5));
	temp = (int32_t)(((int32_t)(r_now[i]))*CPR+(int32_t)(Tim_S[i]->CNT));
	return temp;
}

void Encoder_Update(void)
{
	int i;
	int64_t now;
	int32_t step;
	
	for(i=0;i<4;i++)
	{
		now = GET_ENCODER(i);
		step=now-Encoders[i].Now;
		if(step<2000&&step>-2000)
		{
		       Encoders[i].Now=now;
	         Encoders[i].Total=Encoders[i].Now-Encoders[i].Last;
		       if (step==Int32Abs(step)) 
		       {
			    	Encoders[i].Distance+=Int32Abs(step)*Encoders[i].Convert1*Encoders[i].dir;
		       }
		       else
		       {
		    		Encoders[i].Distance-=Int32Abs(step)*Encoders[i].Convert2*Encoders[i].dir;
		       }
	    }
	}
}

void Encoder_Clear(int index)
{
	Encoders[index].Distance=0.0;
	Encoders[index].Last=Encoders[index].Now;
	Encoders[index].Total=0;
}


void Encoder_InitXY(uint8_t i)
{
	N[i]=Encoders[0].Total;
	N[i+2]=Encoders[1].Total;
}

void Encoder_InitR(void)
{
	Encoders[0].Radius=Encoders[0].Distance/(10*PI+Gyro_Radian_Total);
	Encoders[1].Radius=Encoders[1].Distance/(10*PI+Gyro_Radian_Total);
}

/****************************************************************************************************
码盘标定过程：分别沿+y,-y,+x,-x四个方向推三米，记录两个码盘的读数，以下为N[8]数组与码盘读数的对应关系
     +y    -y    +x    -x
E1   N[0]  N[1]  N[4]  N[5]
E2   N[2]  N[3]  N[6]  N[7]
****************************************************************************************************/
void Encoder_Init(void)
{ 
	fp64 C11,C12,C21,C22;
	fp64 C11_1,C11_2,C12_1,C12_2,C21_1,C21_2,C22_1,C22_2;
	fp64 angle_a,angle_b;
	fp64 angle_a1,angle_a2,angle_b1,angle_b2;
	
	if(N[0]<0)
	{
		N[0]=-N[0];
		N[1]=-N[1];
		N[4]=-N[4];
		N[5]=-N[5];
	}
	
	if(N[6]<0)
	{
		N[2]=-N[2];
		N[3]=-N[3];
		N[6]=-N[6];
		N[7]=-N[7];
	}
	
    switch((int)(N[4]/fabs(N[4])))
    {
      case 1:
        angle_a1=atan(-N[4]/N[0]);
        angle_a2=atan(-N[5]/N[1]);
        angle_a=(angle_a1+angle_a2)/2;
        C11_1=3000*cos(angle_a)/N[0];
        C11_2=-3000*sin(angle_a)/N[4];
        C11 = (C11_1+C11_2)/2;
        C12_1=-3000*cos(angle_a)/N[1];
        C12_2=3000*sin(angle_a)/N[5];
        C12 =(C12_1+C12_2)/2;
        break;
      case -1:
        angle_a1=atan(N[5]/N[0]);
        angle_a2=atan(N[4]/N[1]);
        angle_a=(angle_a1+angle_a2)/2;
        C11_1=3000*cos(angle_a)/N[0];
        C11_2=3000*sin(angle_a)/N[5];
        C11 = (C11_1+C11_2)/2;
        C12_1=-3000*cos(angle_a)/N[1];
        C12_2=-3000*sin(angle_a)/N[4];
        C12 =(C12_1+C12_2)/2;
        break;
    }
    
    switch((int)(N[2]/fabs(N[2])))
    {
      case 1:
        angle_b1=atan(N[6]/N[2]);
        angle_b2=atan(N[7]/N[3]);
        angle_b=(angle_b1+angle_b2)/2;
        C21_1=3000*sin(angle_b)/N[6];
        C21_2=3000*cos(angle_b)/N[2];
        C21 = (C21_1+C21_2)/2;
        C22_1=-3000*sin(angle_b)/N[7];
        C22_2=-3000*cos(angle_b)/N[3];
        C22 =(C22_1+C22_2)/2;
        break;
      case -1:
        angle_b1=atan(-N[6]/N[3])+PI;
        angle_b2=atan(-N[7]/N[2])+PI;
        angle_b=(angle_b1+angle_b2)/2; 
        C21_1=3000*sin(angle_b)/N[6];
        C21_2=-3000*cos(angle_b)/N[3];
        C21 = (C21_1+C21_2)/2;
        C22_1=-3000*sin(angle_b)/N[7];
        C22_2=3000*cos(angle_b)/N[2];
        C22 =(C22_1+C22_2)/2;
        break;  
    }
	Encoders[0].radian=angle_a;
	Encoders[1].radian=angle_b;
	Encoders[0].Convert1=C11;
	Encoders[0].Convert2=C12;
	Encoders[1].Convert1=C21;
	Encoders[1].Convert2=C22;
	
}
