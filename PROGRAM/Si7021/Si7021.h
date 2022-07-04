#ifndef	_SI7021_H
#define	_SI7021_H

//#include "myiic.h"
#include "main.h"

#define u8		uint8_t
#define u16		uint16_t
#define u32		uint32_t

#define delay_ms HAL_Delay
//SI7021_SCL
//SI7021_SDA

//数据缓存区结构体
typedef	struct
{
	float temp;
	float humi;
	u8 crc;
}_si7021_value;

//平均值滤波器结构体
typedef struct
{
	u8 curI;
	u8 thAmount;
	float tBufs[10];
	float hBufs[10];
}_si7021_filter;

//外部声明，滤波后的最终结果，可使用串口打印
extern float TEMP_buf,Humi_buf;

/************多少个数据参与平均值滤波************/
#define MEAN_NUM  10

/*******************传感器相关*******************/
#define	SLAVE_ADDR		0x40	//7位器件地址

#define	HUMI_HOLD_MASTER	0xE5
#define	TEMP_HOLD_MASTER	0xE3
		
#define SI7013_READ_TEMP 0xE0

#define	HUMI_NOHOLD_MASTER	0xF5
#define	TEMP_NOHOLD_MASTER	0xF3

#define Si7021_RST			0xFE
#define Write_UserReg		0xE6
#define Read_UserReg		0xE7


//void single_write_Si7021(u8 *REG_address);
void Multiple_read_Si7021(u8 REG_address, u16 *value);
void measure_Si7021(void);

#endif
