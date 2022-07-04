#include "lcd_init.h"

#include "spi.h"
/******************************************************************************
      函数说明：LCD串行数据写入函数
      入口数据：dat  要写入的串行数据
      返回值：  无
******************************************************************************/
void LCD_Writ_Bus(uint8_t dat) 
{	
	HAL_SPI_Transmit(&hspi1,&dat,1,10);//发送数据
	//HAL_SPI_Transmit_DMA(&hspi1,&dat,1);//发送数据
}


/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA8(uint8_t dat)
{
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA(uint16_t dat)
{
	LCD_Writ_Bus(dat>>8);
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      函数说明：LCD写入命令
      入口数据：dat 写入的命令
      返回值：  无
******************************************************************************/
void LCD_WR_REG(uint8_t dat)
{
	LCD_DC_Clr();//写命令
	LCD_Writ_Bus(dat);
	LCD_DC_Set();//写数据
}


/******************************************************************************
      函数说明：设置起始和结束地址
      入口数据：x1,x2 设置列的起始和结束地址
                y1,y2 设置行的起始和结束地址
      返回值：  无
******************************************************************************/
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	if(USE_HORIZONTAL==0)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==1)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1+80);
		LCD_WR_DATA(y2+80);
		LCD_WR_REG(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==2)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
	else
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1+80);
		LCD_WR_DATA(x2+80);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
}



void write_data(uint8_t dat) 
{	
	HAL_SPI_Transmit(&hspi1,&dat,1,10);//发送数据
}

void LCD_Init(void)
{
	unsigned char write_data_an[6] = {0x0C, 0x0C, 0x00, 0x33, 0x33};
	
	unsigned char write_data_an1[14] = {0xD0,0x04,0x0D,0x11,0x13,0x2B,0x3F,0x54,0x4C,0x18,0x0D,0x0B,0x1F,0x23};
	
	unsigned char write_data_an2[15] = {0xD0,0x04,0x0C,0x11,0x13,0x2C,0x3F,0x44,0x51,0x2F,0x1F,0x1F,0x20,0x23,0x21};
	
	
	LCD_RES_Clr();//复位
	HAL_Delay(10);
	LCD_RES_Set();
	HAL_Delay(10);
	
	//LCD_BLK_Set();//打开背光
//	LCD_BLK_Clr();
  HAL_Delay(10);
	
	//************* Start Initial Sequence **********//
	LCD_WR_REG(0x11); //Sleep out 
	HAL_Delay(12);              //Delay 120ms 
	//************* Start Initial Sequence **********// 
	LCD_WR_REG(0x36);
	if(USE_HORIZONTAL==0)write_data(0x00);
	else if(USE_HORIZONTAL==1)write_data(0xC0);
	else if(USE_HORIZONTAL==2)write_data(0x70);
	else write_data(0xA0);

	LCD_WR_REG(0x3A); 
	write_data(0x05);

	LCD_WR_REG(0xB2);
//	write_data(0x0C);
//	write_data(0x0C);
//	write_data(0x00);
//	write_data(0x33);
//	write_data(0x33); 
	HAL_SPI_Transmit(&hspi1,write_data_an,5,10);//发送数据

	LCD_WR_REG(0xB7); 
	write_data(0x35);  

	LCD_WR_REG(0xBB);
	write_data(0x19);

	LCD_WR_REG(0xC0);
	write_data(0x2C);

	LCD_WR_REG(0xC2);
	write_data(0x01);

	LCD_WR_REG(0xC3);
	write_data(0x12);   

	LCD_WR_REG(0xC4);
	write_data(0x20);  

	LCD_WR_REG(0xC6); 
	write_data(0x0F);    

	LCD_WR_REG(0xD0); 
	write_data(0xA4);
	write_data(0xA1);

	LCD_WR_REG(0xE0);
//	write_data(0xD0);
//	write_data(0x04);
//	write_data(0x0D);
//	write_data(0x11);
//	write_data(0x13);
//	write_data(0x2B);
//	write_data(0x3F);
//	write_data(0x54);
//	write_data(0x4C);
//	write_data(0x18);
//	write_data(0x0D);
//	write_data(0x0B);
//	write_data(0x1F);
//	write_data(0x23);
	HAL_SPI_Transmit(&hspi1,write_data_an1,14,10);//发送数据
	
//	LCD_WR_REG(0xE1);
//	write_data(0xD0);
//	write_data(0x04);
//	write_data(0x0C);
//	write_data(0x11);
//	write_data(0x13);
//	write_data(0x2C);
//	write_data(0x3F);
//	write_data(0x44);
//	write_data(0x51);
//	write_data(0x2F);
//	write_data(0x1F);
//	write_data(0x1F);
//	write_data(0x20);
//	write_data(0x23);
	HAL_SPI_Transmit(&hspi1,write_data_an2,15,10);//发送数据
	
	LCD_WR_REG(0x21); 

	LCD_WR_REG(0x29); 
} 








