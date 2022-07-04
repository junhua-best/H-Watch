#include "lcd_init.h"

#include "spi.h"
/******************************************************************************
      ����˵����LCD��������д�뺯��
      ������ݣ�dat  Ҫд��Ĵ�������
      ����ֵ��  ��
******************************************************************************/
void LCD_Writ_Bus(uint8_t dat) 
{	
	HAL_SPI_Transmit(&hspi1,&dat,1,10);//��������
	//HAL_SPI_Transmit_DMA(&hspi1,&dat,1);//��������
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA8(uint8_t dat)
{
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA(uint16_t dat)
{
	LCD_Writ_Bus(dat>>8);
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_REG(uint8_t dat)
{
	LCD_DC_Clr();//д����
	LCD_Writ_Bus(dat);
	LCD_DC_Set();//д����
}


/******************************************************************************
      ����˵����������ʼ�ͽ�����ַ
      ������ݣ�x1,x2 �����е���ʼ�ͽ�����ַ
                y1,y2 �����е���ʼ�ͽ�����ַ
      ����ֵ��  ��
******************************************************************************/
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	if(USE_HORIZONTAL==0)
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//������д
	}
	else if(USE_HORIZONTAL==1)
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1+80);
		LCD_WR_DATA(y2+80);
		LCD_WR_REG(0x2c);//������д
	}
	else if(USE_HORIZONTAL==2)
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//������д
	}
	else
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1+80);
		LCD_WR_DATA(x2+80);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//������д
	}
}



void write_data(uint8_t dat) 
{	
	HAL_SPI_Transmit(&hspi1,&dat,1,10);//��������
}

void LCD_Init(void)
{
	unsigned char write_data_an[6] = {0x0C, 0x0C, 0x00, 0x33, 0x33};
	
	unsigned char write_data_an1[14] = {0xD0,0x04,0x0D,0x11,0x13,0x2B,0x3F,0x54,0x4C,0x18,0x0D,0x0B,0x1F,0x23};
	
	unsigned char write_data_an2[15] = {0xD0,0x04,0x0C,0x11,0x13,0x2C,0x3F,0x44,0x51,0x2F,0x1F,0x1F,0x20,0x23,0x21};
	
	
	LCD_RES_Clr();//��λ
	HAL_Delay(10);
	LCD_RES_Set();
	HAL_Delay(10);
	
	//LCD_BLK_Set();//�򿪱���
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
	HAL_SPI_Transmit(&hspi1,write_data_an,5,10);//��������

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
	HAL_SPI_Transmit(&hspi1,write_data_an1,14,10);//��������
	
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
	HAL_SPI_Transmit(&hspi1,write_data_an2,15,10);//��������
	
	LCD_WR_REG(0x21); 

	LCD_WR_REG(0x29); 
} 








