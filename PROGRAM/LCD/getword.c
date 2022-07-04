
#include "getword.h"





void LCD_Show_Chinese_word_32size(uint16_t x,uint16_t y,uint16_t fc,unsigned char*an)
{
	uint8_t i,j;
	uint16_t x0=x;
	
	//LCD_Address_Set(x,y,x+31,y+31);//设置要写入数据的区域
	
	for(i=0;i<128;i++) //每个32大小的汉字为128个字节，循环128字节
	{
		for(j=0;j<8;j++) //每个字节是数据，比较每一位的数据是否是1 ，是1就设置该像素点的颜色
		{	
			if(an[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//画一个点
			x++;
			if((x-x0)==32)
			{
				x=x0;
				y++;
				break;
			}
		}
	}
}


void LCD_Show_Chinese_word_24size(uint16_t x,uint16_t y,uint16_t fc,unsigned char*an)
{
	uint8_t i,j;
	uint16_t x0=x;
	
	//LCD_Address_Set(x,y,x+23,y+23);//设置要写入数据的区域
	
	for(i=0;i<72;i++) //每个32大小的汉字为128个字节，循环128字节
	{
		for(j=0;j<8;j++) //每个字节是数据，比较每一位的数据是否是1 ，是1就设置该像素点的颜色
		{	
			if(an[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//画一个点
			x++;
			if((x-x0)==24)
			{
				x=x0;
				y++;
				break;
			}
		}
	}
}


void LCD_show_one_chinese(uint16_t x,uint16_t y,uint16_t color,uint8_t *string, unsigned char size)
{
	long int word_address = 0x00;
	
	unsigned char word_data[128] = {0};
	
	
	//打开W26Q64的片选信号，关闭屏幕的片选信号。
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
		
	/*
	address=((H-0xA1)×94+L-0XA1)×(size);
	其中H、L 分别代表GB2312 的第一个字节和第二个字节(也就是高位和低位)，
	比如“好”字
	16进制为0xba(186) 0xc3(195),
	使用16X16字体，则字体有32个字节，
	使用32X32字体，则字体有128个字节，
	这里使用16大小的做运算。
	0xA1的10进制为161
	94是固定值
	[(186-161)*94 +195 - 161]* 32 = 76288 得到0x12a00 
	位置在0x12a00 76288 
	*/
	if(size == 32)
	{
		word_address = ( (string[0]-0xa1)*94 + string[1] - 0xa1)*128;
		
		FLASH_Read_Data(word_address,128,word_data);
		

		//打开屏幕的片选信号，关闭W26Q64的片选信号。
		HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
		
		LCD_Show_Chinese_word_32size(x,y,color,word_data);
	}
	if(size == 24)
	{
		word_address = GB2312_24X24_heiti + ( (string[0]-0xa1)*94 + string[1] - 0xa1)*72;
		
		FLASH_Read_Data(word_address,72,word_data);
		

		//打开屏幕的片选信号，关闭W26Q64的片选信号。
		HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
		
		LCD_Show_Chinese_word_24size(x,y,color,word_data);
	}
	
	
}






void LCD_show_Chinese(uint16_t x, uint16_t y, uint16_t color, uint8_t *string, uint8_t length ,unsigned char size)
{
	uint8_t an[2] = {0};
	uint16_t i=0;

	for(i=0; i < length*2; i=i+2)
	{
		an[0]= string[i];
		an[1]= string[i+1];
		LCD_show_one_chinese(x, y, color, an, size);
		if(size == 32)
			x = x + 32;
		if(size == 24)
			x = x + 20;
	}
}










