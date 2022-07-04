#ifndef _HMC5883L_H
#define _HMC5883L_H


#define SlaveAddress 0X3C     //HMC5883L从机地址

extern short HMC_X,HMC_Y,HMC_Z; //HMC5883三轴数据输出

unsigned char HMC5883_SB_Read(unsigned char Slave_Address, unsigned char Register_Address) ;

unsigned char HMC5883_SB_Write(unsigned char Slave_Address, unsigned char Register_Address, unsigned char Register_Data) ;

void HMC5883_Init(void) ;

float HMC5883_Get_Angle(void) ;

#endif


