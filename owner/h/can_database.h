#ifndef _CAN_DATABASE_H
#define _CAN_DATABASE_H
	#include "stm32f10x.h"
	
	#ifndef NULL
	#define NULL ((void *)0)
	#endif
	
	#define fp64 double
	#define fp32  float

	/*Data_Type*/
	#define READ_ONLY  0	//主控读，外设写
	#define WRITE_ONLY 1	//主控写，外设读
	
	
/*所有命令对应的ID号列表*/
/*
ID位号	[	(GS:GPS Slave  MS:Motor Slave)							]
NO.7	[			0--GS			|		1--Others				]
NO.6	[	0--MOSI		|	1--MISO	|	0--MS MOSI	|	1--Others	]
NO.5~0	[	ID numbers				|	ID numbers  &  Others		]
*/
	typedef enum
	{
//B0x-xxx-xxx 主控&&GPS协主控通信ID*/
	//MOSI   B00-xxx-xxx
        
		M_G_CMD_ID		= 0x00,		//主控向GPS板发命令的ID
		//写数据命令
        W_PWM_ID			= 0xc4,
			//B00-001-xxx  写GPS数据
        
		W_GPS_X_ID		= 0x08,
		W_GPS_Y_ID		= 0x09,
		W_GPS_ANG_ID	= 0x0A,
			//B00-010-xxx  写GYRO数据
		W_GYRO_C1_ID	= 0x10,
		W_GYRO_C2_ID	= 0x11,
			//B00-011-xxx  写ENC1系数
		W_ENC1_C1_ID	= 0x18,
		W_ENC1_C2_ID	= 0x19,
		W_ENC1_RAD_ID	= 0x1A,
		W_ENC1_ANG_ID	= 0x1B,
			//B00-100-xxx  写ENC2系数
		W_ENC2_C1_ID	= 0x20,
		W_ENC2_C2_ID	= 0x21,
		W_ENC2_RAD_ID	= 0x22,
		W_ENC2_ANG_ID	= 0x23,
	//MISO   B01-xxx-xxx
		G_M_CMD_ID		= 0x40,		//GPS板向主控发命令的ID
		//发数据命令
			//B01-001-xxx  发GPS数据
		S_GPS_X_ID		= 0x48,
		S_GPS_Y_ID		= 0x49,
		S_GPS_ANG_ID	= 0x4A,
			//B01-010-xxx  发GYRO数据
		S_GYRO_C1_ID	= 0x50,
		S_GYRO_C2_ID	= 0x51,
		S_GYRO_RAD_ID	= 0x52,
			//B01-011-xxx  发ENC1数据
		S_ENC1_C1_ID	= 0x58,
		S_ENC1_C2_ID	= 0x59,
		S_ENC1_RAD_ID	= 0x5A,
		S_ENC1_ANG_ID	= 0x5B,
		S_ENC1_DIS_ID	= 0x5C,
			//B01-100-xxx  发ENC2数据
		S_ENC2_C1_ID	= 0x60,
		S_ENC2_C2_ID	= 0x61,
		S_ENC2_RAD_ID	= 0x62,
		S_ENC2_ANG_ID	= 0x63,
		S_ENC2_DIS_ID	= 0x64,
//B11-000-xxx 主控&&ISO协主控通信ID*/
	//MOSI   B11-0001-xx

	}ID_NUMDEF;	
		
	/*主控和GPS协主控之间的命令格式*/
  typedef enum
	{
		NO_COMMAND			= 0x00,
		//M_G_CMD_ID下的命令
		GPS_READ			= 0x01,		//索取GPS信息命令
		CHECK_FLOAT			= 0x02,		//通知GPS协主控校准陀螺仪零漂命令
		GYRO_INIT			= 0x03,		//通知GPS协主控标定陀螺仪命令
		ENC_L_INIT			= 0x04,		//通知GPS协主控标定码盘正反转系数命令
		ENC_R_INIT			= 0x05,		//通知GPS协主控标定码盘旋转半径命令
		//G_M_CMD_ID下的命令
		CF_NORMAL			= 0x06,
		CF_CHANGED			= 0x07,
		CF_ERROR			= 0x08
	}COM_TYPE;
	
	typedef struct
	{
		uint8_t  Data_type;
		ID_NUMDEF  Data_ID;
		uint8_t* Data_ptr;
		uint8_t  Data_length;
		
		//在can总线接到数据后，调用的函数
		void (*MenuFunc)(void);			//入口函数		
		uint8_t  Channel;
		uint8_t  Fifo_num;			//在接收方将该ID配置的fifo号
	}Can_Data;

	union fi64_to_u8     //fp64型数据，或int64_t型数据转成u8型数据进行can总线发送
	{
		fp64	 f64_data;
		int64_t  i64_data;
		int32_t  i32_data[2];
		uint16_t u16_data[4];
		uint8_t  u8_data[8];
	};
	
	extern uint8_t Can_Data_Num;
	extern Can_Data Can_Database[];
	extern uint8_t HASH_TABLE[256];
	
	extern uint8_t MOSI_CMD;
	extern uint8_t MISO_CMD;

	extern union fi64_to_u8 MOSI_GPS_X;
	extern union fi64_to_u8 MOSI_GPS_Y;
	extern union fi64_to_u8 MOSI_GPS_ANG;
	extern union fi64_to_u8 MOSI_GYRO_C1;
	extern union fi64_to_u8 MOSI_GYRO_C2;
	extern union fi64_to_u8 MOSI_ENC1_C1;
	extern union fi64_to_u8 MOSI_ENC1_C2;
	extern union fi64_to_u8 MOSI_ENC1_RAD;
	extern union fi64_to_u8 MOSI_ENC1_ANG;
	extern union fi64_to_u8 MOSI_ENC2_C1;
	extern union fi64_to_u8 MOSI_ENC2_C2;
	extern union fi64_to_u8 MOSI_ENC2_RAD;
	extern union fi64_to_u8 MOSI_ENC2_ANG;

	extern union fi64_to_u8 MISO_GPS_X;
	extern union fi64_to_u8 MISO_GPS_Y;
	extern union fi64_to_u8 MISO_GPS_ANG;
	extern union fi64_to_u8 MISO_GYRO_C1;
	extern union fi64_to_u8 MISO_GYRO_C2;
	extern union fi64_to_u8 MISO_GYRO_RAD;
	extern union fi64_to_u8 MISO_ENC1_C1;
	extern union fi64_to_u8 MISO_ENC1_C2;
	extern union fi64_to_u8 MISO_ENC1_RAD;
	extern union fi64_to_u8 MISO_ENC1_ANG;
	extern union fi64_to_u8 MISO_ENC1_DIS;
	extern union fi64_to_u8 MISO_ENC2_C1;
	extern union fi64_to_u8 MISO_ENC2_C2;
	extern union fi64_to_u8 MISO_ENC2_RAD;
	extern union fi64_to_u8 MISO_ENC2_ANG;
	extern union fi64_to_u8 MISO_ENC2_DIS;
	
    extern union fi64_to_u8 MOSI_PWM;
	void Hash_Table_init(void);
	void Write_Database(ID_NUMDEF ID_NUM);

#endif
