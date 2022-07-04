
#include "hmc5883l.h"
#include "i2c.h"
#include "main.h"
#include "math.h"


//==========================HMC5883函数定义=============================

unsigned char HMC5883_SB_Read(unsigned char Slave_Address, unsigned char Register_Address) 
{
	unsigned char REG_data;
	
	HAL_I2C_Mem_Read(&hi2c2, Slave_Address, Register_Address, I2C_MEMADD_SIZE_8BIT, &REG_data, 1, HAL_MAX_DELAY);
	
	return REG_data;	
}


unsigned char HMC5883_SB_Write(unsigned char Slave_Address, unsigned char Register_Address, unsigned char Register_Data)
{
	HAL_I2C_Mem_Write(&hi2c2, Slave_Address, Register_Address, I2C_MEMADD_SIZE_8BIT, &Register_Data, 1, HAL_MAX_DELAY);
	
	return 0;
     
}


void HMC5883_Init(void)
{
	
    HMC5883_SB_Write(SlaveAddress, 0X00, 0X58); //写寄存器A，30Hz数据输出、采样平均数0
    HMC5883_SB_Write(SlaveAddress, 0X01, 0X40); //写寄存器B，传感器量程+-0.88Ga、增益1370高斯
    HMC5883_SB_Write(SlaveAddress, 0X02, 0X00); //写寄存器C，连续数据输出
    
}

short HMC_X,HMC_Y;//,HMC_Z; //HMC5883三轴数据输出
float HMC5883_Get_Angle(void)
{
	unsigned char i ;
	
	float Angle ;
	
	short Recive_Data[6] ;   //store temperary data
	
	
	
//	HMC5883_Init() ;
	
	for(i=0; i<6; i++)
	{
		Recive_Data[i] = HMC5883_SB_Read(SlaveAddress, i+3) ;  //get data
	}

	HMC_X = Recive_Data[0]<<8 | Recive_Data[1];//Combine MSB and LSB of X Data output register
//	HMC_Z = Recive_Data[2]<<8 | Recive_Data[3];//Combine MSB and LSB of Z Data output register
	HMC_Y = Recive_Data[4]<<8 | Recive_Data[5];//Combine MSB and LSB of Y Data output register

	Angle= atan2((double)HMC_Y,(double)HMC_X) * (180 / 3.14159265) + 180; // angle in degrees
	return Angle ;
}

