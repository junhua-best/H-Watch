#include "SI7021.h"
//#include "delay.h"
#include "usart.h"
#include "i2c.h"
#include "stdio.h"

//结构体定义
_si7021_value si7021;//数据缓存区结构体
_si7021_filter si7021_filter;//平均值滤波器结构体

//变量定义，滤波后的最终结果，可使用串口打印
float TEMP_buf,Humi_buf;


#define BufferSize 1					//读写大小(注意不要设置太大)
 
#define FLT_TO_UINT32(m, e)           (((uint32_t)(m) & 0x00FFFFFFU) | (uint32_t)((int32_t)(e) << 24))

 
//函数名称：Multiple_read_Si7021
//函数功能：多字节读取传感器
//参数描述：
//返 回 值：
void Multiple_read_Si7021(u8 REG_address, u16 *value)
{
	u8 Si7021_BUF[2]={0};
	HAL_StatusTypeDef status;
	
	/* 
	第1个参数为I2C操作句柄
	第2个参数为从机设备地址
	第3个参数为从机寄存器地址
	第4个参数为从机寄存器地址长度
	第5个参数为发送的数据的起始地址
	第6个参数为传输数据的大小
	第7个参数为操作超时时间 
	*/
	status =	HAL_I2C_Mem_Read(&hi2c2, SLAVE_ADDR<<1, REG_address, I2C_MEMADD_SIZE_8BIT, Si7021_BUF, 2, HAL_MAX_DELAY);

//	printf("%02x %02x\r\n",Si7021_BUF[0],Si7021_BUF[1]);


	if(status != HAL_OK)
	{
		switch(status)
		{
			case 0x00U: printf("IIC正常 \r\n"); break;
			case 0x01U: printf("IIC错误 \r\n"); break;
			case 0x02U: printf("IIC忙碌 \r\n"); break;
			case 0x03U: printf("IIC超时 \r\n"); break;
		}
	}
	
	
	*value=((Si7021_BUF[0]<<8)+Si7021_BUF[1]);
}

//函数名称：measure_si7021
//函数功能：NO HOLD MASTER模式下读取温湿度 
//参数描述：无
//返 回 值：无
void measure_Si7021(void)
{
	//缓存变量定义
	u16 TEMP,HUMI;

	//读取温度
	Multiple_read_Si7021(TEMP_HOLD_MASTER,&TEMP);
	TEMP_buf=(((((float)TEMP)*175.72f)/65536.0f) - 46.85f);
	
	//读取湿度
	Multiple_read_Si7021(HUMI_HOLD_MASTER,&HUMI);
	Humi_buf=(((((float)HUMI)*125.0f)/65535.0f) - 6.0f);
}




